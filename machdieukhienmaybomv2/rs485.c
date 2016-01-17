#include "rs485.h"

void ConsoleInit(void)
{
    #if defined(USART_USE_BRGH_HIGH)
        /* Transmit 8-bits mode
         * Enable transmit
         * asynchronous mode at high speed
         */
        TXSTA = 0x24;
    #else
         /* 8-bits transmit mode
         * enable transmit
         * asynchronous mode at low speed
         */
        TXSTA = 0x20;
    #endif

    /* Enable serial port
     * Enable receiver
     */
    RCSTA = 0x90;

    SPBRG = SPBRG_VAL;
    BAUDCON = 0x40;//8-bit Baud Rate Generator.
}

void ConsolePut(BYTE c)
{
    while( !ConsoleIsPutReady() );
    TXREG = c;
}

// Truyen mot chuoi ky tu len terminal @dat_a3cbq91
void ConsolePutROMString(ROM char* str)
{
    BYTE c;

    while (c = *str++)
    {
        while (!ConsoleIsPutReady());
        TXREG = c;
    }

    // Since this function is mostly for debug, we'll block here to make sure
    // the last character gets out, in case we have a breakpoint on the
    // statement after the function call.
    while (!ConsoleIsPutReady());
}

ROM unsigned char CharacterArray[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// In mot byte len terminal bang cach truyen 1 byte trong 2 lan, truyen MSB truoc, LSB sau. @dat_a3cbq91
void PrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint >> 4)&0x0F;
    while (!ConsoleIsPutReady());
    TXREG = CharacterArray[toPrint];
    toPrint = (PRINT_VAR)&0x0F;
    while (!ConsoleIsPutReady());
    TXREG = CharacterArray[toPrint];
    return;
}

void PrintWord(WORD toPrint)
{
    BYTE MSB,LSB;
    MSB = toPrint >> 8;
    LSB = toPrint & 0x00FF;
    PrintChar(MSB);
    PrintChar(LSB);
}

void PrintDWord(DWORD toPrint)
{
    BYTE t3,t2,t1,t0;
    t3 = toPrint >> 24;
    t2 = toPrint >> 16;
    t1 = toPrint >> 8;
    t0 = toPrint & 0x000000FF;

    PrintChar(t3);
    PrintChar(t2);
    PrintChar(t1);
    PrintChar(t0);
}

