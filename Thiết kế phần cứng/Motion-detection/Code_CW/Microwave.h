#include <16F1936.h>
#device *=16
#device adc=8
/*
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOPROTECT                //Code not protected from reading
#FUSES MCLR                     //Master Clear pin enabled
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#FUSES PLL_SW                   //4X HW PLL disabled, 4X PLL enabled/disabled in software
#FUSES BORV27                   //Brownout reset at 2.7V
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOBROWNOUT
*/
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOPROTECT                //Code not protected from reading
#FUSES MCLR                     //Master Clear pin enabled
#FUSES NOCPD                    //No EE protection
#FUSES NOBROWNOUT               //No brownout reset
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES WDT_SW                
#FUSES NOCLKOUT              
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOVCAP                
#FUSES PLL_SW                   
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES BORV19                
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG                  //No Debug mode for ICD
//#use delay(clock=20000000)

#use delay(clock=8000000)

#use rs232(baud=19200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)


