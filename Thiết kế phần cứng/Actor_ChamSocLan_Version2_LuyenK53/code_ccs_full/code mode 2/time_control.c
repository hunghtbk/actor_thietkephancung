// FILE TIME_CONTROL DUNG DE SETUP CAC THONG SO CUA THOI GIAN
// DA BAO GOM KHAI BAO CAC THONG SO CAN THIET
// include cac ham can thiet vao thu vien
//#include "lcdfixed.c"      // khai bao ham cho lcd
//-------------------------------------------------------------

//bien luu gia tri thoi gian tam thoi
unsigned char   second,minute,hour,day=1,date,month,year;
unsigned char   temp_max, huff_max,temp,huff;         //,huff_max_temp,temp_max_temp;
//bien luu xem dang chinh gio, phut, giay, ngay thang?
unsigned char state_settime=1,set_flag_mode2,STT_time_on;
unsigned char maybom_on=0,count_in=0;
unsigned char mode_run=0,menu_system,mode_run_flag=1;
struct Time_on_off
{
   unsigned char hour_on;
   unsigned char minute_on;
   unsigned char time_on;
   unsigned char state_on;
};

struct Time_on_off DEV_INFO[5];
//time----------------------------------------------------------------------
//bien luu tiet hoc dung cho auto alarm

unsigned long     sum_on_min,sum_on_max,sum_on; 


//-------------------------------------------------------------------------
void put_time(unsigned char time,int x, int y);
void put_date_info();
void put_time_info();
void lcd_blink_on();
void lcd_blink_off();
// ham doc cac gia tri tu ds1307
void update_time()
{
   second =bcd2bin(read_DS1307(0x00));
   minute =bcd2bin(read_DS1307(0x01));
   hour   =bcd2bin(read_DS1307(0x02));
   day    =bcd2bin(read_DS1307(0x03));
   date   =bcd2bin(read_DS1307(0x04));
   month  =bcd2bin(read_DS1307(0x05));
   year   =bcd2bin(read_DS1307(0x06));     
}
//------------------------------------------------------------------------------------
//dung ham put_time de in ra second, minute, hour, date, month, year
// input : gia tri cua bien can in ra
//         vi tri cua bien can in ra tren LCD
// output : none
void put_time(unsigned char time,int x, int y) 
{
    unsigned char time_low,time_high;
    // tach bien time thanh 2 so hien thi tren lcd
    time_high = time/10 + 0x30;
    time_low  = time%10 + 0x30; 
    // di den vi tri x,y hien thi gia tri time
    lcd_gotoxy(x,y);
    lcd_putc(time_high);
    lcd_putc(time_low );
}

//-----------------------------------------------------------------------------------
//void put_data_info : hien thi thong tin thu ngay thang tren lcd
//input: none
//output: none
void put_date_info()
{
    lcd_gotoxy(1,3);
    printf(lcd_putc,"   ");  
    switch(day)
    { 
        // neu la chu nhat 
        case 1:
            printf(lcd_putc,"CN ");
         break;
        // neu la thu 2 
        case 2:
            printf(lcd_putc,"T2 ");
        break;
        //neu la thu 3
        case 3:
            printf(lcd_putc,"T3 ");
        break; 
        // neu la thu 4
        case 4:
            printf(lcd_putc,"T4 ");
        break; 
        // neu la thu 5
        case 5:
            printf(lcd_putc,"T5 ");
        break; 
        // neu la thu 6
        case 6:
            printf(lcd_putc,"T6 ");
        break; 
        // neu la thu 7
        case 7:
            printf(lcd_putc,"T7 ");
        break; 
        // neu khong co truong hop nao thi out
        default: 
        break;
    }
    put_time(date,7,3);
    lcd_putc('-');
    put_time(month,10,3);
    lcd_putc('-');
    lcd_gotoxy(13,3);   
    printf(lcd_putc,"20      "); 
    put_time(year,15,3);
}

