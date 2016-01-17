#include <main_mode2.h>

#use rs232(UART,baud=19200,xmit=PIN_C6,rcv=PIN_C7,parity=N,bits=8)

unsigned char rx_data;
void uart_putchar(unsigned char udata)
{
  while(!TRMT);
  TXREG = udata;
}

#include "ds1307.c"
#include "AT24LC01.c"
#include "lcdfixed.c"
#include "run_mode1.c"
#include "time_control.c"
#include "seg_dislay.c"
void switch_menu();
// chuong trinh ngat timer 1
#int_TIMER1
void  TIMER1_isr(void) 
{
   // gui ki tu 'r' di de lay gia tri nhiet do va do am
   //uart_putchar('r');
}
// chuong trinh ngat timer 0
#int_TIMER0
void  TIMER0_isr(void) 
{
   if(menu_system==0 || menu_system==3)
      update_time();
   // hien thi led led 7 thanh
   dislay_seg(hour,minute);
   if(!led) led=1;
   else     led=0;
}
// chuong trinh ngat uart
#INT_RDA
void Receive_isr() 
{
   if(mode_run==1)
      dieukhien_van();
   //if(mode_run==2){}
   if(mode_run==3)
   {
      unsigned char rx_buff[3],rx_count;
      rx_buff[rx_count++] = RCREG;
      if(rx_count>1)
      {
         temp=rx_buff[0];
         huff=rx_buff[1];
         rx_count=0;
      }
   }
}

void main()
{
   //T1CONL=0x30;
   //T1CONH=0xA0;
   
   setup_timer_0 (RTCC_INTERNAL|RTCC_DIV_16);
   set_timer0(0);
   led_io=0;
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_io_dieukhien();
   Speaker_io=0;
   setup_comparator(NC_NC_NC_NC);// This device COMP currently not supported by the PICWizard
   lcd_init();
   delay_ms(10);
   init_DS1307();
   delay_ms(10);
   init_ext_eeprom();
   delay_ms(10);
   LED_LCD_IO=0;
   LED_LCD=1;
   Init_595();
   menu_system=0;
   load_eeprom();
   off_all_van();
   while(true)
   {
      switch_menu();
   }
}

void switch_menu()
{
   if(!MENU_BTN)
   {
      // bat loa len tao xung o chan loa
      Speaker = 1;
      delay_ms(150);
      Speaker = 0;
      delay_ms(150); 
      state_settime=1;
      lcd_gotoxy(1,4);
      printf(lcd_putc,"                    ");
      menu_system++;
      if(menu_system==5)   menu_system=0;
   }
   switch(menu_system)
   {
      // chay chuong trinh chinh
      case 0:
      {
         // chay chuong trinh chinh
         if(mode_run ==1)
         {
            // hien thi cac gia tri len lcd
            // hien thi che do mode len dong 1
            lcd_gotoxy(1,1);
           // update_time();
            // hien thi led led 7 thanh
            
            printf(lcd_putc,"  CHE DO : MODE 1   ");
            // hien thi thoi gian + date
            put_time_info();
            put_date_info();
            delay_ms(100);
            // chay chuong trinh tu uart dieu khien may bom
            
         }
         if(mode_run==2)
         {
            update_time();
            lcd_gotoxy(1,1);
            printf(lcd_putc,"  CHE DO : MODE 2   ");
            // hien thi thoi gian + date
            put_time_info();
            put_date_info();
            lcd_gotoxy(1,4);
            printf(lcd_putc,"                    ");
            delay_ms(100);
            run_mode2();
         }
         if(mode_run==3)
         {
            // hien thi gia tri nhiet do va do am lcd
            //run_mode3();
         }
      }
      break;
      // chay chuong trinh setup time
      case 1:
      {
         set_time();
      }
      break;
      // chay chuong trinh setup date
      case 2:
      {
         set_date();
      }
      break;
      // chay chuong trinh setup time
      case 3:
      {
         set_mode();
      }
      break;
      // neu dang o mode 2 setup thoi gian on off
      case 4:
      {
         //
         if(mode_run==2)
            set_time_on_off();
         //if(mode_run==3);
            //set_temp_huff();
         if(mode_run==1)
            menu_system=0;        
      }
      break;
      // neu dang o mode 3 setup nhiet do on off
      default:
      break;
   }
}
