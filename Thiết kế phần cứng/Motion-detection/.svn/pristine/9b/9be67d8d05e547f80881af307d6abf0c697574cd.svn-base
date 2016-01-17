/*******************************************************************************
 * Microchip ZigBee2006 Residential Stack
 *
 * Zigbee Router
 *
 * Day la ma nguon de cau hinh cho mot node tro thanh Router theo chuan giao thuc
 * Zigbee. Ma nguon chay tren phan cung duoc thiet ke boi nhom WSAN - lab411.
 * Trong ma nguon cua nhom, co su dung kien truc Microchip Stack de xay dung cac
 * ung dung theo chuan giao tiep khong day Zigbee. De hieu duoc hoat dong cua he
 * thong, hay doc tai lieu WSAN Specification
 *
 *******************************************************************************
 * FileName:        Router.c
 * Project:         DemoPIC24Router
 * Version:         2.0
 *
 * Controller:      dsPIC33FJ128MC804
 * Editor:          MPLAB X IDE v1.41
 * Complier:        C30 v3.30 or higher
 *
 * Company support: Microchip Technology, Inc.
 *
 * Developer:       Nguyen Tien Dat - KSTN - DTVT - K54
 * Group:           WSAN group - Lab411
 * Edition:         13/10/2012
 *
 *******************************************************************************/

//******************************************************************************
// Header Files
//******************************************************************************

// Include the main ZigBee header file.
#include "zAPL.h"

#ifdef I_SUPPORT_SECURITY
    #include "zSecurity.h"
#endif

// If you are going to send data to a emboard, include this file.
#include "console.h"

//******************************************************************************
// Configuration Bits
//******************************************************************************

//Internal Fast RC (FRC)
_FOSCSEL(FNOSC_FRC);

//Clock switching is enabled, Fail-Safe Clock Monitor is disabled
//OSC2 pin has clock out function
//XT Oscillator Mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);

//Watchdog timer enabled/disabled by user software
_FWDT(FWDTEN_OFF & WDTPOST_PS512)
	
//******************************************************************************
// Constants						//defined application service	@dat_a3cbq91
//******************************************************************************
#define ON              1
#define SET             1
#define ENABLE          1
#define OFF             0
#define CLEAR           0
#define DISABLE         0

#define NetworkAddrMSB_EMB  0x00
#define NetworkAddrLSB_EMB  0x0A

//******************************************************************************
// Function Prototypes
//******************************************************************************
// Function is modified by WSAN - lab411
void ProcessZigBeePrimitives(void);
// End by WSAN - lab411

// Function create by WSAN - lab411
BYTE GetMACByte( void );
BYTE GetHexDigit( void );
BYTE ZigBeeReceiverDone = 1;
void HardwareInit( void );
void ProcessNONZigBeeTasks(void);
void SendOneByte(WORD ClusterID, BYTE MSB, BYTE LSB, BYTE cmd);
void ProcessCmd(void);
// End by WSAN - Lab411

//******************************************************************************
// Application Variables
//******************************************************************************

NETWORK_DESCRIPTOR  *currentNetworkDescriptor;
ZIGBEE_PRIMITIVE    currentPrimitive;
NETWORK_DESCRIPTOR  *NetworkDescriptor;
BYTE                orphanTries;
#ifdef I_SUPPORT_SECURITY
    extern KEY_VAL	KeyVal;
    #ifdef USE_EXTERNAL_NVM
        extern WORD trustCenterLongAddr;
        extern NETWORK_KEY_INFO plainSecurityKey[2];
    #else
        extern ROM LONG_ADDR trustCenterLongAddr;
    #endif
#endif

BYTE AllowJoin = ENABLE;
BYTE RejectByte;
WORD_VAL MSGPacketCount;

extern NWK_STATUS nwkStatus;

// Variable declared by WSAN - Lab411
char    i;
volatile char    EnableSendCmd = DISABLE;
volatile BYTE Addr_MSB,Addr_LSB,Cmd,signal,counter = CLEAR;

// End by WSAN - lab411