//-----------------------------------------------------------------------------------
//void put_data_info : hien thi thong tin thu ngay thang tren lcd
//input: gia tri gio phut giay
//output: none
void put_time_info()
{
    lcd_gotoxy(1,2);
    printf(lcd_putc,"      ");
    put_time(hour,7,2); 
    lcd_putc(':');
    put_time(minute,10,2);
    lcd_putc(':');
    put_time(second,13,2);
    lcd_gotoxy(15,2);
    printf(lcd_putc,"      ");
}

//------------------------------------------------------------------------------------   
// hien thi trang thai nhap nhay tren lcd
void lcd_blink_on()
{
    lcd_putc(' ');  //B0D 1101
    lcd_putc(' ');  //B0D 1101
    delay_ms(100);
}

//----------------------------------------------------------------------------------
// hien thi trang thai nhap nhay(tat nhap nhay)
void lcd_blink_off()
{
    lcd_putc(0x0C);
    delay_us(1);
}
// =================================================================================
// ham thuc hien chuong trinh chay cho mode 2 
// su dung timer de hien thi thoi gian led lcd
void set_time()
{
    // gia tri gio da duoc doc ra gio hien thi len lcd
   if(state_settime)// dung de hien thi 1 lan duy nhat
   {
        lcd_gotoxy(1,2);
        printf(lcd_putc,"Setup time          ");
        lcd_gotoxy(1,3);
        printf(lcd_putc,"                    ");  
        lcd_gotoxy(1,4);
        printf(lcd_putc,"                    "); 
        // hien thi gia tri thoi gian ra lcd   
        put_time(hour,5,3);
        lcd_putc(":");
        put_time(minute,8,3);
        lcd_putc(":");
        put_time(second,11,3); 
        // duoc set lai bang 1 khi chuuyen che do setup
        state_settime=0;
        set_flag_mode2=0;
   }  
   
   // khi nut menu duoc bam chuyen cac che do hoat dong trong mode 2
   if(!SET_BTN)
   {
      // bat loa len tao xung o chan loa
      Speaker = 1;
      delay_ms(150);
      Speaker = 0;
      delay_ms(150); 
      // cho den khi nut duoc nha ra
      while(!SET_BTN);
      //tang bien chuyen che do
      set_flag_mode2++;
      if(set_flag_mode2==5)        
      {
         set_flag_mode2=0;
         // xoa dong stored di
         lcd_gotoxy(1,4);
         printf(lcd_putc,"                    ");      
      }
   } 
   // su ly cac trang thai co. setup thoi gian thuc  
   switch(set_flag_mode2)
   {
      // de do trang thai nhap nhay tai vi tri cua giay.
      // neu an nut tang giam thi se thay doi gia tri giay
      case 0:  
      {    
         lcd_gotoxy(11,3); 
         lcd_blink_on(); 
         //delay_ms(10);
         put_time(second,11,3);
         delay_ms(100);
         if(!UP_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            second++;
            if(second==60)        second=0; 
            // hien thi len lcd   
            //put_time(second,11,3);
         } 
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            second--;
            if(second==255)        second=59; 
            // hien thi len lcd
            //put_time(second,11,3);
            
         } 
      } 
      break;
      // neu an nut tang giam thi se thay doi gia tri phut
      case 1:             
      {
         lcd_gotoxy(8,3); 
         lcd_blink_on(); 
         put_time(minute,8,3);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            minute++;
            if(minute==60)    minute=0; 
           // put_time(minute,8,3); 
          }
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
                    //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            minute--;
            if(minute==255)   minute=59;
           // put_time(minute,8,3);
         }
     }
      break;
      // neu an nut tang giam thi se thay doi gia tri gio
      case 2:             //set minute
      {    
         lcd_gotoxy(5,3); 
         lcd_blink_on(); 
         put_time(hour,5,3);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            hour++;
            if(hour==24)   hour=0;   
            //put_time(hour,5,3); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            hour--;
            if(hour==255)   hour=23;   
            //put_time(hour,5,3); 
         }
       }
      break;
      // luu gia tri setup vao ds1307
      case 3:            
      {
         //lcd_blink_off(); 
         lcd_gotoxy(1,4);
         printf(lcd_putc,">Stored       ");
         set_flag_mode2=4;
         write_DS1307(0x00,bin2bcd(second));
         write_DS1307(0x01,bin2bcd(minute));
         write_DS1307(0x02,bin2bcd(hour));
         // luu gia tri vao ds 1307
      }
      break;
      // 
      default:
      break;
   }
}
// ham dung set date
void set_date()
{
    // gia tri gio da duoc doc ra gio hien thi len lcd
   if(state_settime)// dung de hien thi 1 lan duy nhat
   {
        lcd_gotoxy(1,2);
        printf(lcd_putc,"Setup date          ");
        lcd_gotoxy(1,3);
        printf(lcd_putc,"                    ");  
        lcd_gotoxy(1,4);
        printf(lcd_putc,"                    "); 
        put_date_info(); 
        // duoc set lai bang 1 khi chuuyen che do setup
        state_settime=0;
        set_flag_mode2=0;
   }  
   
   // khi nut menu duoc bam chuyen cac che do hoat dong trong mode 2
   if(!SET_BTN)
   {
      // bat loa len tao xung o chan loa
      Speaker = 1;
      delay_ms(150);
      Speaker = 0;
      delay_ms(150); 
      // cho den khi nut duoc nha ra
      while(!SET_BTN);
      //tang bien chuyen che do
      set_flag_mode2++;
      if(set_flag_mode2==6)        
         {
            set_flag_mode2=0;
            // xoa dong stored di
            lcd_gotoxy(1,4);
            printf(lcd_putc,"                  ");
            
         }
   } 
   // su ly cac trang thai co. setup thoi gian thuc  
   switch(set_flag_mode2)
   {
      // de do trang thai nhap nhay tai vi tri cua giay.
      // neu an nut tang giam thi se thay doi gia tri giay
      case 0:  
      {    
         lcd_gotoxy(10,3); 
         lcd_blink_on(); 
         put_time(month,10,3);
         delay_ms(100);
         
         if(!UP_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            month++;
            if(month==13)        month=1; 
            // hien thi len lcd
            //put_time(month,10,3);
            
         } 
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            month--;
            if(month==0)        month=12; 
            // hien thi len lcd
            //put_time(month,10,3);
            
         } 
      } 
      break;
      // neu an nut tang giam thi se thay doi gia tri phut
      case 1:             
      {
         lcd_gotoxy(7,3); 
         lcd_blink_on(); 
         put_time(date,7,3); 
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            date++;
            if(date==32)    date=1; 
            //put_time(date,7,3); 
          }
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
                    //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            date--;
            if(date==0)   date=31;
            //put_time(date,7,3);
         }
     }
      break;
      // neu an nut tang giam thi se thay doi gia tri gio
      case 2:             //set minute
      {    
         lcd_gotoxy(15,3); 
         lcd_blink_on(); 
         put_time(year,15,3);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            year++;
            if(year==100)   year=0;
           // put_time(year,15,3); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            year--;
            if(year==255)   year=99;   
           // put_time(hour,15,3); 
         }
       }
      break;
      // neu an nut tang giam thi se thay doi gia tri gio
      case 3:             //set minute
      {    
         lcd_gotoxy(4,3); 
         lcd_blink_on();  
         put_date_info();
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            day++;
            if(day==8)   day=1;   
            //put_date_info(); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            day--;
            if(day==0)   day=7;   
            //put_date_info(); 
         }
       }
      break;
      // luu gia tri setup vao ds1307
      case 4:            
      {
         //lcd_blink_off(); 
         lcd_gotoxy(1,4);
         printf(lcd_putc,">Stored       ");
         // luu gia tri vao ds 1307
         write_DS1307(0x03,bin2bcd(day));
         write_DS1307(0x04,bin2bcd(date));
         write_DS1307(0x05,bin2bcd(month));
         write_DS1307(0x06,bin2bcd(year));
         set_flag_mode2=5;
      }
      break;
      // 
      default:
      break;
   }
}

