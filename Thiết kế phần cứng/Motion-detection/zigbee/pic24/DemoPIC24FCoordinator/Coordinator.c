/* Microchip ZigBee2006 Residential Stack
 * Demo Coordinator
 *
 *******************************************************************************
 * FileName:        Coordinator.c
 * Date modified:   13/11/2012
 *
 * Microcontroller: PIC24FJ128GA306
 * Transceiver:     Microchip MRF24J40
 *
 * Editor:          MPLAB X IDE v1.50
 * Complier:        MCC30 v3.31
 *
 * Developer:       Nguyen Tien Dat
 * Class:           KSTN - DTVT - K54
 * Group:           WSAN
 * Organization:    Lab411 - Hanoi University of Technology
 *
 * Description:     Chuong trinh nay mo ta cach setup mot thiet bi tro thanh mot
 *                  node mang dong vai tro la mot Zigbee Coordinator. Chuong trinh
 *                  cung duoc thiet ke de tuong tac voi cac thiet bi khac trong
 *                  mang theo chuan Zigbee (Zigbee Router hoac Zigbee End Device).
 *                  Cac hoat dong do bao gom: cho phep gia nhap/khong gia nhap
 *                  mang, yeu cau ban tin tu mot thiet bi hoac mot nhom thiet bi
 *                  trong mang, gui ban tin toi mot thiet bi hoac mot nhom cac
 *                  thiet bi trong mang.
 * 
 * Note:            Xem lich su ma nguon trong file Readme.txt
*******************************************************************************/
//******************************************************************************
// Header Files
//******************************************************************************

// Include the main ZigBee header file.
#include "zAPL.h"
#include "zNVM.h"
#ifdef I_SUPPORT_SECURITY
    #include "zSecurity.h"
    extern NETWORK_KEY_INFO    currentNetworkKeyInfo;
    #ifdef USE_EXTERNAL_NVM
        extern NETWORK_KEY_INFO plainSecurityKey[2];

        extern BOOL SetSecurityKey(INPUT BYTE index, INPUT NETWORK_KEY_INFO newSecurityKey);
    #endif
#endif

// If you are going to send data to a terminal, include this file.
#if defined(USE_USART) || defined(USE_DEBUG) || defined(USE_CONTROL_PUMP) || defined(USE_MicroWaveS)
    #include "console.h"
#endif

// If you are going to get temperature and humidity, include this file.
#if defined(USE_SHT10)
    #include "delay.h"
    #include "SHT1x.h"
#endif
//******************************************************************************
// Configuration Bits
//******************************************************************************

#if defined(__PIC24F__)
    // Configure by dat_a3cbq91
    _CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & LPCFG_OFF & ICS_PGx1 & WINDIS_OFF & FWDTEN_WDT_SW &  FWPSA_PR128 & WDTPS_PS512);
    _CONFIG2(IESO_ON & FNOSC_FRC & FCKSM_CSECME & OSCIOFCN_OFF & IOL1WAY_OFF & POSCMD_HS);
    _CONFIG3(BOREN_ON);
    // End by dat_a3cbq91
#elif defined(__dsPIC33F__) || defined(__PIC24H__)
    // Explorer 16 board
    _FOSCSEL(FNOSC_PRI)			// primary osc
    _FOSC(OSCIOFNC_OFF & POSCMD_XT)     // XT Osc
    _FWDT(FWDTEN_OFF)			// Disable Watchdog timer
    // JTAG should be disabled as well
#elif defined(__dsPIC30F__)
    // dsPICDEM 1.1 board
    _FOSC(XT_PLL16)		// XT Osc + 16X PLL
    _FWDT(WDT_OFF)		// Disable Watchdog timer
    _FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF)
#else
    #error Other compilers are not yet supported.
#endif

//******************************************************************************
// Constants                        //defined application service   @dat_a3cbq91
//******************************************************************************
//Hardware definations
#if defined(CHAMSOCLAN) || defined(THEODOIDONGVAT) || defined(ROUTER_EMB)
    #define WSAN_LED_TRIS 		TRISFbits.TRISF3
    #define WSAN_LED 			LATFbits.LATF3
#else
    #define POWER_LED_TRIS 		TRISEbits.TRISE4
    #define POWER_LED 			LATEbits.LATE4
#endif
    #define REF_VOLTAGE_TRIS            TRISBbits.TRISB8
    #define REF_ANALOG			ANSBbits.ANSB8

#define ON              1
#define SET             1
#define ENABLE          1
#define OFF             0
#define CLEAR           0
#define DISABLE         0

// Gia tri cho biet bo ADC dang do tin hieu tu cam bien khoi hay tu nguon cap
#define MQ6_Mode_ADC    0b00
#define Power_Mode_ADC  0b01
#define Finish_Convert  0b10

// Cac trang thai cua node mang
// 0x02 --> phat hien co khoi
// 0x03 --> muc nang luong co dau hieu can kiet
// 0x04 --> phat hien co doi tuong dang xam nhap
#define HasSmoke        0x02
#define LowPower        0x03
#define Security        0x04

// Cac gia tri nay dung de thiet lap cho thanh ghi AD1CON2 lay dien ap tham chieu o dau?
#define Measure_MQ6     0b01
#define Measure_Voltage 0b00

// Gia tri nguong cua cac tin hieu tu cam bien khoi va nguon nang luong
// cho phep node gui tin hieu canh bao ve Router-EMB
#define ThresholdMQ6	900
#define ThresholdPower	1300
//******************************************************************************
// Function Prototypes
//******************************************************************************
//functions that have been created and modified by dat_a3cbq91
void HardwareInit( void );
void ProcessNONZigBeeTasks(void);
void ProcessZigBeePrimitives(void);

void SendOneByte(BYTE data, WORD ClusterID);

#if defined(USE_SHT10)
    void Send_HTE_ToRouterEmboard(void);
    void LoadSHT10(void);
#endif
//end by dat_a3cbq91