//******************************************************************************
//******************************************************************************
//******************************************************************************
int main(void)
{
    /* Configure by WSAN-lab411 */
    //PLL setup:
    CLKDIVbits.PLLPRE = 0; // N1=2: PLL VCO Output Divider Select bits; 0 -> /2 (default)
    PLLFBDbits.PLLDIV = 6; // M=40: PLL Feedback Divisor bits; 30 -> 30 x multiplier
    // (divisor is 2 more than the value)
    CLKDIVbits.PLLPOST = 0; // N2=2: PLL Phase Detector Input Divider bits; 0 -> /2

    /*
     * Fosc = Fin*(M/(N1+N2))
     * N1 = PLLPRE + 2
     * N2 = 2 x (PLLPOST + 1)
     * M = PLLDIV + 2
     * >> Fosc = 8M*(32/4) = 64MHz >> Fcy = 64/2 = 32MIPS
     */
    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03); // NOSC !!
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011); // Internal OSC :000
    // XT: 010
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);
    OSCTUN = 0; // Tune FRC oscillator, if FRC is used
    RCONbits.SWDTEN = DISABLE; // Disable Watch Dog Timer
    // Peripheral Pin Select
    asm volatile (  "mov #OSCCONL, w1  \n"
                    "mov #0x46, w2     \n"
                    "mov #0x57, w3     \n"
                    "mov.b w2, [w1]    \n"
                    "mov.b w3, [w1]    \n"
                    "bclr OSCCON, #6");

    // INT1 >>> RP18------------------------------------------------------------
    RPINR0bits.INT1R = 0b10010;
    // ------SPI1---------------------------------------------------------------
    // SCK1 >> RP15 >> RB15//voi PIC24 thay bang RP10>>RF4
    RPINR20bits.SCK1R = 0b01111;
    RPOR7bits.RP15R = 0b01000;
    // SDO1 >> RP16 >> RC0
    RPOR8bits.RP16R = 0b00111;
    // SDI1 >> RP17 >> RC1
    RPINR20bits.SDI1R = 0b10001;
    //--------SPI2--------------------------------------------------------------
    //SCK2 >> RP24
    RPINR22bits.SCK2R = 0b11000;
    RPOR12bits.RP24R = 0b01011;
    //SDO2 >> RP21
    RPOR10bits.RP21R = 0b01010;
    //SDI2 >> RP20
    RPINR22bits.SDI2R = 0b10100;
    //--------UART1-------------------------------------------------------------
    // RX1 >> RP22
    RPINR18bits.U1RXR = 22;
    // TX1 >> RP9
    RPOR4bits.RP9R = 3;
    //--------UART2-------------------------------------------------------------
    // RX2 >> RP19
    RPINR19bits.U2RXR = 19;
    //TX2 >> RP25
    RPOR12bits.RP25R = 5;

    asm volatile (  "mov #OSCCONL, w1  \n"
                    "mov #0x46, w2     \n"
                    "mov #0x57, w3     \n"
                    "mov.b w2, [w1]    \n"
                    "mov.b w3, [w1]    \n"
                    "bset OSCCON, #6");
    //--------------------------------------------------------------------------
    /* End by WSAN-lab411 */
    
    CLRWDT();
    ENABLE_WDT();

    NetworkDescriptor = NULL;
    orphanTries = 3;

    // If you are going to send data to a terminal, initialize the UART.
    ConsoleInit();
	UART1Init();
    // Initialize the hardware - must be done before initializing ZigBee.
    HardwareInit();
	UART1PutROMString((ROM char*)"UART1init \r\n");
 //   printf("Init Hardware\r\n");

    // Initialize the ZigBee Stack.
    ZigBeeInit();
 //   printf("Init Zigbee\r\n");

    // *************************************************************************
    // Perform any other initialization here
    // *************************************************************************

  //  printf("R&D ZigBee-Router EMB by WSAN-Lab411\r\n");
    #if (RF_CHIP == MRF24J40)
   //     printf("Transceiver-MRF24J40\r\n");
    #else
   //     printf("Transceiver-Unknown\r\n" );
    #endif

    // Enable interrupts to get everything going.
    RFIE = ENABLE;

    /* Start the network anew each time Node is booted up */
    NWKClearNeighborTable();
    #if defined(I_SUPPORT_BINDING)
        ClearBindingTable();
    #endif
    
    /* Clear the Group Table */
    RemoveAllGroups();
    
    #if defined(I_SUPPORT_BINDING)
        RemoveAllBindings(macPIB.macShortAddress);
    #endif
    
    /* Clearing nwk status flags */
    nwkStatus.flags.Val = 0x00;

    /* Initialize for primitive */
    currentPrimitive = NO_PRIMITIVE;
    
    while (1)
    {
        CLRWDT();
        /* Determine which is the next ZigBee Primitive to operate on */
        ZigBeeTasks( &currentPrimitive );

        /* Process the next ZigBee Primitive */
        ProcessZigBeePrimitives();

        /* do any non ZigBee related tasks and then go back to ZigBee tasks */
        ProcessNONZigBeeTasks();  
    }
}     

