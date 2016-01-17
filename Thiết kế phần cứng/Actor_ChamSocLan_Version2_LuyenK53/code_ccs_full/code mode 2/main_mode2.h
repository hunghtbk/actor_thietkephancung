#include <18F46K20.h>
#device adc=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES HS                       //Low power osc < 200 khz
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(clock=4000000)
#locate TRISA =   0x0F92
#locate TRISB =   0x0F93
#locate TRISC =   0x0F94
#locate TRISD =   0x0F95
#locate TRISE =   0x0F96
#locate LATA  =   0x0F89
#locate LATB  =   0x0F8A
#locate LATC  =   0x0F8B
#locate LATD  =   0x0F8C
#locate LATE  =   0x0F8D
#locate PORTA =   0x0F80
#locate PORTB =   0x0F81
#locate PORTC =   0x0F82
#locate PORTD =   0x0F83
#locate PORTE =   0x0F84
#locate TXSTA =   0x0FAC
#locate TXREG =   0x0FAD
#locate RCREG =   0x0FAE
// khai bao cac nut bam
#bit TRMT  =   TXSTA.1
#bit SET_BTN  =   PORTB.0
#bit UP_BTN   =   PORTB.2
#bit DOWN_BTN =   PORTB.1
#bit MENU_BTN =   PORTB.3
//Nut an chon che do manual-auto
//auto :  tu dong dieu khien bom theo
//manual: do nguoi dung tu setup va dieu khien. 
#bit  Speaker  = LATC.0             // 
#bit  Speaker_io  = TRISC.0         // 
#bit  LED_LCD  = LATA.4             // 
#bit  LED_LCD_IO  = TRISA.4         // 
//define cac van dien tu
#bit     VAN1    =  LATC.5       // dieu khien relay1
#bit     VAN2    =  LATC.2       // dieu khien relay2
#bit     VAN3    =  LATD.4       // dieu khien relay3
#bit     VAN4    =  LATD.5       // dieu khien relay4
#bit     VAN5    =  LATD.6       // dieu khien relay5
#bit     VAN6    =  LATD.7       // dieu khien relay6
#bit     MAYBOM  =  LATD.3       // dieu khien may bom
#bit     VAN1_IO    =  TRISC.5
#bit     VAN2_IO    =  TRISC.2
#bit     VAN3_IO    =  TRISD.4
#bit     VAN4_IO    =  TRISD.5
#bit     VAN5_IO    =  TRISD.6
#bit     VAN6_IO    =  TRISD.7
#bit     MAYBOM_IO  =  TRISD.3  