extern void RemoveAllGroups(void);
extern void RemoveAllBindings(SHORT_ADDR);
extern BYTE LookupAPSAddress( LONG_ADDR *);
extern BOOL APSSaveAPSAddress(APS_ADDRESS_MAP *AddressMap);
//******************************************************************************
// Application Variables
//******************************************************************************

ZIGBEE_PRIMITIVE    currentPrimitive;

SHORT_ADDR          destinationAddress;
#ifdef I_SUPPORT_SECURITY
    extern KEY_VAL	KeyVal;
    #ifdef USE_EXTERNAL_NVM
        extern WORD trustCenterLongAddr;
        extern NETWORK_KEY_INFO plainSecurityKey[2];
    #else
        extern ROM LONG_ADDR trustCenterLongAddr;
    #endif
#endif

//variables has modified by dat_a3cbq91
#if defined(ENERGY_TRACKING)
    WORD Energy_Level;//Luu tru ket qua do dien ap nguon
#endif

#if defined(USE_MQ6)
    WORD Mq6Signal;//Luu tru ket qua do tin hieu tu cam bien khoi MQ6
#endif

#if defined(ENERGY_TRACKING) && defined(USE_MQ6)
    WORD ADC_result;//Luu tru gia tri lay mau tu ADC module
#endif

#if defined(USE_SHT10)
    WORD humidity, temperature;//Luu tru gia tri do am, nhiet do lay tu SHT chua qua xu ly @dat_a3cbq91
#endif

BYTE        i;

static union
{
    struct
    {
                WORD MQ6Warning             : 1;//when has smoke, set this flag
                WORD LowPowerWarning        : 1;//when voltage drop-out, set this flag
                WORD EnableGetDataHTE       : 1;//when need to print humi-temp data on terminal, set this flag
                WORD HTE_Data_Ready         : 1;
                WORD PrintNeighborTable     : 1;
        #if defined(USE_MQ6)||defined(ENERGY_TRACKING)
            #if defined(USE_MQ6)&&defined(ENERGY_TRACKING)
                WORD MQ6orVoltage           : 2;//this flag use to notify what type voltage reference in ADC module
                                                //when we need to use both MQ6 sensor and monitor energy level
                WORD Reserve                : 9;
            #else
                WORD CompleteADC            : 1;//this flag use to notify that complete ADC conversion
                WORD Reserve                : 10;
            #endif
        #else
                WORD Reserve                : 11;
        #endif

    } bits;
    WORD Val;
} WSANFlags;
#define WSAN_FLAGS_INIT       0x0000
//end by dat_a3cbq91

//******************************************************************************
//******************************************************************************
//******************************************************************************
int main(void)
{
    /*******************************create by lab411****************************/
    OSCCON = 0x2001;//current osc HS, new osc FRC

    CLKDIV = 0x0000;

    /************Maping Pins************/
    //unlock registers
    asm volatile ("mov #OSCCONL, w1  \n"
        "mov #0x46, w2     \n"
        "mov #0x57, w3     \n"
        "mov.b w2, [w1]    \n"
        "mov.b w3, [w1]    \n"
        "bclr OSCCON, #6");

    // INT1 >>> RP21
    RPINR0bits.INT1R = 21;

    /*------SPI1------*/
    // SCK1 >> RP26
    RPINR20bits.SCK1R = 26;
    RPOR13bits.RP26R  = 8;
    // SDO1 >> RP19
    RPOR9bits.RP19R   = 7;
    // SDI1 >> RP27
    RPINR20bits.SDI1R = 27;

    /*------SPI2------*/
    // SCK2 >> RP3
    RPINR22bits.SCK2R = 3;
    RPOR1bits.RP3R    = 11;
    //SDO2 >> RP2
    RPOR1bits.RP2R    = 10;
    // SDI2 >> RP4
    RPINR22bits.SDI2R = 4;

    /*------UART1-----*/
    // RX1 >> RP22
    RPINR18bits.U1RXR = 22;
    // TX1 >> RP25
    RPOR12bits.RP25R  = 3;

    /*------UART2-----*/
    // RX2 >> RP24
    RPINR19bits.U2RXR = 24;
    // TX2 >> RP23
    RPOR11bits.RP23R  = 5;

    //lock register
    asm volatile ("mov #OSCCONL, w1  \n"
        "mov #0x46, w2     \n"
        "mov #0x57, w3     \n"
        "mov.b w2, [w1]    \n"
        "mov.b w3, [w1]    \n"
        "bset OSCCON, #6");
    /*********************************end by lab411*****************************/
    CLRWDT();
    ENABLE_WDT();

    /* Initialize the UART such that data can be sent and recieved on terminal */
    #if defined(USE_USART) && defined(USE_DEBUG)
    ConsoleInit();
        printf("\r\nInit UART2");
    #endif

    /* Initialize the hardware before initializing the ZigBee Stack */
    HardwareInit();
    #if defined(USE_USART) && defined(USE_DEBUG)
        printf("\r\nInit Hardware");
    #endif

    /* if using security force the use of key slot 0 in stack */
    #ifdef I_SUPPORT_SECURITY
    {
        BYTE i;
        i = 0xff;
        PutNwkActiveKeyNumber(&i);
    }
    #endif

    /* Initialize the ZigBee Stack */
    ZigBeeInit();
    currentPrimitive = NO_PRIMITIVE;
    #if defined (USE_USART) && defined(USE_DEBUG)
        printf("\r\nInit Zigbee Stack");
    #endif

    PHYSetShortRAMAddr(0x00, 0x0d);

    NWKClearNeighborTable();
    /* Refresh the tables anew each time the Node is booted up */
    #if defined(I_SUPPORT_BINDINGS)
        ClearBindingTable();
    #endif

    /* Clear the Group Table */
    RemoveAllGroups();
    #if defined(I_SUPPORT_BINDINGS)
        RemoveAllBindings(macPIB.macShortAddress);
    #endif
    // *************************************************************************
    // Perform any other initialization here
    // *************************************************************************
    #if defined (USE_USART) && defined(USE_DEBUG)
        ConsolePutROMString( (ROM char *)"\r\n*************************************" );
        ConsolePutROMString( (ROM char *)"\r\nR&D Coordinator by WSAN-Lab411\r\n" );
        #if (RF_CHIP == MRF24J40)
            ConsolePutROMString( (ROM char *)"Transceiver-MRF24J40\r\n" );
        #else
            ConsolePutROMString( (ROM char *)"Transceiver-Unknown\r\n" );
        #endif
    #endif

    WSANFlags.Val = WSAN_FLAGS_INIT;

    /* Enable interrupts to get the stack operational */
    RFIE = 1;


    while (1)
    {
        /* Clear the watch dog timer */
        CLRWDT();

        /* Process the current ZigBee Primitive */
        ZigBeeTasks( &currentPrimitive );

        /* Determine the next ZigBee Primitive */
        ProcessZigBeePrimitives();

        /* do any non ZigBee related tasks  */
        ProcessNONZigBeeTasks();
    }
}