// ham set thoi gian on off cho may bom-> che do 2 (dieu khien bom)
// ham nay duoc goi khi dang o che do 2 con o cac che do khac
// ham nay khong duoc goi den
void set_time_on_off()
{
    // gia tri gio da duoc doc ra gio hien thi len lcd
   if(state_settime)// dung de hien thi 1 lan duy nhat
   {
        lcd_gotoxy(1,2);
        printf(lcd_putc,"Thoi gian bom       ");
        lcd_gotoxy(1,3);
        printf(lcd_putc,"            ON:  h  ");  
        lcd_gotoxy(1,4);
        printf(lcd_putc,"            IN:    p"); 
        STT_time_on=1;
        lcd_gotoxy(15,2);
        printf(lcd_putc,"%u",STT_time_on);
        put_time(DEV_INFO[STT_time_on].hour_on,16,3);
        put_time(DEV_INFO[STT_time_on].minute_on,19,3);
        put_time(DEV_INFO[STT_time_on].time_on,17,4);
        lcd_gotoxy(1,3);
        if(DEV_INFO[STT_time_on].state_on==1)
           printf(lcd_putc,"Enable ");
        else
           printf(lcd_putc,"Disable");
        // duoc set lai bang 1 khi chuuyen che do setup
        state_settime=0;
        set_flag_mode2=0;
   }  
   
   // khi nut menu duoc bam chuyen cac che do hoat dong trong mode 2
   if(!SET_BTN)
   {
      // bat loa len tao xung o chan loa
      Speaker = 1;
      delay_ms(150);
      Speaker = 0;
      delay_ms(150); 
      // cho den khi nut duoc nha ra
      while(!SET_BTN);
      //tang bien chuyen che do
      set_flag_mode2++;
      if(set_flag_mode2==7)        
         {
            set_flag_mode2=0;
            // xoa dong stored di
            lcd_gotoxy(1,4);
            printf(lcd_putc,"       ");
            
         }
   } 
   // su ly cac trang thai co. setup thoi gian thuc  
   switch(set_flag_mode2)
   {
      // de do trang thai nhap nhay tai vi tri cua .
      // neu an nut tang giam thi se thay doi gia tri giay
      case 0:  
      {
         lcd_gotoxy(15,2);
         lcd_putc(' ');
         delay_ms(100);
         lcd_gotoxy(15,2);
         printf(lcd_putc,"%u",STT_time_on);
         delay_ms(100);
         if(!UP_BTN)          //neu bam nut add--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            STT_time_on++;
            if(STT_time_on==4)        STT_time_on=1; 
            // hien thi thoi gian tuong ung len lcd
            //put_time(STT_time_on,15,2);
            put_time(DEV_INFO[STT_time_on].hour_on,16,3);
            put_time(DEV_INFO[STT_time_on].minute_on,19,3);
            put_time(DEV_INFO[STT_time_on].time_on,17,4);
            lcd_gotoxy(1,3);
            if(DEV_INFO[STT_time_on].state_on==1)
               printf(lcd_putc,"Enable ");
            else
               printf(lcd_putc,"Disable");           
         } 
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            STT_time_on--;
            if(STT_time_on==0)        STT_time_on=3; 
            //put_time(STT_time_on,15,2);
            put_time(DEV_INFO[STT_time_on].hour_on,16,3);
            put_time(DEV_INFO[STT_time_on].minute_on,19,3);
            put_time(DEV_INFO[STT_time_on].time_on,17,4);
            lcd_gotoxy(1,3);
            if(DEV_INFO[STT_time_on].state_on==1)
               printf(lcd_putc,"Enable ");
            else
               printf(lcd_putc,"Disable"); 
            
         } 
      } 
      break;
      // chon che do la disable hay enable gio bat tat may bom
      case 1:             
      {
         lcd_gotoxy(1,3); 
         lcd_blink_on();
         lcd_gotoxy(1,3); 
         if(DEV_INFO[STT_time_on].state_on==1)               
               printf(lcd_putc,"Enable");
         else
               printf(lcd_putc,"Disable ");
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            lcd_gotoxy(1,3); 
            if(DEV_INFO[STT_time_on].state_on==1)               
            {
               printf(lcd_putc,"Disable");
               DEV_INFO[STT_time_on].state_on=0;
            }
            else
            {
               printf(lcd_putc,"Enable ");
               DEV_INFO[STT_time_on].state_on=1;
            }
          }
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
                    //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            lcd_gotoxy(1,3); 
            if(DEV_INFO[STT_time_on].state_on==1)               
            {
               printf(lcd_putc,"Disable");
               DEV_INFO[STT_time_on].state_on=0;
            }
            else
            {
               printf(lcd_putc,"Enable ");
               DEV_INFO[STT_time_on].state_on=1;
            }
         }
     }
      break;
      // neu an nut tang giam thi se thay doi gia tri gio bat bom
      case 2:             //set minute
      {    
         lcd_gotoxy(16,3); 
         lcd_blink_on(); 
         put_time(DEV_INFO[STT_time_on].hour_on,16,3);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].hour_on++;
            if(DEV_INFO[STT_time_on].hour_on==24)   DEV_INFO[STT_time_on].hour_on=0;   
            //put_time(DEV_INFO[STT_time_on].hour_on,16,3); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].hour_on--;
            if(DEV_INFO[STT_time_on].hour_on==255)   DEV_INFO[STT_time_on].hour_on=23;   
            //put_time(DEV_INFO[STT_time_on].hour_on,16,3); 
         }
       }
      break;
      // neu an nut tang giam thi se thay doi gia tri phut bat
      case 3:             //set minute
      {    
         lcd_gotoxy(19,3); 
         lcd_blink_on(); 
         put_time(DEV_INFO[STT_time_on].minute_on,19,3);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].minute_on+=5;
            if(DEV_INFO[STT_time_on].minute_on==60)   DEV_INFO[STT_time_on].minute_on=0;   
            //put_time(DEV_INFO[STT_time_on].minute_on,19,3); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].minute_on-=5;
            if(DEV_INFO[STT_time_on].minute_on>250)   DEV_INFO[STT_time_on].minute_on=55;   
            //put_time(DEV_INFO[STT_time_on].minute_on,19,3); 
         }
       }
      break;
      // luu gia tri setup vao ds1307
      case 4:            
      {
         lcd_gotoxy(17,4); 
         lcd_blink_on(); 
         put_time(DEV_INFO[STT_time_on].time_on,17,4);
         delay_ms(100);
         if(!UP_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].time_on+=5;
            if(DEV_INFO[STT_time_on].time_on==65)   DEV_INFO[STT_time_on].time_on=0;   
            //put_time(DEV_INFO[STT_time_on].time_on,17,4); 
         }
         if(!DOWN_BTN)            //neu bam button add------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            DEV_INFO[STT_time_on].time_on-=5;
            if(DEV_INFO[STT_time_on].time_on>250)   DEV_INFO[STT_time_on].time_on=60;   
            //put_time(DEV_INFO[STT_time_on].time_on,17,4); 
         }
      }
      break;
      // luu cac gia tri da set vao eeprom tren 24LC01
      case 5:
      {
         //lcd_blink_off(); 
         //luu vao eeprom dia chi luu la tu 0x0A
         write_ext_eeprom(0x0A+4*STT_time_on+0,DEV_INFO[STT_time_on].hour_on);
         write_ext_eeprom(0x0A+4*STT_time_on+1,DEV_INFO[STT_time_on].minute_on);
         write_ext_eeprom(0x0A+4*STT_time_on+2,DEV_INFO[STT_time_on].time_on);
         write_ext_eeprom(0x0A+4*STT_time_on+3,DEV_INFO[STT_time_on].state_on);
         // hien thi len lcd trang thai da luu du lieu vao eeprom
         lcd_gotoxy(1,4);
         printf(lcd_putc,">Stored");
         set_flag_mode2=6;
         
      }
      break;
      default:
      break;
   }
}

