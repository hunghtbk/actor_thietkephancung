// thu vien dieu khien tat cac van cua may bom
// co cac che do bat tat cac van
// dinh nghia cac chan vi dieu khien noi voi van
#define Maybom PORTC.1
#define Van1   PORTA.2
#define Van2   PORTA.2
#define Van3   PORTA.2
#define Van4   PORTA.2
#define Van5   PORTA.2
#define Van6   PORTA.2
// khai bao cac ham khoi tao io cho pin 

#define Maybom_IO PORTC.1
#define Van1_IO   PORTA.2
#define Van2_IO   PORTA.2
#define Van3_IO   PORTA.2
#define Van4_IO   PORTA.2
#define Van5_IO   PORTA.2
#define Van6_IO   PORTA.2

// ham khoi tao cho cac chan
void setup_io_dieukhien()
{
   Van1_IO=0;
   Van2_IO=0;
   Van3_IO=0;
   Van4_IO=0;
   Van5_IO=0;
   Van6_IO=0;
}
// ham bat tat ca cac van dieu tu len
void on_all_van()
{
   unsigned long i;
   Van1=1;
   Van2=1;
   Van3=1;
   Van4=1;
   Van5=1;
   Van6=1;
   for(i=0;i<3000;i++);//tre 1 khoang thoi gian roi moi bat may bom
   Maybom=1; 
}
// ham tat het cac van dien tu
void off_all_van()
{
   unsigned long i;
   Maybom=0;
   for(i=0;i<3000;i++);// tre 1 khoang thoi gian roi moi tat cac van 24VAC
   Van1=0;
   Van2=0;
   Van3=0;
   Van4=0;
   Van5=0;
   Van6=0;
}
// ham nay duoc su dung trong ngat uart khi nhan ban tin dieu khien
// tu gateway
// bien rx_data la bien luu gia tri data nhan tu uart
void dieukhien_van()
{
   unsigned char rx_data;
   unsigned long i;
   //nhan du lieu va dua vao du lieu nhan dc vao bien data
   // check xem ban tin du lieu la gi
   switch(rx_data)
   {
      // nhan duoc ban tin bat van 1
      case 0x81:
      {
         Van1=1;                 // bat van 1
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x81);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1;
      }
      break;
      // nhan duoc ban tin dieu khien van 2
      case 0x82:
      {
         Van2=1;                 // bat van 2
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x82);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1  ;       
      }
      break;
      // nhan duoc ban tin dieu khien van 3
      case 0x83:
      {
         Van3=1;                 // bat van 3
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x83);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1   ;      
      }
      break;
      // nhan duoc ban tin dieu khien van 4
      case 0x84:
      {
         Van4=1;                 // bat van 4
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x84);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1  ;       
      }
      break;
      // nhan duoc ban tin dieu khien van 5
      case 0x85:
      {
         Van5=1;                 // bat van 5
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x85);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1   ;      
      }
      break;
      // nhan duoc ban tin dieu khien van 6
      // van 6 dung cho vuon tue
      case 0x86:
      {
         Van6=1;                 // bat van 6
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x86);
         // check xem may bom co duoc bat khong?
         // neu may bom dang tat thi bat may bom len 
         if(May_bom!=1) Maybom=1   ;      
      }
      break;
      // nhan duoc ban tin bat tat ca cac van 
      case 0x8F:
      {               
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x8F);
         on_all_van();
      }
      break;
      // nhan duoc ban tin dieu khien tat van 1
      case 0x01:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x01);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van2==0&&Van3==0&&Van4==0&&Van5==0&&Van6==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
         }
         Van1=0;                 // tat van 1
      }
      break;
      // nhan duoc ban tin dieu khien tat van 2
      case 0x02:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x02);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van1==0&&Van3==0&&Van4==0&&Van5==0&&Van6==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
         }
         Van2=0;                 // tat van 2
      }
      break;
      // nhan duoc ban tin dieu khien tat van 3
      case 0x03:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x03);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van1==0&&Van2==0&&Van4==0&&Van5==0&&Van6==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
         }
         Van3=0;                 // tat van 3
      }
      break;
      // nhan duoc ban tin dieu khien tat van 4
      case 0x04:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x04);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van1==0&&Van2==0&&Van3==0&&Van5==0&&Van6==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
         }
         Van4=0;                 // tat van 4
      }
      break;
      // nhan duoc ban tin dieu khien tat van 5
      case 0x05:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x05);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van1==0&&Van2==0&&Van3==0&&Van4==0&&Van6==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
         }
         Van5=0;                 // tat van 5
      }
      break;
      // nhan duoc ban tin dieu khien tat van 6
      case 0x06:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x06);
         // check xem co con van nao duoc bat khong?
         // neu nhu cac van tat het thi tat may bom
         // sau do moi tat van duoc dieu khien
         if(Van1==0&&Van2==0&&Van3==0&&Van4==0&&Van5==0) 
         {
            Maybom=0;
            // tre 1 khoang thoi gian roi moi tat van dieu khien
            for(i=0;i<1000;i++);
            Van6=0;                 // tat van 1
         }
      }
      break;
      // nhan duoc ban tin dieu khien tat het cac van
      case 0x0F:
      {
         //gui lai ban tin phan hoi cho gateway thong qua uart
         uart_putchar(0x0F);
         off_all_van();
      }
   }
}