void ProcessZigBeePrimitives(void)
{
    switch (currentPrimitive)
    {
        case NLME_DIRECT_JOIN_confirm:
            #if defined(USE_USART)
                if (params.NLME_DIRECT_JOIN_confirm.Status == NWK_TABLE_FULL)
                {
                    ConsolePutROMString( (ROM char *)"  Neighbor table is full.\r\n" );
                }
                else
                {
                    ConsolePutROMString( (ROM char *)"  Direct join successful.\r\n" );
                }
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;

        case NLME_NETWORK_FORMATION_confirm:
            if (!params.NLME_NETWORK_FORMATION_confirm.Status)
            {
                #if defined(USE_USART)
                    ConsolePutROMString( (ROM char *)"PAN " );
                    PrintWord(macPIB.macPANId.Val);
                    ConsolePutROMString( (ROM char *)" started successfully.\r\n" );
                #endif
                params.NLME_PERMIT_JOINING_request.PermitDuration = 0xFF;//to permit the joining of the network by new devices
                params.NLME_PERMIT_JOINING_request._updatePayload = TRUE;

                currentPrimitive = NLME_PERMIT_JOINING_request;
            }
            else
            {
                #if defined(USE_USART)
                    PrintChar( params.NLME_NETWORK_FORMATION_confirm.Status );
                    ConsolePutROMString( (ROM char *)" Error forming network. Trying again...\r\n" );
                #endif
                currentPrimitive = NO_PRIMITIVE;
            }
            break;

        case NLME_PERMIT_JOINING_confirm:
            if (!params.NLME_PERMIT_JOINING_confirm.Status)
            {
                #if defined(USE_USART)
                    ConsolePutROMString( (ROM char *)"Joining permitted.\r\n" );
                #endif
                currentPrimitive = NO_PRIMITIVE;

                WSAN_LED = SET;
            }
            else
            {
                #if defined(USE_USART)
                    PrintChar( params.NLME_PERMIT_JOINING_confirm.Status );
                    ConsolePutROMString( (ROM char *)" Join permission unsuccessful. We cannot allow joins.\r\n" );
                #endif
                currentPrimitive = NLME_PERMIT_JOINING_request;
            }
            break;

        case NLME_ROUTE_DISCOVERY_confirm:
            #if defined(USE_USART)
                if (!params.NLME_ROUTE_DISCOVERY_confirm.Status)
                {
                    ConsolePutROMString( (ROM char *)"..Route Reply Successful...\r\n" );
                }
                else
                {
                    PrintChar( params.NLME_PERMIT_JOINING_confirm.Status );
                    ConsolePutROMString( (ROM char *)"..Route Reply not Successful..\r\n" );
                }
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;


        case NLME_JOIN_indication:
            #if defined(USE_USART)
                ConsolePutROMString( (ROM char *)"\r\nNode " );
                PrintWord(params.NLME_JOIN_indication.ShortAddress.Val);
                ConsolePutROMString( (ROM char *)" With MAC Address " );
                //@dat_a3cbq91: Print MAC address of node has just joined
                for(i = 7; i ^ 0xFF; --i)
                {
                    PrintChar(params.NLME_JOIN_indication.ExtendedAddress.v[i]);
                }
                ConsolePutROMString( (ROM char *)" just joined.\r\n" );
            #endif

            /* For Zigbee 2006: If a new device with the same old longAddress address
             * joins the PAN, then make sure the old short address is no longer used and is
             * overwritten with the new shortAddress & longAddress combination
             */
            {  /* same long address check block */
                APS_ADDRESS_MAP currentAPSAddress1;
                currentAPSAddress1.shortAddr   =   params.NLME_JOIN_indication.ShortAddress;
                currentAPSAddress1.longAddr    =   params.NLME_JOIN_indication.ExtendedAddress;

                if(LookupAPSAddress(&params.NLME_JOIN_indication.ExtendedAddress) )
                {
                    for( i = 0; i ^ apscMaxAddrMapEntries; i++)
                    {
                        #ifdef USE_EXTERNAL_NVM
                            GetAPSAddress( &currentAPSAddress,  apsAddressMap + i * sizeof(APS_ADDRESS_MAP) );
                        #else
                            GetAPSAddress( &currentAPSAddress,  &apsAddressMap[i] );
                        #endif
                            if (!memcmp( (void *)&currentAPSAddress.longAddr, (void *)&currentAPSAddress1.longAddr, 8 ))
                            {
                                /* overwrite the old with the new short/long address combo  */
                                #ifdef USE_EXTERNAL_NVM
                                    PutAPSAddress( apsAddressMap + i * sizeof(APS_ADDRESS_MAP), &currentAPSAddress1);
                                #else
                                    PutAPSAddress( &apsAddressMap[i], &currentAPSAddress1 );
                                #endif
                            }
                    }   /* end for loop */
                }   /* end if */
            }   /* end address check block */

            #ifdef I_SUPPORT_SECURITY
                #ifdef I_AM_TRUST_CENTER
                {
                    BOOL AllowJoin = TRUE;
                    // decide if you allow this device to join
                    if( !AllowJoin )
                    {
                        // no need to set deviceAddress, since it is overlap with NLME_JOIN_indication
                        //params.NLME_LEAVE_request.DeviceAddress = params.NLME_JOIN_indication.ExtendedAddress;
                        params.NLME_LEAVE_request.RemoveChildren = TRUE;
                        currentPrimitive = NLME_LEAVE_request;
                        break;
                    }

                    #ifdef I_SUPPORT_SECURITY_SPEC
                        if( params.NLME_JOIN_indication.secureJoin )
                        {
                            BYTE i;
                            for(i = 0; i < 16; i++)
                            {
                                KeyVal.v[i] = 0;
                            }
                            params.APSME_TRANSPORT_KEY_request.Key = &KeyVal;
                            params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.KeySeqNumber = 0;

                        }
                        else
                        {
                            BYTE i;
                            GetNwkActiveKeyNumber(&i);
                            #ifdef USE_EXTERNAL_NVM
                            currentNetworkKeyInfo = plainSecurityKey[i-1];
                            #else
                                GetNwkKeyInfo(&currentNetworkKeyInfo, (ROM void *)&(NetworkKeyInfo[i-1]));
                            #endif
                            params.APSME_TRANSPORT_KEY_request.Key = &(currentNetworkKeyInfo.NetKey);
                            params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.KeySeqNumber = currentNetworkKeyInfo.SeqNumber.v[0];
                        }
                    #else
                        #ifdef PRECONFIGURE_KEY
                        {
                            BYTE i;
                            for(i = 0; i < 16; i++)
                            {
                                KeyVal.v[i] = 0;
                            }
                            params.APSME_TRANSPORT_KEY_request.Key = &KeyVal;
                            params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.KeySeqNumber = 0;
                            params.APSME_TRANSPORT_KEY_request._UseSecurity = TRUE;
                        }
                        #else
                            if( params.NLME_JOIN_indication.secureJoin )
                            {
                                BYTE i;
                                for(i = 0; i < 16; i++)
                                {
                                    KeyVal.v[i] = 0;
                                }
                                params.APSME_TRANSPORT_KEY_request.Key = &KeyVal;
                                params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.KeySeqNumber = 0;
                                params.APSME_TRANSPORT_KEY_request._UseSecurity = TRUE;
                            }
                            else
                            {
                                BYTE i;
                                GetNwkActiveKeyNumber(&i);
                                #ifdef USE_EXTERNAL_NVM
                                    currentNetworkKeyInfo = plainSecurityKey[i-1];
                                #else
                                    GetNwkKeyInfo(&currentNetworkKeyInfo, (ROM void *)&(networkKeyInfo[i-1]));
                                #endif
                                params.APSME_TRANSPORT_KEY_request.Key = &(currentNetworkKeyInfo.NetKey);
                                params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.KeySeqNumber = currentNetworkKeyInfo.SeqNumber.v[0];
                                params.APSME_TRANSPORT_KEY_request._UseSecurity = FALSE;
                            }
                        #endif
                    #endif
                    params.APSME_TRANSPORT_KEY_request.KeyType = ID_NetworkKey;
                    params.APSME_TRANSPORT_KEY_request.DestinationAddress = params.NLME_JOIN_indication.ExtendedAddress;
                    params.APSME_TRANSPORT_KEY_request.TransportKeyData.NetworkKey.UseParent = FALSE;
                    currentPrimitive = APSME_TRANSPORT_KEY_request;
                }
                #else
                    #ifdef I_SUPPORT_SECURITY_SPEC
                        params.APSME_UPDATE_DEVICE_request.Status = (params.NLME_JOIN_indication.secureJoin ) ? 0x00 : 0x01;
                    #else
                        #ifdef PRECONFIGURE_KEY
                            params.APSME_UPDATE_DEVICE_request.Status = 0x00;
                        #else
                            params.APSME_UPDATE_DEVICE_request.Status = 0x01;
                        #endif
                    #endif
                    params.APSME_UPDATE_DEVICE_request.DeviceShortAddress = params.NLME_JOIN_indication.ShortAddress;
                    params.APSME_UPDATE_DEVICE_request.DeviceAddress = params.NLME_JOIN_indication.ExtendedAddress;
                    GetTrustCenterAddress(&params.APSME_UPDATE_DEVICE_request.DestAddress);
                    for(i=0; i < 8; i++)
                    params.APSME_UPDATE_DEVICE_request.DestAddress.v[i] = 0xaa;

                    currentPrimitive = APSME_UPDATE_DEVICE_request;
                #endif
            #else
                currentPrimitive = NO_PRIMITIVE;
            #endif
            break;

        case NLME_LEAVE_indication:
            {
                LONG_ADDR myLongAddr;

                GetMACAddress(&myLongAddr);
                if(!memcmppgm2ram( &params.NLME_LEAVE_indication.DeviceAddress, &myLongAddr, 8 ))
                {
                    #if defined(USE_USART)
                        ConsolePutROMString( (ROM char *)"Node has left the network.\r\n" );
                    #endif
                }
                else
                {
                    #if defined(USE_USART)
                        ConsolePutROMString( (ROM char *)" Another node has left the network.\r\n" );
                    #endif
                }
            }
            #ifdef I_SUPPORT_SECURITY
            {
                SHORT_ADDR		LeavingChildShortAddress;
                if( !APSFromLongToShort(&params.NLME_LEAVE_indication.DeviceAddress) )
                {
                    currentPrimitive = NO_PRIMITIVE;
                    break;
                }
                LeavingChildShortAddress = currentAPSAddress.shortAddr;

                #ifdef I_AM_TRUST_CENTER
                    params.APSME_UPDATE_DEVICE_indication.Status = 0x02;
                    params.APSME_UPDATE_DEVICE_indication.DeviceAddress = params.NLME_LEAVE_indication.DeviceAddress;
                    GetMACAddress(&params.APSME_UPDATE_DEVICE_indication.SrcAddress);
                    params.APSME_UPDATE_DEVICE_indication.DeviceShortAddress = LeavingChildShortAddress;
                    currentPrimitive = APSME_UPDATE_DEVICE_indication;
                    break;
                #else
                    params.APSME_UPDATE_DEVICE_request.Status = 0x02;
                    GetTrustCenterAddress(&params.APSME_UPDATE_DEVICE_request.DestAddress);
                    params.APSME_UPDATE_DEVICE_request.DeviceShortAddress = LeavingChildShortAddress;
                    currentPrimitive = APSME_UPDATE_DEVICE_request;
                    break;
                #endif
            }

            #else
                currentPrimitive = NO_PRIMITIVE;
            #endif
            break;
            
        case NLME_RESET_confirm:
            #if defined(USE_USART)
                ConsolePutROMString( (ROM char *)"ZigBee Stack has been reset.\r\n" );
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;

        case NLME_LEAVE_confirm:
            #if defined(USE_USART)
                PrintChar(params.NLME_LEAVE_confirm.Status);
                ConsolePutROMString( (ROM char *)" Device has left the network\r\n" );
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;

        case APSDE_DATA_confirm:
            #if defined(USE_USART)
                if (params.APSDE_DATA_confirm.Status)
                {
                    ConsolePutROMString( (ROM char *)"Error " );
                    PrintChar( params.APSDE_DATA_confirm.Status );
                    ConsolePutROMString( (ROM char *)" sending message.\r\n" );
                }
                else
                {
                    ConsolePutROMString( (ROM char *)" Message sent successfully.\r\n" );
                }
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;

        case APSDE_DATA_indication:
            {
                BYTE        data;
                BYTE        dataLength = 0;
                BYTE        transByte = 0;

                currentPrimitive = NO_PRIMITIVE;

                switch (params.APSDE_DATA_indication.DstEndpoint)
                {
                    /* Process anything sent to ZDO */
                    case EP_ZDO:
                        {
                            dataLength = APLGet();//lay do dai du lieu @dat_a3cbq91
                            switch( params.APSDE_DATA_indication.ClusterId.Val )
                            {

                                // ********************************************************
                                // Put a case here to handle each ZDO response that we requested.
                                // ********************************************************

                                case BIND_rsp:
                                case UNBIND_rsp:
                                    data = APLGet();
                                    #if defined(USE_USART)
                                        if (data == SUCCESS)
                                        {
                                            ConsolePutROMString( (ROM char *)"Binding/Unbinding done successfully.\r\n" );
                                        }
                                        else
                                        {
                                            PrintChar( data );
                                            ConsolePutROMString( (ROM char *)" Error doing binding/unbinding...\r\n" );
                                        }
                                    #endif
                                    break;

                                case NWK_ADDR_rsp:
                                    if (APLGet() == SUCCESS)
                                    {
                                        #if defined(USE_USART)
                                            /* update our table when device recieves the address it requested */
                                            ConsolePutROMString( (ROM char *)"Receiving NWK_ADDR_rsp.\r\n" );
                                        #endif
                                        // Skip over the IEEE address of the responder.
                                        for (data=0; data ^ 8; data++)
                                        {
                                            currentAPSAddress.longAddr.v[data] = APLGet();
                                            transByte++;
                                        }
                                        currentAPSAddress.shortAddr.v[0] = destinationAddress.byte.LSB = APLGet();
                                        currentAPSAddress.shortAddr.v[1] = destinationAddress.byte.MSB = APLGet();
                                        transByte += 2;
                                        APSSaveAPSAddress(&currentAPSAddress);
                                    }
                                    break;

                                /* Process any further ZDO responses here */
                                default:
                                    break;
                            }

                            // Read out the rest of the MSG in case there is another transaction.
                            for (; transByte<dataLength; transByte++)
                            {
                                APLGet();
                            }
                        }
                        break;

                    // ************************************************************************
                    // Place a case for each user defined endpoint.
                    // ************************************************************************

                    default:
                   /* In this project, every endpoint except Endpoint EP_ZDO is processed here */
                    {
                        switch (params.APSDE_DATA_indication.ClusterId.Val)
                        {
                            //user clusterID application
                            #if defined(USE_CONTROL_PUMP)
                            case ACTOR_REQUEST_CLUSTER:
                            {
                                //cau truc ban tin yeu cau bat bom tu router-emboard
                                //------------------------------------------------------------------------------------
                                // 0bSVVVVVVV
                                // S = 0 --> tat, S = 1 --> bat
                                // VVVVVVV --> van nao duoc bat/tat, dac biet, neu VVVVVVV = 0b1111111 thi tat ca ca van duoc bat/tat
                                //------------------------------------------------------------------------------------
                                data = APLGet();// lay lenh bat/tat bom tuoi
                                //ConsolePut(data);//lenh nay dung that khi bat dieu khien may bom.
                                PrintChar(data);//lenh nay dung de hien thi tren bo nhung xem cho vui
                                #if defined(USE_USART) && defined(USE_DEBUG)
                                    printf("\r\nJust send cmd to board");
                                #endif

                                SendOneByte(data,ACTOR_RESPONSE_CLUSTER);//bao ve Gateway da ra lenh dieu khien bat/tat van tuoi

                                #if defined(USE_USART) && defined(USE_DEBUG)
                                    printf("\r\nConfirm to Gateway");
                                #endif
                            }
                                break;
                            #endif

                            #if defined(USE_SHT10)
                            case HTE_REQUEST_CLUSTER://gui du lieu nhiet do do am ma emboard yeu cau
                            {
                                //ban tin yeu cau lay du lieu nhiet do - do am tu bo nhung co dang: 0x00
                                data = APLGet();
                                LoadSHT10();//lay du lieu nhiet do - do am
                                Send_HTE_ToRouterEmboard();
                                #if defined(USE_USART) && defined(USE_DEBUG)
                                    printf("\r\nJust send data to Router_EMB");
                                #endif
                            }
                                break;
                            #endif


                            default:
                                /* Catch all place for all none ZDO msgs not processed above */
                                #if defined(USE_USART)
                                    printf("\r\nGot message...");
                                #endif
                                break;
                        }
                        if (currentPrimitive ^ APSDE_DATA_request)
                            TxData = TX_DATA_START;//reset lai chi so TxData
                    }
                        break;
                }
                APLDiscardRx();
            }
            break;

        case APSME_ADD_GROUP_confirm:
        case APSME_REMOVE_GROUP_confirm:
        case APSME_REMOVE_ALL_GROUPS_confirm:
            #if defined(USE_USART) || defined(ROUTER_EMB)
                printf("\r\nPerform Group Operation" );
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;

        /* if nothing to process first check to see if we are in startup seqence */
        case NO_PRIMITIVE:
            if (!ZigBeeStatus.flags.bits.bNetworkFormed)
            {
                if (!ZigBeeStatus.flags.bits.bTryingToFormNetwork)
                {
                    #if defined(USE_USART)
                        ConsolePutROMString( (ROM char *)"Trying to start network...\r\n" );
                    #endif
                    params.NLME_NETWORK_FORMATION_request.ScanDuration          = 8;
                    params.NLME_NETWORK_FORMATION_request.ScanChannels.Val      = ALLOWED_CHANNELS;
                    params.NLME_NETWORK_FORMATION_request.PANId.Val             = 0x1AAA;
                    params.NLME_NETWORK_FORMATION_request.BeaconOrder           = MAC_PIB_macBeaconOrder;
                    params.NLME_NETWORK_FORMATION_request.SuperframeOrder       = MAC_PIB_macSuperframeOrder;
                    params.NLME_NETWORK_FORMATION_request.BatteryLifeExtension  = MAC_PIB_macBattLifeExt;
                    currentPrimitive = NLME_NETWORK_FORMATION_request;
                    break;
                }
            }
            else
            {
                if(!ZigBeeStatus.flags.bits.bHasBackgroundTasks)
                {
                    // ************************************************************************
                    // Place all processes that can send messages here.  Be sure to call
                    // ZigBeeBlockTx() when currentPrimitive is set to APSDE_DATA_request.
                    // ************************************************************************
                    #if defined(USE_MQ6)
                        //gui thong tin co khoi ve router-emboard
                        if(ZigBeeReady() && WSANFlags.bits.MQ6Warning)
                        {
                            WSANFlags.bits.MQ6Warning = CLEAR;
                            SendOneByte(HasSmoke,STATE_NODE_CLUSTER);//thong bao co khoi ve router-emboard
                        }
                    #endif

                    #if defined(ENERGY_TRACKING)
                        //gui thong tin sap het nang luong ve router-emboard
                        if(ZigBeeReady() && WSANFlags.bits.LowPowerWarning)
                        {
                            WSANFlags.bits.LowPowerWarning = CLEAR;
                            SendOneByte(LowPower,STATE_NODE_CLUSTER);
                        }
                    #endif

                    #if defined(USE_SHT10)
                        //gui thong tin nhiet do-do am ve router-emboard
                        if(ZigBeeReady() && WSANFlags.bits.HTE_Data_Ready)
                        {

                            WSANFlags.bits.HTE_Data_Ready = CLEAR;
                            Send_HTE_ToRouterEmboard();
                        }
                    #endif
                }
            }
            break;

        default:
            #if defined(USE_USART) || defined(ROUTER_EMB)
                PrintChar( currentPrimitive );
                ConsolePutROMString( (ROM char *)" Unhandled primitive.\r\n" );
            #endif
            currentPrimitive = NO_PRIMITIVE;
            break;
    }

}

