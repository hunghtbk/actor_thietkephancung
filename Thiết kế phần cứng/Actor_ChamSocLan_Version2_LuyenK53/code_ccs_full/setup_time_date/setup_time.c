// FILE TIME_CONTROL DUNG DE SETUP CAC THONG SO CUA THOI GIAN
// DA BAO GOM KHAI BAO CAC THONG SO CAN THIET
// 
#include <delay.h>
#include <fixedlcd.h>

//unsigned char state_settime=1;
// DS1307 Real Time Clock functions
#include    <ds1307.h>
#define     MENU_BTN         PINC.1         //  nut bam lua chon cac che do
#define     SET_BTN          PIND.3         //  nut setup cac thong so
#define     UP_BTN           PINC.2         //  nut bam tang cac gia tri
#define     DOWN_BTN         PIND.2         //  nut bam giam cac che do
#define     AUTO_BTN         PIND.4         //  lua chon che do manual-auto
#define     Speaker          PORTC.0        //  pin tieng loa
//----------------------------------------------------------------------
// cac ham tuong tac voi lcd bao gom
// ham lcd_gotoxy() di den 1 vi tri tren lcd
// ham lcd_putchar(data) hien thi 1 gia tri data ra lcd
// ham lcd_puts("string");   hien thi 1 chuoi string ra lcd
//-------------------------------------------------------------

//bien luu gia tri thoi gian tam thoi
unsigned char second,minute,hour,day,date,month,year;
//bien luu xem dang chinh gio, phut, giay, ngay thang?
unsigned char  set_val=0;
//co set_flag dang o che do set thoi gian
unsigned char     set_flag=0;
// bien luu cac trang thai cua nut bam
unsigned char button_menu=1;
// dung de chuong trinh chi duoc goi 1 lan trong vong lap for
unsigned char state_settime;
//time----------------------------------------------------------------------
void put_time(unsigned char time,int x, int y);   // dua du lieu thoi gian ra lcd
void put_date_info();                             // hien thi cac gia tri ngay thang nam ra lcd
void put_time_info();                             // hien thi cac gia tri gio phut giay ra lcd
void lcd_blink_on();                              // tao nhap nhay tren lcd
void lcd_blink_off();                             // tat nhap nhay tren lcd
void time_change(int count);                      // ham thay doi gia tri thoi gian

//------------------------------------------------------------------------------------
//dung ham put_time de in ra second, minute, hour, date, month, year
// input : gia tri cua bien can in ra
//         vi tri cua bien can in ra tren LCD
// hien thi 2 ki tu cua 1 bien 
// output : none
void put_time(unsigned char time,unsigned char x, unsigned char y) 
{
    unsigned char time_low,time_high;
    // tach bien time thanh 2 so hien thi tren lcd
    time_high = time/10 + 0x30;
    time_low  = time%10 + 0x30; 
    // di den vi tri x,y hien thi gia tri time
    lcd_gotoxy(x,y);
    lcd_putchar(time_high);
    lcd_putchar(time_low);
}

//-----------------------------------------------------------------------------------
//void put_data_info : hien thi thong tin thu ngay thang tren lcd
//input: none
//output: none
void put_date_info()
{
    lcd_gotoxy(0,1);
    lcd_puts(" ");  
    switch(day)
    { 
        // neu la chu nhat 
        case 1:
            lcd_puts("CN ");
         break;
        // neu la thu 2 
        case 2:
            lcd_puts("T2 ");
        break;
        //neu la thu 3
        case 3:
        lcd_puts("T3 ");
        break; 
        // neu la thu 4
        case 4:
        lcd_puts("T4 ");
        break; 
        // neu la thu 5
        case 5:
        lcd_puts("T5 ");
        break; 
        // neu la thu 6
        case 6:
        lcd_puts("T6 ");
        break; 
        // neu la thu 7
        case 7:
        lcd_puts("T7 ");
        break; 
        // neu khong co truong hop nao thi out
        default: 
        break;
    }
    put_time(date,4,1);
    lcd_putchar('-');
    put_time(month,7,1);
    lcd_putchar('-');
    lcd_gotoxy(10,1);   
    lcd_puts("20    "); 
    put_time(year,12,1);
}