// ham loa du lieu tu eeprom
void load_eeprom()
{
   unsigned char i;
   for(i=1;i<5;i++)
   {
      DEV_INFO[i].hour_on   = read_ext_eeprom(0x0A+4*i+0);
      DEV_INFO[i].minute_on = read_ext_eeprom(0x0A+4*i+1);
      DEV_INFO[i].time_on   = read_ext_eeprom(0x0A+4*i+2);
      DEV_INFO[i].state_on  = read_ext_eeprom(0x0A+4*i+3);
   }
   // load du lieu temp_max
   temp_max = read_ext_eeprom(0x07);
   huff_max = read_ext_eeprom(0x08);
   mode_run = read_ext_eeprom(0x09);
}
// ham chay khi dang o mode 2
void run_mode2()
{
   unsigned char count;
   for(count=1;count<4;count++)
   {
      if(maybom_on==0)
      {
         sum_on_max=DEV_INFO[count].hour_on*60+DEV_INFO[count].minute_on+DEV_INFO[count].time_on;
         sum_on_min=DEV_INFO[count].hour_on*60+DEV_INFO[count].minute_on-1;
         count_in=count;
      }
      sum_on=  hour*60+minute;
      if(sum_on<sum_on_max&&sum_on>sum_on_min&&DEV_INFO[count_in].state_on==1)
      {         
         on_all_van();
         delay_ms(500);
         if(maybom_on==0)
            lcd_init();
         maybom_on=1;
      }
      else
      {

         off_all_van();
         maybom_on=0;
      }
   }
}