void ProcessNONZigBeeTasks(void)
{
    // *********************************************************************
    // Place any non-ZigBee related processing here.  Be sure that the code
    // will loop back and execute ZigBeeTasks() in a timely manner.
    // *********************************************************************
    #if defined(USE_SHT10)
        //Lay du lieu nhiet do-do am
        if(WSANFlags.bits.EnableGetDataHTE)
        {
            WSANFlags.bits.EnableGetDataHTE = CLEAR;
            LoadSHT10();

            //chu y: neu muon doi ra do C --> xem muc 4.3 datasheet SHT1x
            //xu ly lay du lieu do am tuong doi --> xem muc 4.1 & 4.2 datasheet SHT1x
            #if defined(USE_USART) && defined(USE_DEBUG)
                printf("\r\nTemperature (hex): ");
                PrintWord(temperature);

                printf("\r\nHumidity (hex): ");
                PrintWord(humidity);
            #endif

            WSANFlags.bits.HTE_Data_Ready = SET;//cho phep gui du lieu dinh ki ve gateway
        }

    #endif

    #if defined(ENERGY_TRACKING) && defined(USE_MQ6)
    if (WSANFlags.bits.MQ6orVoltage == Finish_Convert)
    {
        #if defined(USE_USART) && defined(USE_DEBUG)
            /* Hien thi ket qua tu cam bien khoi */
            printf("\r\nMQ6 signal (hex): ");
            PrintWord(Mq6Signal);

            /* Hien thi ket qua do dien ap nguon */
            printf("\r\nVoltage (hex): ");
            PrintWord(Energy_Level);
        #endif

        if (Mq6Signal < ThresholdMQ6)
        {
            WSANFlags.bits.MQ6Warning = SET;//kich hoat viec gui tin hieu ve router emboard
        }

        if (Energy_Level > ThresholdPower)
        {
            WSANFlags.bits.LowPowerWarning = SET;//kich hoat viec gui tin hieu ve router emboard
        }
        WSANFlags.bits.MQ6orVoltage = MQ6_Mode_ADC;
    }
    #endif

    #if defined(ENERGY_TRACKING) && !defined(USE_MQ6)
    if(WSANFlags.bits.CompleteADC)
    {
        #if defined(USE_USART) && defined(USE_DEBUG)
            printf("\r\nVoltage (hex): ");
            PrintWord(Energy_Level);
        #endif
        if (Energy_Level > ThresholdPower)
        {
            WSANFlags.bits.LowPowerWarning = SET;
        }
        WSANFlags.bits.CompleteADC = CLEAR;
    }
    #endif

    #if !defined(ENERGY_TRACKING) && defined(USE_MQ6)
    if(WSANFlags.bits.CompleteADC)
    {
        #if defined(USE_USART) && defined(USE_DEBUG)
            printf("\r\nMQ6 signal (hex): ");
            PrintWord(Mq6Signal);
        #endif
        if (Mq6Signal<ThresholdMQ6)
        {
            WSANFlags.bits.MQ6Warning = SET;
        }
        WSANFlags.bits.CompleteADC = CLEAR;
    }
    #endif
}