/* Prototype: void ProcessZigBeePrimitive(void)
 * Input: None
 * Output: None
 * Discribe: Gui ket qua xu ly primitive hien tai len lop ung dung.
 *           Thiet lap primitive moi cho vong lap ke tiep.
 * Note: NO_PRIMITIVE va APSDE_DATA_indication la 2 primitive dung de gui va
 *       nhan ban tin chuan Zigbee.
 */
void ProcessZigBeePrimitives(void)
{
    switch (currentPrimitive)
    {
        case NLME_ROUTE_DISCOVERY_confirm:
            if (!params.NLME_ROUTE_DISCOVERY_confirm.Status)
            {
                printf("Route Reply OK\r\n" );
            }
            else
            {
                printf("Route Reply Failed:\r\n" );
                PrintChar( params.NLME_PERMIT_JOINING_confirm.Status );
            }
            currentPrimitive = NO_PRIMITIVE;
            break;

        case NLME_NETWORK_DISCOVERY_confirm:
            currentPrimitive = NO_PRIMITIVE;
            if (!params.NLME_NETWORK_DISCOVERY_confirm.Status)
            {
                if (!params.NLME_NETWORK_DISCOVERY_confirm.NetworkCount)
                {
                    printf("No networks found. Trying again!\r\n" );
                }
                else
                {
                    // Save the descriptor list pointer so we can destroy it later.
                    NetworkDescriptor = params.NLME_NETWORK_DISCOVERY_confirm.NetworkDescriptor;

                    // Select a network to try to join.  We're not going to be picky right now...
                    currentNetworkDescriptor = NetworkDescriptor;

SubmitJoinRequest:
                    params.NLME_JOIN_request.PANId          = currentNetworkDescriptor->PanID;
                    params.NLME_JOIN_request.JoinAsRouter   = TRUE;
                    params.NLME_JOIN_request.RejoinNetwork  = FALSE;
                    params.NLME_JOIN_request.PowerSource    = MAINS_POWERED;
                    params.NLME_JOIN_request.RxOnWhenIdle   = TRUE;
                    params.NLME_JOIN_request.MACSecurity    = FALSE;
                    params.NLME_JOIN_request.ExtendedPANID = currentNetworkDescriptor->ExtendedPANID;
                    
                    currentPrimitive = NLME_JOIN_request;

                    printf("Network(s) found. Trying to join\r\n " );
                    PrintChar( params.NLME_JOIN_request.PANId.byte.MSB );
                    PrintChar( params.NLME_JOIN_request.PANId.byte.LSB );
                    printf(" | ");
                    for(i=7;i>=0;i--)
                        PrintChar(currentNetworkDescriptor->ExtendedPANID.v[i]);
                }
            }
            else
            {
                printf("Error finding network:\r\n " );
                PrintChar( params.NLME_NETWORK_DISCOVERY_confirm.Status );
                printf("Trying again!\r\n");
            }
            break;

        case NLME_JOIN_confirm:
            if (!params.NLME_JOIN_confirm.Status)
            {
                printf("Join OK!\r\n" );

                // Free the network descriptor list, if it exists. If we joined as an orphan, it will be NULL.
                while (NetworkDescriptor)
                {
                    currentNetworkDescriptor = NetworkDescriptor->next;
                    nfree( NetworkDescriptor );
                    NetworkDescriptor = currentNetworkDescriptor;
                }

                // Start routing capability.
                params.NLME_START_ROUTER_request.BeaconOrder = MAC_PIB_macBeaconOrder;
                params.NLME_START_ROUTER_request.SuperframeOrder = MAC_PIB_macSuperframeOrder;
                params.NLME_START_ROUTER_request.BatteryLifeExtension = FALSE;
                currentPrimitive = NLME_START_ROUTER_request;
            }
            else
            {
                currentPrimitive = NO_PRIMITIVE;
                printf("Status:\r\n ");
                PrintChar( params.NLME_JOIN_confirm.Status );

                // If we were trying as an orphan, see if we have some more orphan attempts.
                if (ZigBeeStatus.flags.bits.bTryOrphanJoin)
                {
                    // If we tried to join as an orphan, we do not have NetworkDescriptor, so we do
                    // not have to free it.
                    printf(". Could not join as orphan. " );
                    orphanTries--;
                    if (orphanTries == 0)
                    {
                        printf("Must try as new node!" );
                        ZigBeeStatus.flags.bits.bTryOrphanJoin = CLEAR;
                    }
                    else
                    {
                        printf("Trying again!" );
                    }
                }
                else
                {
                    printf(". Could not join selected network. " );
                    currentNetworkDescriptor = currentNetworkDescriptor->next;
                    if (currentNetworkDescriptor)
                    {
                        printf("Trying next discovered network!" );
                        goto SubmitJoinRequest;
                    }
                    else
                    {
                        //Ran out of descriptors.  Free the network descriptor list, and fall
                        // through to try discovery again.
                        printf("Cleaning up and retrying discovery!" );
                        while (NetworkDescriptor)
                        {
                            currentNetworkDescriptor = NetworkDescriptor->next;
                            nfree( NetworkDescriptor );
                            NetworkDescriptor = currentNetworkDescriptor;
                        }
                    }
                }
            }
            break;

        case NLME_START_ROUTER_confirm:
            if (!params.NLME_START_ROUTER_confirm.Status)
            {
                printf("Router Started! Enabling joins...\r\n" );
                params.NLME_PERMIT_JOINING_request.PermitDuration = 0xFF;   // No Timeout
                params.NLME_PERMIT_JOINING_request._updatePayload = TRUE;
                currentPrimitive = NLME_PERMIT_JOINING_request;
            }
            else
            {
                printf("Router start Failed:\r\n" );
                PrintChar( params.NLME_JOIN_confirm.Status );
                printf(". We cannot route frames" );
                currentPrimitive = NO_PRIMITIVE;
            }
            break;

        case NLME_PERMIT_JOINING_confirm:
            if (!params.NLME_PERMIT_JOINING_confirm.Status)
            {
                // Thiet lap lai dia chi mang cua Router_EMB @dat_a3cbq91
                macPIB.macShortAddress.v[1] = NetworkAddrMSB_EMB;
                macPIB.macShortAddress.v[0] = NetworkAddrLSB_EMB;
                // Set ShortAddr on Transceiver !
                PHYSetShortRAMAddr(0x03, macPIB.macShortAddress.v[0]);
                PHYSetShortRAMAddr(0x04, macPIB.macShortAddress.v[1]);
                printf("My Network Address is 0x000A now!\r\n");
                currentPrimitive = NO_PRIMITIVE;
            }
            else
            {
                printf("Join permission Failed:\r\n " );
                PrintChar( params.NLME_PERMIT_JOINING_confirm.Status );
                printf(". We cannot allow joins" );
                currentPrimitive = NO_PRIMITIVE;
            }
            break;

        case NLME_JOIN_indication:
            /* For Zigbee 2006: If a new device with the same old longAddress address
            * joins the PAN, then make sure the old short address is no longer used and is
            * overwritten with the new shortAddress & longAddress combination
            */
            /* Neu mot thiet bi da tung la thanh vien cua mang, can phai dam bao
             * la dia chi mang khong duoc dung nua va cap (dia chi mang & dia chi MAC)
             * se duoc ghi lai trong bang @dat_a3cbq91
             */
            {  /* same long address check block */
                APS_ADDRESS_MAP currentAPSAddress1;
                currentAPSAddress1.shortAddr   =   params.NLME_JOIN_indication.ShortAddress;
                currentAPSAddress1.longAddr    =   params.NLME_JOIN_indication.ExtendedAddress;

                if(LookupAPSAddress(&params.NLME_JOIN_indication.ExtendedAddress) )
                {

                    for( i = 0; i < apscMaxAddrMapEntries; i++)
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
                }       /* end if */
            }           /* end address check block */
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
                                    GetNwkKeyInfo(&currentNetworkKeyInfo, (ROM void *)&(NetworkKeyInfo[i-1]));
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
                    printf("We have left the network\r\n" );
                }
                else
                {
                    printf("Another node has left the network\r\n" );
                }
            }
            #ifdef I_SUPPORT_SECURITY
            {
                SHORT_ADDR	LeavingChildShortAddress;
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
            printf("ZigBee Stack has been reset\r\n" );
            /* For Zigbee 2006 The Specs require that node needs to
             * try joining as an orphan first after each reset,
             * see Mandatory test 3.9
            */
            ZigBeeStatus.flags.bits.bTryOrphanJoin = SET;

            currentPrimitive = NO_PRIMITIVE;
            break;

        case NLME_LEAVE_confirm:
            printf("Leaving the Zigbee network: \r\n" );
            PrintChar(params.NLME_LEAVE_confirm.Status);

            currentPrimitive = NO_PRIMITIVE;
            break;

        case APSDE_DATA_indication:
            {
                currentPrimitive = NO_PRIMITIVE;

                switch (params.APSDE_DATA_indication.DstEndpoint)
                {
                    // ************************************************************************
                    // Place a case for each user defined endpoint.
                    // ************************************************************************
                    case WSAN_Dst:
                    {
                        switch(params.APSDE_DATA_indication.ClusterId.Val)
                        {
                            case STATE_NODE_CLUSTER:
                                printf("#");
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.MSB);
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.LSB);
                                PrintChar(APLGet());//SS
								printf("\r\n");
 							break;
                            case JOIN_CONFIRM_CLUSTER:
                                printf("#JN:");
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.MSB);
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.LSB);
                                PrintChar(APLGet()); //MM
								printf("\r\n");
                            break;
                            case HTE_RESPONSE_CLUSTER:
                            {
                                printf("#DF:");
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.MSB);
                                PrintChar(params.APSDE_DATA_indication.SrcAddress.ShortAddr.byte.LSB);
                                PrintChar(APLGet());//nhiet do
                                PrintChar(APLGet());
                                PrintChar(APLGet());//do am
                                PrintChar(APLGet());
                                PrintChar(APLGet());//dien ap
								PrintChar(APLGet());
								printf("\r\n");	
								ZigBeeReceiverDone = 1;
                            }
                            break;

                            /* Place other Cluster.ID cases here */
                            case ACTOR_RESPONSE_CLUSTER:
                            {
                                printf("#OK:");
                                PrintChar(APLGet());
								printf("\r\n");
                            }
                            break;
                            default:
                                break;
                        }   /* switch 1*/

                        if( currentPrimitive != APSDE_DATA_request )
                            TxData = TX_DATA_START;
                    }           /* if msg */
                        break;

                    default:
                        break;
                }
            APLDiscardRx();