//-----------------------------------------------------------------------------------
//void put_data_info : hien thi thong tin thu ngay thang tren lcd
//input: gia tri gio phut giay
//output: none
void put_time_info()
{
    lcd_gotoxy(0,0);
    lcd_puts("    ");
    put_time(hour,4,0); 
    lcd_putchar(':');
    put_time(minute,7,0);
    lcd_putchar(':');
    put_time(second,10,0);
    lcd_gotoxy(12,0);
    lcd_puts("    ");
}

//------------------------------------------------------------------------------------   
// hien thi trang thai nhap nhay tren lcd
  void lcd_blink_on()
{
    lcd_putchar(0x0D);  //0D
    delay_us(10);
}

//----------------------------------------------------------------------------------
// hien thi trang thai nhap nhay(tat nhap nhay)
void lcd_blink_off()
{
    lcd_putchar(0x0C);
    delay_us(10);
}

//------------------------------------------------------------------------------------
// di chuyen con tro den vi tri can setup gia tri thoi gian
// output : none
void lcd_blink_move()
{
    //dang o che do set thoi gian  
    if (set_flag==1)    
    {
        switch(set_val)
        {
            case 0:             //set complete
                lcd_blink_off();
            break;
            case 1:             //set second
                lcd_gotoxy(10,0);
                //delay_ms(2);
            break;
            case 2:             //set minute
                lcd_gotoxy(7,0);
                //delay_ms(2);
            break;
            case 3:             //set hour
                lcd_gotoxy(4,0);
                //delay_ms(2);
            break;
            case 4:             //set day
                lcd_gotoxy(1,1);
                //delay_ms(2);
            break;
            case 5:             //set date
                lcd_gotoxy(4,1);
                //delay_ms(2);
            break;
            case 6:             //set month
                lcd_gotoxy(7,1);
                //delay_ms(2);
            break;
            case 7:             //set year
                lcd_gotoxy(12,1);
                //delay_ms(2);
            break;
            default:
            break;
        }
    }
}