/*******************************************************************************
HardwareInit

All port directioning and SPI must be initialized before calling ZigBeeInit().

For demonstration purposes, required signals are configured individually.
*******************************************************************************/
void HardwareInit(void)
{
    //Digital pins @ added by dat_a3cbq91
    ANSE = 0x0000;
    ANSG = 0x0000;
    ANSD = 0x0000;

    #if(CLOCK_FREQ < 1000000)
        //SPI1 for RF transceiver
        SPI1CON1 = 0b0000000100111111;      // CLOCK_FREQ as SPI CLOCK
        SPI1STAT = 0x8000;

        //SPI2 for EEPROM
        SPI2CON1 = 0b0000000100111111;      // CLOCK_FREQ as SPI CLOCK
        SPI2STAT = 0x8000;
    #else
        //comment by dat_a3cbq91
        /* SPI1 for RF transceiver */
        SPI1CON1 = 0b0000000100111110;  //PIC24FJ128GA306 is Master, MRF24J40 is Slaver
                                        //Internal SPI clock is enabled
                                        //SDO1 pin is controlled by the module
                                        //Communication is byte-wide (8 bits)
                                        //Input data is sampled at the middle of data output time
                                        //Serial output data changes on transition from active clock state to Idle clock state
                                        //~SS1 pin is not used by the module; pin is controlled by the port function
                                        //Idle state for the clock is a low level; active state is a high level
                                        //Secondary prescale 1:1
                                        //Primary prescale 4:1 -> CLOCK_FREQ/2/4 as SPI CLOCK

        //Enables module and configures SCKx, SDOx, SDIx and ~SSx as serial port pins
        //Interrupt when the last data in the receive buffer is read; as a result, the buffer is empty (SRXMPT bit is set)
        SPI1STAT = 0x8000;

        /* SPI2 for EEPROM */
        //same as configuration SPI1 module
        SPI2CON1 = 0b0000000100111110;
        SPI2STAT = 0x8000;
    #endif

    #ifdef USE_EXTERNAL_NVM
    	EEPROM_nCS	= 1;
    	EEPROM_nCS_TRIS	= 0;
    	IFS2bits.SPI2IF = 1;
    #endif

    PHY_RESETn = 0;
    PHY_RESETn_TRIS = 0;
    PHY_CS = 1;
    PHY_CS_TRIS = 0;

    RFIF = 0;
    RFIE = 1;

    if(RF_INT_PIN == 0)
    {
        RFIF = 1;
    }
    
    //configure pin which is attached by led is output
    WSAN_LED_TRIS = CLEAR;
    //Clear Led to a initialize state
    WSAN_LED = 0;

    //--------------------------------------------------------------------------
    // This section is required for application-specific hardware initialization
    //--------------------------------------------------------------------------
    #if defined(ENERGY_TRACKING) || defined(USE_MQ6)
        //input analog at AN8
        REF_VOLTAGE_TRIS = SET;
        REF_ANALOG = ENABLE;

        //Enable supply voltage to Zener Diode
        TRISBbits.TRISB10 = 0;
        LATBbits.LATB10 = 1;

        //configure ADC module
        AD1CON1 = 0x0470;//12-bit A/D operation
                        //Output format bit is absolute decimal result, unsigned, right-justified
                        //Internal counter ends sampling and starts conversion
                        //Sampling begins when SAMP bit is manually set
        #if defined(USE_MQ6)
            AD1CON2 = 0x4800;   //external positive voltage reference --> for measure signal from smoke sensor
                                //Conversion result is loaded into the buffer ADC1BUF8
                                //Interrupt at the completion of conversion for each sample
                                //Always uses channel input selects for Sample A
        #else
            AD1CON2 = 0x0800;//VDD is voltage reference --> for measure voltage lever of power
        #endif
        AD1CON3 = 0x1F02;//Sample Time = 31Tad, Tad = 3Tcy
        AD1CHS = 0x0008;//channel AN8

        AD1CON1bits.ADON = 1;
        //ADC interrupt
        IFS0bits.AD1IF = 0;
        IEC0bits.AD1IE = 1;
        IPC3bits.AD1IP = 6;
    #endif

    // ----------------- Init SHT -----------------
    #if defined(USE_SHT10)
        // Make RG2, RG3 outputs.
        SCK_TRIS    =   CLEAR;
        DATA_TRIS   =   CLEAR;
    #endif
}