//			currentPrimitive = NO_PRIMITIVE;
            }
            break;

        case APSDE_DATA_confirm:
            if (params.APSDE_DATA_confirm.Status)
            {
                printf("Error sending message: ");
                PrintChar(params.APSDE_DATA_confirm.Status);
				printf("\r\n");
            }
            else
            {
                printf("Sending message OK!\r\n" );
            }
            currentPrimitive = NO_PRIMITIVE;
            break;

        case APSME_ADD_GROUP_confirm:
        case APSME_REMOVE_GROUP_confirm:
        case APSME_REMOVE_ALL_GROUPS_confirm:
            printf("Perform Group Operation\r\n" );
            currentPrimitive = NO_PRIMITIVE;
            break;

        case NO_PRIMITIVE:
            if (AllowJoin &&!ZigBeeStatus.flags.bits.bNetworkJoined)
            {
                if (!ZigBeeStatus.flags.bits.bTryingToJoinNetwork)
                {
                    if (ZigBeeStatus.flags.bits.bTryOrphanJoin)
                    {
                        printf("Trying to join network as an orphan...\r\n" );

                        params.NLME_JOIN_request.ScanDuration     = 8;
                        params.NLME_JOIN_request.ScanChannels.Val = ALLOWED_CHANNELS;
                        params.NLME_JOIN_request.JoinAsRouter     = TRUE;
                        params.NLME_JOIN_request.RejoinNetwork    = 0x01;
                        params.NLME_JOIN_request.PowerSource      = MAINS_POWERED;
                        params.NLME_JOIN_request.RxOnWhenIdle     = TRUE;
                        params.NLME_JOIN_request.MACSecurity      = FALSE;

                        params.NLME_JOIN_request.ExtendedPANID = currentNetworkDescriptor->ExtendedPANID;
                        currentPrimitive = NLME_JOIN_request;

                        /* For Zigbee 2006 09/17/07, we now need to do an orphan join after a reset, but
                         * we choose not to do this forever, and quit after a few of retries
                        */
                        orphanTries--;
                        if (orphanTries == 0)
                        {
                            orphanTries = 3;
                            printf("Must now try as a new node...\r\n" );
                            ZigBeeStatus.flags.bits.bTryOrphanJoin = 0;
                        }
                    }
                    else
                    {
                        printf("Trying to join network as a new device!\r\n" );
                        params.NLME_NETWORK_DISCOVERY_request.ScanDuration          = 8;
                        params.NLME_NETWORK_DISCOVERY_request.ScanChannels.Val      = ALLOWED_CHANNELS;
                        currentPrimitive = NLME_NETWORK_DISCOVERY_request;
                    }
                }
            }
            else
            {
                if (ZigBeeReady())
                {
				if((!ZigBeeStatus.flags.bits.bHasBackgroundTasks)&&(ZigBeeReceiverDone))
                    {
      					ZigBeeReceiverDone = 0;
     				//	ProcessCmd();
				    }

                }

                    // ************************************************************************
                    // Place all processes that can send messages here.  Be sure to call
                    // ZigBeeBlockTx() when currentPrimitive is set to APSDE_DATA_request.
                    // ************************************************************************
            }
            break;

        default:
            PrintChar( currentPrimitive );
            printf("Unhandled primitive\r\n" );
            currentPrimitive = NO_PRIMITIVE;
            break;
    }
}

