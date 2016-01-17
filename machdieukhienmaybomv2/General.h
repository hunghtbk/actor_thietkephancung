//include header file
#include <p18f26k20.h>

typedef unsigned char BYTE;                 // 8-bit
typedef unsigned short int WORD;            // 16-bit
typedef unsigned long DWORD;                // 32-bit

//format of message
typedef union _MESSAGE
{
    DWORD msg;

    struct
    {
        WORD Humidity;//DWORD LSB
        WORD Temperature;//DWORD MSB
    } data;
    BYTE v[4];
} MESSAGE;

#define ROM  rom

/*
***********************************************************************************************
*PORT DEFINITION
***********************************************************************************************
*/

#define DATA_WR		LATCbits.LATC4
#define	DATA_RD   	PORTCbits.RC4
#define	SCK   		LATCbits.LATC3
#define DATA_TRIS	TRISCbits.TRISC4
#define SCK_TRIS	TRISCbits.TRISC3
#define RS485_DRIVE_TRIS TRISCbits.TRISC5
#define RS485_DRIVE    LATCbits.LATC5

#define Receive 0
#define Transmit 1
//#define USE_DEBUG