/*******************************************************************************
User Interrupt Handler

The stack uses some interrupts for its internal processing. Once it is done
checking for its interrupts, the stack calls this function to allow for any
additional interrupt processing.
*******************************************************************************/
//this routine is written by dat_a3cbq91
void _ISR __attribute__((interrupt, auto_psv)) _T3Interrupt(void)
{
    DWORD timer = CYCLE_HTE_SENDING;
    IFS0bits.T3IF = 0;

    #if defined(USE_SHT10)
        WSANFlags.bits.EnableGetDataHTE = SET;//enable get temperature and humidity from SHT10
    #endif
    #if defined(ENERGY_TRACKING) || defined(USE_MQ6)
        AD1CON1bits.SAMP = ON;//start sampling for conversion
    #endif

    //re-assigned for PR3:PR2
    PR3 = timer >> 16;
    PR2 = timer & 0xFFFF;

    //if you don't want print neighbor of this node, comment the following
    //WSANFlags.bits.PrintNeighborTable = SET;
}
//this routine is written by dat_a3cbq91
void _ISR __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = CLEAR;
    //Lay gia tri chuyen doi ADC
    #if defined(USE_MQ6) && defined(ENERGY_TRACKING)
        //lay ket qua chuyen doi ADC
        ADC_result = ADC1BUF8;
        switch(WSANFlags.bits.MQ6orVoltage)
        {
            case MQ6_Mode_ADC:
                Mq6Signal = ADC_result;
                AD1CON2bits.PVCFG = Measure_Voltage;//chuyen sang do dien ap cua nguon
                break;

            case Power_Mode_ADC:
                Energy_Level = ADC_result;
                AD1CON2bits.PVCFG = Measure_MQ6;//chuyen sang do tin hieu tu MQ6
                break;

            default:
                break;
        }
        ++(WSANFlags.bits.MQ6orVoltage);
    #endif

    #if defined(USE_MQ6) && !defined(ENERGY_TRACKING)
        Mq6Signal = ADC1BUF8;
        WSANFlags.bits.CompleteADC = SET;
    #endif

    #if defined(ENERGY_TRACKING) && !defined(USE_MQ6)
        Energy_Level = ADC1BUF8;
        WSANFlags.bits.CompleteADC = SET;
    #endif
}
void _ISR __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = CLEAR;
	
}



