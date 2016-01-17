#include "General.h"
#include "rs485.h"
#include "SHT1x.h"

//******************************************************************************
// Configuration Bits
//******************************************************************************
#pragma romdata CONFIG1H = 0x300001
const rom BYTE config1H = 0b00000001;// XT oscillator mode

#pragma romdata CONFIG2L = 0x300002
const rom BYTE config2L = 0b00011110;// Brown-out Reset Enabled in hardware @ 1.8V, PWRTEN enabled

#pragma romdata CONFIG2H = 0x300003
const rom BYTE config2H = 0b00010010;// WDT 1:512 postscale, WDT is controlled by SWDTEN bit

#pragma romdata CONFIG3H = 0x300005
const rom BYTE config3H = 0b10000100;// PORTB digital on RESET, MCLR pin enabled
                                     // The system lock is held off until the HFINTOSC is stable

#pragma romdata CONFIG4L = 0x300006
const rom BYTE config4L = 0b10000001;// Stack full will cause reset, LVP off
                                     // Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins
#pragma romdata

//******************************************************************************
// Variables declare
//******************************************************************************
//All variables in this application is global because we don't want system to spend time to allocate memory.
WORD humidity = 0, temperature = 0;//store result that get from SHT sensor
MESSAGE msg;//store data (humi & temp) to send to UART (RS485)
volatile BYTE command;//get character from UART Tx
volatile BYTE GetHumiTempFlag = 0;//This flag enable/disable get data from SHT

//******************************************************************************
// Functions declare
//******************************************************************************
void UARTRx_ISR(void);//uart rx interrupt service routine
void InitSystem(void);//initialize necessary modules
void LoadSHT10(void);//Get data from SHT


void main(void)
{
    //Initialize all modules
    InitSystem();
    #ifdef USE_DEBUG
        printf("Hello\r\n");
    #endif

    //main loop
    while(1)
    {
        //neu co lenh gui toi (nhan tu bo dem UART)
        if(GetHumiTempFlag)
        {
            //xoa co bao trang thai
            GetHumiTempFlag = 0;
            

            //lay du lieu nhiet do do am.
            LoadSHT10();

            //load data to send uart
            msg.data.Humidity = humidity;
            msg.data.Temperature = temperature;

            #ifdef USE_DEBUG
                PrintDWord(msg.msg);
                printf("\r\n");
                PrintWord(msg.data.Temperature);
                PrintWord(msg.data.Humidity);
                printf("\r\n");
                PrintChar(msg.v[3]);
                PrintChar(msg.v[2]);
                PrintChar(msg.v[1]);
                PrintChar(msg.v[0]);
                printf("\r\n");
            #else
                RS485_DRIVE = Transmit;
                ConsolePut(msg.v[3]);
                ConsolePut(msg.v[2]);
                ConsolePut(msg.v[1]);
                ConsolePut(msg.v[0]);
                RS485_DRIVE = Receive;
            #endif
        }
    }
}

//xu ly ngat nhan uart
#pragma code highVector=0x08
void HighVector (void)
{
    _asm goto UARTRx_ISR _endasm
}
#pragma code /* return to default code section */

#pragma interrupt UARTRx_ISR
void UARTRx_ISR(void)
{
    if(PIR1bits.RCIF)
    {
        PIR1bits.RCIF = 0;
        command = RCREG;
        if(command == 'r')
        {
            GetHumiTempFlag = 1;
        }
    }
}

void InitSystem(void)
{
    //Initialize oscillator module
    OSCCONbits.SCS = 0b00; //Primary clock (determined by CONFIG1H[FOSC<3:0>])

    //khoi tao cac chan giao tiep voi SHT.
    DATA_TRIS = 0;
    SCK_TRIS = 0;

    //khoi tao chan dieu khien MAX485
    RS485_DRIVE_TRIS = 0;
    RS485_DRIVE= Receive;

    //khoi tao module truyen thong bat dong bo UART
    ConsoleInit();

    //khoi tao ngat cho UART
    RCONbits.IPEN = 1;//bat ngat uu tien
    IPR1bits.RCIP = 1;//ngat Rx UART muc cao
    PIE1bits.RCIE = 1;//
    PIR1bits.RCIF = 0;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

void LoadSHT10()
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
