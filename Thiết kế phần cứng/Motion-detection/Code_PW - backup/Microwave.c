//---------------------------------------------------------
// date created:07/11/2012
// Update:
// 8/11/2012: chuyen code CW sang PW
//
//
//
//
//
//
//
//
//
//
//
//
//
// author : Nguyen Hung
// mcu:PIC16f688
// clock:8Mhz
// descrip:Use with microwave_boad
//----------------------------------------------------------

#include <Microwave.h>
//#use rs232(baud=19200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

//.........Defined........................................
//---------------------------------------------------------
#define MAX_PULSE_WIDTH 12500    //200Hz
#define MIN_PULSE_WIDTH 1250     //200Hz
#define FILTER_VALUE    9
#define PULSE_ACTIVE    20     // active pulse = 32.7 x PULSE_ACTIVE
#define PULSE_OFF       40     // off pulse = 32.7 x PULSE_OFF - active pulse


#BYTE PORTA = 0x05
#BIT RA0 = 0x05.0
#BIT RA1 = 0x05.1
#BIT RA2 = 0x05.2

#BYTE PORTC = 0x07
#BIT RC0 = 0x07.0
#BIT RC1 = 0x07.1
#BIT RC2 = 0x07.2
#BIT RC3 = 0x07.3
#BIT RC4 = 0x07.4
#BIT RC5 = 0x07.5

#BYTE TRISA = 0x85
#BIT TA0 = 0x85.0
#BIT TA1 = 0x85.1
#BIT TA2 = 0x85.2

#BYTE TRISC = 0x87
#BIT TC0 = 0x87.0
#BIT TC1 = 0x87.1
#BIT TC2 = 0x87.2
#BIT TC3 = 0x87.3
#BIT TC4 = 0x87.4
#BIT TC5 = 0x87.5

//........variable.........................................

volatile unsigned int16 timer0_var =0;
volatile char pulse_count;
//volatile unsigned int16 pulse_buff;


//----------------------------------------------------------
//...........macro.........................................
//
void enable_timer1(void);
void disable_timer1(void);

void IDLE_MODE(void);
void OFF_MODE(void);
void ACTIVE_MODE(void);
void wait(unsigned int16 k);

//
//----------------------------------------------------------
//---------interrupt handle funtion-------------------------
#int_EXT
void  EXT_isr(void) 
{
unsigned int16 pulse_buff =0;
pulse_buff = get_timer1();
//printf("\r\n %lu \t\t %d \r\n",pulse_buff,pulse_count);
if(pulse_count == 0)
   {
   enable_timer1();
//   enable_interrupts(INT_TIMER1);
   pulse_count++;
   }
else
   {
   if(pulse_buff < MIN_PULSE_WIDTH)
      {
       if(pulse_count>FILTER_VALUE) 
         {
         printf("a");
         }
 //    printf("\r\n %lu \t\t %d \r\n",pulse_buff,pulse_count);
      IDLE_MODE();
      }
   else 
      {
      pulse_count++ ;
      }
   }
}
#INT_TIMER0
void TIMER0_handle(void)
{
   timer0_var++;
   if(timer0_var == PULSE_ACTIVE) 
   {
   //process of off mode 
   OFF_MODE();
   }
   else if(timer0_var == PULSE_OFF)
   {
   //process of active mode
   timer0_var = 0;
   ACTIVE_MODE();
   }
}

//!#INT_TIMER1
//!void TIMER1_handle(void)
//!{
//!if(pulse_count>FILTER_VALUE) 
//!   {
//!   printf("W");
//!   IDLE_MODE();
//!   }
//!}


#int_RDA
void  RDA_isr(void) 
{
   int8 value;
   value = getc();
   putc('R');
}


void main()
{
   setup_comparator(NC_NC_NC_NC);// This device COMP currently not supported by the PICWizard
  // enable_interrupts(INT_EXT);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256); //32.7 ms overflow
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RDA);
   //enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
  
 //  setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);//128ms overflow

   ext_int_edge( H_TO_L );   // Sets up EXT
   setup_timer_1 (T1_DISABLED );
   printf("NguyenHung \n \r DT5 - K53 - DHBKHN \n \r");
   printf("Ready to use !! \n\r ");
   TC3 = 0;
   RC3 = 1;

while(true)
   {
   if(get_timer1() > MAX_PULSE_WIDTH)
      {
         if(pulse_count>FILTER_VALUE) 
         {
            printf("W");
            IDLE_MODE();
         }
      }
    }
}

//-------------------------------------------------------------
void enable_timer1(void)
{
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //262 ms overflow
   set_timer1(0);                               //this sets timer1 register to 0
}

void disable_timer1(void)
{
   set_timer1(0); 
   setup_timer_1 ( T1_DISABLED );
}
void IDLE_MODE(void)
{
   disable_interrupts(INT_TIMER1);
//   disable_timer1();
   set_timer1(0); 
   setup_timer_1 ( T1_DISABLED );
   pulse_count =0;
  // pulse_buff =0;
}
void OFF_MODE(void)
{
   disable_timer1 ();
   disable_interrupts(INT_TIMER1);
   disable_interrupts(INT_EXT);
   
   set_timer1(0);
   pulse_count =0;

   RC3 =0 ;
   }
void ACTIVE_MODE(void)
{
   disable_interrupts(INT_TIMER0);
   RC3 =1 ;
   wait(0x100);// so luyen: 01686269854
//   wait(0xffff);
   set_timer0(0);
   enable_interrupts(INT_TIMER0);
   set_timer1(0);
   enable_interrupts(INT_EXT);
   ext_int_edge( H_TO_L );   // higt to low
   pulse_count =0;
   }
void wait(unsigned int16 k)
{
while(k--);
}