/*********************************************************************
 * Function:        void SendOneByte(BYTE data, WORD ClusterID)
 *
 * PreCondition:    Init OK
 *
 * Input:           state
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        Gui mot thong tin ve router-EMB. Thong tin nay co the la cac
 *                  trang thai chay rung, canh bao muc nang luong hoac la thong
 *                  xac nhan co mot node mang nao do vua gia nhap mang
 *
 * Note:
 ********************************************************************/
void SendOneByte(BYTE data, WORD ClusterID)
{
    //cau truc ban tin: SS
    TxBuffer[TxData++] = data;

    /* load parameters for APSDE_DATA_request primitive */
    params.APSDE_DATA_request.DstAddrMode = APS_ADDRESS_16_BIT;

    /* load network address of router-emboard */
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[1] = RouterEmboardAddrMSB;
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[0] = RouterEmboardAddrLSB;

    params.APSDE_DATA_request.SrcEndpoint = WSAN_Endpoint;
    params.APSDE_DATA_request.DstEndpoint = WSAN_Endpoint;
    params.APSDE_DATA_request.ProfileId.Val = MY_PROFILE_ID;

    //params.APSDE_DATA_request.asduLength; TxData
    params.APSDE_DATA_request.RadiusCounter = DEFAULT_RADIUS;

//    params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_FORCE;
//    params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_SUPPRESS;
    params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_ENABLE;

    #ifdef I_SUPPORT_SECURITY
        params.APSDE_DATA_request.TxOptions.Val = Enable;
    #else
        params.APSDE_DATA_request.TxOptions.Val = DISABLE;
    #endif
    params.APSDE_DATA_request.TxOptions.bits.acknowledged = ENABLE;
    params.APSDE_DATA_request.ClusterId.Val = ClusterID;

    ZigBeeBlockTx();
    currentPrimitive = APSDE_DATA_request;
}