//----------------------------------------------------------------------------------
// ham thay doi trang thai 
void change_state()
{
    //unsigned long temp;
    //dang o trang thai binh thuong set_flag=0 va nhan duoc nut bam
    if(set_flag==0)
    {
        if(!SET_BTN)              //neu button set duoc bam-->chuyen sang trang thai set thoi gian
        {        
            Speaker = 1;
            delay_ms(100);
            Speaker = 0;
            delay_ms(100);
            put_date_info();
            put_time_info();
            while(!SET_BTN)         //cho den khi set button duoc nha?
            {
                // neu giu nut bam qua lau dua ra canh bao\
                // sau thoi gian 5s
                //error_BTN(temp);                   
            }
            //temp=0;
            set_flag=1;             //bao hieu dang la che do set time
            set_val=1;              //tang bien set_val de move blink den vi tri tuong ung
            lcd_blink_on();         //bat blinking  
            //dua contro den vi tri set gia tri giay
            lcd_blink_move();       //dua con tro toi vi tri tuong ung            
        }
        // neu khong duoc an nut thi chi hien thi thoi gian thuc
       /* else
        {
            //delay_ms(5);
            //if(state_settime)
            //{
                //set_val=0;
                //set_flag=0; 
                delay_ms(100);
                button_menu=0;
                lcd_gotoxy(0,0);
                lcd_puts("Setup thoi gian "); 
                lcd_gotoxy(0,1);
                lcd_puts("                ");
                //Speaker=1;   
            //}
            //state_settime=0;
        }*/
    }
           
    //dang o trang thai set time  set_flag=1
    if(set_flag==1)
    {
        while(!UP_BTN)            //neu bam button add------
        {
            //cho toi khi tha nut bam
            time_change(1);         //goi ham add time
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
        }

        while(!DOWN_BTN)          //neu bam nut sub--------
        {
            //cho toi khi tha nut bam
            time_change(-1);        //goi ham tru thoi gian
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
        }

        if(SET_BTN==0)              //neu bam nut set---------
        {
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150);
            while(!SET_BTN)        //cho den khi tha nut bam 
            {    
                //error_BTN(temp); 
            }
            //temp=0;
            set_val++;
            if(set_val==9)          
                set_val=1;
            if(set_val!=8)
            {
                lcd_gotoxy(13,0);
                lcd_puts("  "); 
                lcd_blink_on()  ;
                lcd_blink_move();       //dua con tro toi vi tri tiep theo   
            }
            else 
            {
                lcd_blink_off()  ;
                lcd_gotoxy(13,0);
                lcd_puts("OK"); 
            rtc_set_time(hour,minute,second);
                rtc_set_date(day,date,month,year);
            }
         }  
        //else if(ok_BTN==0)         //neu bam nut ok---------
//        if(OK_BTN==0) 
//        {
//            Speaker = 1;
//            delay_ms(150);
//            Speaker = 0;
//            delay_ms(150);
//            while(!OK_BTN)        //cho toi khi tha nut bam  
//            { 
//                //error_BTN(temp);    
//            }
//            //temp=0;
//            lcd_blink_off();
//            set_val=0;
//            set_flag=0;
//            button_menu=1;
//            rtc_set_time(hour,minute,second);
//            rtc_set_date(day,date,month,year);
//        }    
     }

}

//----------------------------------------------------------------------------------
// thay doi gia tra cua bien duoc setup
void time_change(int count)
{
    if(set_flag==1)  //dang la set thoi gian
    {
        switch(set_val) //set_val se cho biet dang set o vi tri nao
        {
            case 1:             // set second
                    second+=count;
                    if (second==60) second=0;
                    if (second==255) second =59;
                    //rtc_write(0x00,second);
                    put_time(second,10,0);
                    lcd_gotoxy(10,0);
                    //delay_ms(2);
                    break;
            case 2:             //set minute
                    minute+=count;
                    if (minute==60) minute=0;
                    if (minute==255) minute=59;
                    //rtc_write(0x01,minute);
                    //write_ds(Reg_minute,minute);
                    put_time(minute,7,0);
                    lcd_gotoxy(7,0);
                    //delay_ms(2);
                    break;
            case 3:             //set hour
                    hour+=count;
                    if (hour==24) hour=0;
                    if (hour==255) hour=23;
                    //rtc_write(0x02,hour);
                    //write_ds(Reg_hour,hour);
                    put_time(hour,4,0);
                    lcd_gotoxy(4,0);
                    //delay_ms(2);
                    break;
            case 4:             //set day
                    day+=count;
                    if(day==0) day = 7;
                    if(day==8)  day=1;  
                    //rtc_write(0x03,day);
                    //write_ds(Reg_day,day);
                    put_date_info();
                    lcd_gotoxy(1,1);
                    //delay_ms(2);
                    break;
            case 5:             //set date
                    date+=count;
                    if ( date==32)  date=1;
                    if (date==255) date=31;
                    //rtc_write(0x04,date);
                    //write_ds(Reg_date, date);
                    put_time( date,4,1);
                    lcd_gotoxy(4,1);
                    //delay_ms(2);
                    break;
            case 6:             //set month
                    month+=count;
                    if ( month==13)  month=1;
                    if (month == 0) month = 12;  
                    //rtc_write(0x05,month);
                    //write_ds(Reg_month, month);
                    put_time(month,7,1);
                    lcd_gotoxy(7,1);
                    //delay_ms(2);
                    break;
            case 7:             //set year
                    year+=count;
                    if ( year==100)  year=1;
                    if (year == 255) year=99;
                    //rtc_write(0x06,year);
                    //write_ds(Reg_year,  year);
                    put_time( year,12,1);
                    lcd_gotoxy(12,1);
                    //delay_ms(2);
                    break;
            default:
                    break;
        }//end switch
    } //end if
}