/* Prototype: void ProcessNONZigBeeTasks(void)
 * Input: None
 * Output: None
 * Discribe: xu ly cac cong viec khong lien quan den Zigbee.
 */
void ProcessNONZigBeeTasks(void)
{

        // *********************************************************************
        // Place any non-ZigBee related processing here.  Be sure that the code
        // will loop back and execute ZigBeeTasks() in a timely manner.
        // *********************************************************************

}

/*******************************************************************************
HardwareInit

All port directioning and SPI must be initialized before calling ZigBeeInit().

For demonstration purposes, required signals are configured individually.
*******************************************************************************/
void HardwareInit(void)
{
    //digital pin
    AD1PCFGL = 0x01FF;

    #if(CLOCK_FREQ < 1000000)
        SPI1CON1 = 0x013A;	// 0000 0001 0011 1010
        SPI1CON2bits.FRMEN = 0;
        SPI1STATbits.SPIROV = 0;		// Clear Overflow bit
        SPI1STATbits.SPIEN = 1;		// Enable the peripheral

        SPI2CON1 = 0x013A;	// 0000 0001 0011 1010
        SPI2CON2bits.FRMEN = 0;
        SPI2STATbits.SPIROV = 0;		// Clear Overflow bit
        SPI2STATbits.SPIEN = 1;		// Enable the peripheral

    #else
        SPI1CON1 = 0x013A;	// 0000 0001 0011 1010
        SPI1CON2bits.FRMEN = 0;
        SPI1STATbits.SPIROV = 0;		// Clear Overflow bit
        SPI1STATbits.SPIEN = 1;		// Enable the peripheral

        SPI2CON1 = 0x013A;	// 0000 0001 0011 1010
        SPI2CON2bits.FRMEN = 0;
        SPI2STATbits.SPIROV = 0;		// Clear Overflow bit
        SPI2STATbits.SPIEN = 1;		// Enable the peripheral
    #endif

    #ifdef USE_EXTERNAL_NVM
        EEPROM_nCS = 1;
        EEPROM_nCS_TRIS = 0;
        IFS2bits.SPI2IF = 1;
    #endif

    PHY_RESETn = 0;
    PHY_RESETn_TRIS = 0;
    PHY_CS = 1;
    PHY_CS_TRIS = 0;

    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;

    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA10 = 0;

    RFIF = CLEAR;
    RFIE = ENABLE;

    if (RF_INT_PIN == 0)
    {
        RFIF = SET;
    }

    RF_SPIInit();
    EE_SPIInit();

    CNEN1bits.CN15IE = 1;
    CNEN2bits.CN16IE = 1;
    CNPU1bits.CN15PUE = 1;
    CNPU2bits.CN16PUE = 1;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

    // Initialize the SPI1 pins and directions
    TRISBbits.TRISB15 = 0; // SCK
    TRISCbits.TRISC1 = 1; // SDI
    TRISCbits.TRISC0 = 0; // SDO
    LATBbits.LATB15 = 0; // SCK
    LATCbits.LATC0 = 1; // SDO

    // Initialize the SPI2 pins and directions
    TRISCbits.TRISC8 = 0; // SCK
    TRISCbits.TRISC4 = 1; // SDI
    TRISCbits.TRISC5 = 0; // SDO
    LATCbits.LATC8 = 0; // SCK
    LATCbits.LATC5 = 1; // SDO

    TRISAbits.TRISA10 = 0;
    TRISAbits.TRISA7 = 0;
    // UART1
    TRISBbits.TRISB9 = 0;
    TRISCbits.TRISC6 = 1;
    //UART2
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC9 = 0;
}

