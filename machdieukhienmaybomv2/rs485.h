#include "General.h"

#define CLOCK_FREQ 4000000
#define BAUD_RATE 19200

// If using USART with low speed, uncomment the following
#define USART_USE_BRGH_HIGH
#define ConsoleIsPutReady()     (TXSTAbits.TRMT)

#if defined(USART_USE_BRGH_LOW)
    #define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/64) - 1)
#else
    #define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/16) - 1)
#endif

#if SPBRG_VAL > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

void ConsoleInit(void);

void ConsolePut(BYTE c);
void ConsolePutROMString(ROM char* str);
void PrintChar(BYTE);
void PrintWord(WORD);
void PrintDWord(DWORD toPrint);
#define printf(x)	ConsolePutROMString((ROM char *)x)