// ham setup cac mode hoat dong cho mach
void set_mode()
{
    // gia tri gio da duoc doc ra gio hien thi len lcd
   if(state_settime)// dung de hien thi 1 lan duy nhat
   {
        lcd_gotoxy(1,2);
        printf(lcd_putc,"SETUP MODE :        ");
        lcd_gotoxy(1,3);
        printf(lcd_putc,"                    ");  
        lcd_gotoxy(1,4);
        printf(lcd_putc,"                    "); 
        mode_run_flag=mode_run;
        lcd_gotoxy(12,2);
        printf(lcd_putc,"%u",mode_run_flag);
        // duoc set lai bang 1 khi chuuyen che do setup
        state_settime=0;
        set_flag_mode2=0;
   }  
   
   // khi nut menu duoc bam chuyen cac che do hoat dong trong mode 2
   if(!SET_BTN)
   {
      // bat loa len tao xung o chan loa
      Speaker = 1;
      delay_ms(150);
      Speaker = 0;
      delay_ms(150); 
      // cho den khi nut duoc nha ra
      while(!SET_BTN);
      //tang bien chuyen che do
      set_flag_mode2++;
      if(set_flag_mode2==3)        
         {
            set_flag_mode2=0;
            // xoa dong stored di
            lcd_gotoxy(1,3);
            printf(lcd_putc,"       ");
            
         }
   } 
   // su ly cac trang thai co. setup thoi gian thuc  
   switch(set_flag_mode2)
   {
      // de do trang thai nhap nhay tai vi tri cua .
      // neu an nut tang giam thi se thay doi gia tri giay
      case 0:  
      {
         lcd_gotoxy(12,2);
         lcd_putc(' ');
         delay_ms(100);
         lcd_gotoxy(12,2);
         printf(lcd_putc,"%u",mode_run_flag);
         delay_ms(100);
         if(!UP_BTN)          //neu bam nut add--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            mode_run_flag++;
            if(mode_run_flag==4)        mode_run_flag=1;         
         } 
         if(!DOWN_BTN)          //neu bam nut sub--------
         {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            mode_run_flag--;
            if(mode_run_flag==0)        mode_run_flag=3;             
         } 
      } 
      break;
      // chon che do la disable hay enable gio bat tat may bom
      case 1:             
      {
         lcd_gotoxy(1,3); 
         printf(lcd_putc,">Stored");
         mode_run=mode_run_flag;
         lcd_gotoxy(1,1);
         printf(lcd_putc,"  CHE DO : MODE %u   ",mode_run); 
         //thuc hien qua trinh ngat uart khi chuyen mode khong con o mode 2
         if(mode_run==2)
            disable_interrupts(INT_RDA);
         if(mode_run!=2)
            enable_interrupts(INT_RDA);   
         //neu nhu chuyen mode khi mode 2 dang hoat dong
         // mode 2 dang trong thoi gian bat bom-> tat bom
         if(maybom_on==1)
            maybom_on=0;
         //luu vao eeprom
         write_ext_eeprom(0x09,mode_run);
         off_all_van();
         set_flag_mode2=2;
      }
      break; 
      default:
      break;
   }
}