#if defined(USE_SHT10)
/*********************************************************************
 * Function:        void Send_HTE_ToRouterEmboard()
 *
 * PreCondition:
 *
 * Input:           clusterID
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        Gui du lieu nhiet do-do am-nang luong (HTE) ve
 *                  router-EMB.
 *
 * Note:
 ********************************************************************/
void Send_HTE_ToRouterEmboard(void)
{
    //nap du lieu nhiet do - do am - nang luong chua qua xu ly vao buffer TX
    TxBuffer[TxData++] = (BYTE) (temperature >> 8);
    TxBuffer[TxData++] = (BYTE) (temperature);
    TxBuffer[TxData++] = (BYTE) (humidity >> 8);
    TxBuffer[TxData++] = (BYTE) (humidity);
    #if defined(ENERGY_TRACKING)
        TxBuffer[TxData++] = (BYTE) (Energy_Level >> 8);
        TxBuffer[TxData++] = (BYTE) (Energy_Level);
    #else
        TxBuffer[TxData++] = 0x00;
        TxBuffer[TxData++] = 0x00;
    #endif

    params.APSDE_DATA_request.DstAddrMode = APS_ADDRESS_16_BIT;
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[1] = RouterEmboardAddrMSB;
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[0] = RouterEmboardAddrLSB;

    params.APSDE_DATA_request.SrcEndpoint = WSAN_Endpoint;
    params.APSDE_DATA_request.DstEndpoint = WSAN_Endpoint;
    params.APSDE_DATA_request.ProfileId.Val = MY_PROFILE_ID;//MY_PROFILE_ID = 0x0103

    //params.APSDE_DATA_request.asduLength; TxData
    params.APSDE_DATA_request.RadiusCounter = DEFAULT_RADIUS;//gioi han so hop ma du lieu duoc phep truyen qua, o day la 10 hop

    //params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_FORCE;
    //params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_SUPPRESS;
    params.APSDE_DATA_request.DiscoverRoute = ROUTE_DISCOVERY_ENABLE;

    #ifdef I_SUPPORT_SECURITY
        params.APSDE_DATA_request.TxOptions.Val = Enable;
    #else
        params.APSDE_DATA_request.TxOptions.Val = DISABLE;//khong ho tro bao mat
    #endif
    params.APSDE_DATA_request.TxOptions.bits.acknowledged = ENABLE;// Yeu cau ACK tu thiet bi thu
    params.APSDE_DATA_request.ClusterId.Val = HTE_RESPONSE_CLUSTER;
    ZigBeeBlockTx();

    currentPrimitive = APSDE_DATA_request;
}

void LoadSHT10(void)
{
    BYTE error, checksum;

    error = 0;
    error += s_measure((BYTE*) &humidity, &checksum, HUMI); //measure humidity
    error += s_measure((BYTE*) &temperature, &checksum, TEMP); //measure temperature
    while (error != 0)
    {
        s_connectionreset(); //in case of an error: connection reset
        error += s_measure((BYTE*) &humidity, &checksum, HUMI); //measure humidity
        error += s_measure((BYTE*) &temperature, &checksum, TEMP); //measure temperature
    }
}
#endif