/*******************************************************************************
User Interrupt Handler

The stack uses some interrupts for its internal processing. Once it is done
checking for its interrupts, the stack calls this function to allow for any
additional interrupt processing.
*******************************************************************************/

void _ISR __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
	BYTE c;
    IFS1bits.U2RXIF = CLEAR;
	c = U2RXREG;
	UART1Put(c);	
}

void SendOneByte(WORD ClusterID, BYTE MSB, BYTE LSB, BYTE cmd)
{
    TxBuffer[TxData++] = cmd;
    ZigBeeBlockTx();

    /* load parameters for APSDE_DATA_request primitive */
    params.APSDE_DATA_request.DstAddrMode = APS_ADDRESS_16_BIT;

    /* load network address of router-emboard */
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[1] = MSB;
    params.APSDE_DATA_request.DstAddress.ShortAddr.v[0] = LSB;

    params.APSDE_DATA_request.SrcEndpoint = WSAN_Src;
    params.APSDE_DATA_request.DstEndpoint = WSAN_Dst;
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

    currentPrimitive = APSDE_DATA_request;
}
#if defined(USE_COMPUTER)
void ProcessCmd(void)
{
    DISABLE_WDT();//need to avoid reseting MCU automatically.
    /* Get the user's input from the keyboard. */
    Addr_MSB = GetMACByte();
	Addr_LSB = GetMACByte();
	Cmd      = GetMACByte();
    switch (Cmd)
    { 
        case 0:
            	SendOneByte(HTE_DATA_REQUEST_CLUSTER, Addr_MSB,Addr_LSB,Cmd);
				printf(" Da gui yeu cau lay du lieu\r\n");
		break;
		default:
     	    	SendOneByte(ACTOR_REQUEST_CLUSTER, Addr_MSB,Addr_LSB,Cmd);
				printf("Da gui yeu cau dieu khien\r\n");
        break;
    }
 ENABLE_WDT();
}
#else
void ProcessCmd(void)
{
    DISABLE_WDT();//need to avoid reseting MCU automatically.
    /* Get the user's input from the keyboard. */
    Addr_MSB = ConsoleGet();
	Addr_LSB = ConsoleGet();
	Cmd      = ConsoleGet();
//	c = ConsoleGet();
//	ConsolePut(Addr_MSB);
//	ConsolePut(Addr_LSB);
//	ConsolePut(Cmd);
	RejectByte = ConsoleGet();
    switch (Cmd)
    { 
        case 0:
            	SendOneByte(HTE_REQUEST_CLUSTER, Addr_MSB,Addr_LSB,Cmd);
	    break;
		default:
     	    	SendOneByte(ACTOR_REQUEST_CLUSTER, Addr_MSB,Addr_LSB,Cmd);
        break;
    }
 ENABLE_WDT();
}
#endif
BYTE GetHexDigit( void )
{
    BYTE    c;
    while (!ConsoleIsGetReady());
    c = ConsoleGet();
    if (('0' <= c) && (c <= '9'))
        c -= '0';
    else if (('a' <= c) && (c <= 'f'))
        c = c - 'a' + 10;
    else if (('A' <= c) && (c <= 'F'))
        c = c - 'A' + 10;
    else
        c = 0;
    return c;
}

// Ham nay co tac dung lay mot byte dang hexa khi ta nhap 2 ki tu tu ban phim.
// vi du: khi ta muon nhap gia tri 0x12 cho MCU, ta nhan 2 phim '1' va '2', sau
// khi goi ham nay, gia tri nhan duoc la 0x12.
BYTE GetMACByte( void )
{
    BYTE    oneByte;
    //ConsolePutROMString( (ROM char * const) "Enter last MAC byte in hex: \r\n" );
    oneByte = GetHexDigit() << 4;
    oneByte += GetHexDigit();
    //ConsolePutROMString( (ROM char * const) "\r\n" );
    return oneByte;
}