//-----------------------------------------------------------------------------------
void check_set_ds1307()
{
   // if(SET_BTN ==0 || set_flag==1) //neu dang o che do set
        change_state();    
}

//------------------------------------------------------------------------------------
// ham su ly che do setup time
void set_time()
{
    if(state_settime)
    {
        lcd_gotoxy(0,0);
        lcd_puts("Setup time      ");
        lcd_gotoxy(0,1);
        lcd_puts("                ");  
        // hien thi gia tri thoi gian ra lcd   
        put_time(hour,5,1);
        lcd_puts(":");
        put_time(minute,8,1);
        lcd_puts(":");
        put_time(second,11,1); 
        // duoc set lai bang 1 khi chuuyen che do setup
        state_settime=0;
    }  
    
    if(!SET_BTN)          //neu bam nut enter(nut set)--------
    {
        //cho toi khi tha nut bam
        Speaker = 1;
        delay_ms(150);
        Speaker = 0;
        delay_ms(150); 
        set_flagtiet++;
        if(set_flagtiet==7)        set_flagtiet=0; 
        if(set_flagtiet!=6)
        {
            lcd_gotoxy(2,1);
            lcd_puts("     ");
        }  
    } 
         
    switch(set_flagtiet)
    {
        case 0:             //set complete
        {    
            lcd_gotoxy(4,0); 
            lcd_blink_on(); 
            delay_ms(10);  
            lcd_gotoxy(0,1);
            if(enable[STT_tiet]==1)  
                lcd_puts("EN ");
            else
                lcd_puts("DIS");
            if(!UP_BTN)            //neu bam button add------
            {
            //cho toi khi tha nut bam
                Speaker = 1;
                delay_ms(150);
                Speaker = 0;
                delay_ms(150);
                STT_tiet++;
                if(STT_tiet==13)        STT_tiet=1; 
                put_time(STT_tiet,4,0);  
                put_time(h_in[STT_tiet],11,0);
                put_time(m_in[STT_tiet],14,0);
                put_time(h_out[STT_tiet],11,1);
                put_time(m_out[STT_tiet],14,1); 
                lcd_gotoxy(0,1);
                if(enable[STT_tiet]==1)  
                    lcd_puts("EN ");
                else
                    lcd_puts("DIS");
            }

        if(!DOWN_BTN)          //neu bam nut sub--------
        {
            //cho toi khi tha nut bam
            Speaker = 1;
            delay_ms(150);
            Speaker = 0;
            delay_ms(150); 
            STT_tiet--;
            if(STT_tiet==0)        STT_tiet=12;   
            //lcd_gotoxy(5,0);
            put_time(STT_tiet,4,0); 
            put_time(h_in[STT_tiet],11,0);
            put_time(h_in[STT_tiet],11,0);
            put_time(m_in[STT_tiet],14,0);
            put_time(h_out[STT_tiet],11,1);
            put_time(m_out[STT_tiet],14,1); 
            lcd_gotoxy(0,1);
                if(enable[STT_tiet]==1)  
                    lcd_puts("EN ");
                else
                    lcd_puts("DIS");
        }   
        } 
        break;
        case 1:             //set second
        {
                lcd_gotoxy(11,0); 
                lcd_blink_on(); 
               if(!UP_BTN)            //neu bam button add------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150);
                    h_in[STT_tiet]++;
                    if(h_in[STT_tiet]==24)    h_in[STT_tiet]=0; 
                    put_time(h_in[STT_tiet],11,0); 
                }

                if(!DOWN_BTN)          //neu bam nut sub--------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150); 
                    if(h_in[STT_tiet]==0)
                        h_in[STT_tiet]=12;
                    else
                        h_in[STT_tiet]--;  
                    put_time(h_in[STT_tiet],11,0);
                }
            //delay_ms(2);
        }
        break;
        case 2:             //set minute
        {    
            lcd_gotoxy(14,0); 
            lcd_blink_on();  
               if(!UP_BTN)            //neu bam button add------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150);
                    m_in[STT_tiet]++;
                    if(m_in[STT_tiet]==60)    m_in[STT_tiet]=0;   
                    //put_tietinfo();
                    put_time(m_in[STT_tiet],14,0); 
                }

                if(!DOWN_BTN)          //neu bam nut sub--------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150); 
                    if(m_in[STT_tiet]==0)
                        m_in[STT_tiet]=59;
                    else
                        m_in[STT_tiet]--; 
                    put_time(m_in[STT_tiet],14,0);
                }
        }
        break;
        case 3:             //set hour
        {
                lcd_gotoxy(11,1);
                //delay_ms(2);
                lcd_blink_on(); 
                if(!UP_BTN)            //neu bam button add------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150);
                    h_out[STT_tiet]++;
                    if(h_out[STT_tiet]==24)    h_out[STT_tiet]=0;  
                    put_time(h_out[STT_tiet],11,1);
                }

                if(!DOWN_BTN)          //neu bam nut sub--------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150); 
                    if(h_out[STT_tiet]==0)
                        h_out[STT_tiet]=12;
                    else
                        h_out[STT_tiet]--; 
                    put_time(h_out[STT_tiet],11,1);
                }
        }
        break;
        case 4:             //set day
        {
            lcd_gotoxy(14,1); 
            lcd_blink_on();    
               if(!UP_BTN)            //neu bam button add------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150);
                    m_in[STT_tiet]++;
                    if(m_out[STT_tiet]==60)    m_out[STT_tiet]=0;  
                    put_time(m_out[STT_tiet],14,1);
                }

                if(!DOWN_BTN)          //neu bam nut sub--------
                {
                    //cho toi khi tha nut bam
                    Speaker = 1;
                    delay_ms(150);
                    Speaker = 0;
                    delay_ms(150); 
                    if(m_out[STT_tiet]==0)
                        m_out[STT_tiet]=59;
                    else
                        m_out[STT_tiet]--; 
                    put_time(m_out[STT_tiet],14,1);
                }
        }
        break;
        case 5:             //set date  
        {    
            lcd_gotoxy(0,1);
            lcd_blink_on();
            if(!UP_BTN)            //neu bam button add------
            {
                //cho toi khi tha nut bam
                Speaker = 1;
                delay_ms(150);
                Speaker = 0;
                delay_ms(150);
                if(enable[STT_tiet]==1) 
                {
                    enable[STT_tiet]=0; 
                    lcd_gotoxy(0,1);
                    lcd_puts("DIS");
                }
                    
                else    
                {
                    enable[STT_tiet]=1;
                    lcd_gotoxy(0,1);
                    lcd_puts("EN ");  
                }
            }

            if(!DOWN_BTN)          //neu bam nut sub--------
            {
                //cho toi khi tha nut bam
                Speaker = 1;
                delay_ms(150);
                Speaker = 0;
                delay_ms(150); 
                if(enable[STT_tiet]==1) 
                {
                    enable[STT_tiet]=0; 
                    lcd_gotoxy(0,1);
                    lcd_puts("DIS");
                }
                    
                else    
                {
                    enable[STT_tiet]=1;
                    lcd_gotoxy(0,1);
                    lcd_puts("EN ");  
                }
            }
        }  
        break;
        case 6:             //set date  
        {
            lcd_blink_off();
            lcd_gotoxy(0,1);
            lcd_puts("Stored");       
            //delay_ms(2); 
        } 
        break;
        default:
        break;
    }
}

