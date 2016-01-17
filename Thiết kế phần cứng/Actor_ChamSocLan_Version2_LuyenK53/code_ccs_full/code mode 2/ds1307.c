#define DS1307_SDA  PIN_C4
#define DS1307_SCL  PIN_C3
#use i2c(master, sda=DS1307_SDA, scl=DS1307_SCL)

//int i;
/*---------------------- Chuy?n du li?u m? BIN cua MASTER -> du li?u m? BCD cho DS1307------------------
int write(int data)                  // MASTER -> DS1307.
   {                                 //      x -> y.
      int x,y;                       //vd:   7 -> 7  = 0x07.
      x = data;                      //vd:  12 -> 18 = 0x12.
      if(x<10)
         {
            y = x;
         }
      else if(x>=10)
         {
            y = (x/10 * 6) + x;            //vd:  29 -> 41 = 0x29.
         }
      return(y);                     //vd:  35 -> 53 = 0x35.
   }
   
   
/*---------------------- Chuy?n du li?u m? BCD cua DS1307 -> du li?u m? BIN cho MASTER------------------
int read(int data)
   {                                 // MASTER <- DS1307.
      int x,y,z;                     //      x <- y.
      y = data;                      //vd:   5 <-  5 = 0x05.
      i=0;                           //vd:  10 <- 16 = 0x10.
      if(y<10)                       //vd:  20 <- 32 = 0x20.
         {                           //vd:  30 <- 48 = 0x30.
            x = y;                   //vd:  40 <- 64 = 0x40.
            z = x;                   //vd:  50 <- 80 = 0x50.
         }                           //..... 
      else if(y>=10)
         {
            do {
                  x = y - (6 * i);        
                  z = (x/10 * 6) + x;
                  i++;
               }
           while(z!=y);
         }
      return(x);    
   }





/*
// Convert normal decimal numbers to binary coded decimal
int8 decToBcd(int8 val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
int8 bcdToDec(int8 val)
{
  return ( (val/16*10) + (val%16) );
}
*/

BYTE bin2bcd(BYTE binary_value) 
{ 
  return(((binary_value/10*16)+(binary_value%10))); 
} 


// Input range - 00 to 99. 
BYTE bcd2bin(BYTE bcd_value) 
{ 
  return((bcd_value/16*10)+(bcd_value%16)); 
} 

//==========================
// initial DS1307
//==========================
void init_DS1307()
{
   output_float(DS1307_SCL);
   output_float(DS1307_SDA);
}
//==========================
// write data one byte to
// DS1307
//==========================
void write_DS1307(byte address, BYTE data)
{
   short int status;
   i2c_start();
   i2c_write(0xd0);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
   i2c_start();
   status=i2c_write(0xd0);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(0xd0);
   }
}
//==========================
// read data one byte from DS1307
//==========================
BYTE read_DS1307(byte address)
{
   BYTE data;
   i2c_start();
   i2c_write(0xd0);
   i2c_write(address);
   i2c_start();
   i2c_write(0xd1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
/*
void setDateDs1307(int8 second,        // 0-59
                   int8 minute,        // 0-59
                   int8 hour,          // 1-23
                   int8 day,           // 1-7
                   int8 date,          // 1-28/29/30/31
                   int8 month,         // 1-12
                   int8 year,          // 0-99
                   int8 control)
                   
{
   write_DS1307(0x0, decToBcd(second));    // 0 to bit 7 starts the clock
   write_DS1307(0x1, decToBcd(minute));
   write_DS1307(0x2, decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   write_DS1307(0x3, decToBcd(day));
   write_DS1307(0x4, decToBcd(date));
   write_DS1307(0x5, decToBcd(month));
   write_DS1307(0x6, decToBcd(year));
   write_DS1307(0x7, decToBcd(control));

}
*/
void setDateDs1307(int8 second,        // 0-59
                   int8 minute,        // 0-59
                   int8 hour,          // 1-23
                   int8 day,           // 1-7
                   int8 date,          // 1-28/29/30/31
                   int8 month,         // 1-12
                   int8 year)          // 0-99
                   
                   
{
   write_DS1307(0x0, bin2bcd(second));    // 0 to bit 7 starts the clock
   write_DS1307(0x1, bin2bcd(minute));
   write_DS1307(0x2, bin2bcd(hour));      // If you want 12 hour am/pm you need to set
                                        // bit 6 (also need to change readDateDs1307)
   write_DS1307(0x3, bin2bcd(day));
   write_DS1307(0x4, bin2bcd(date));
   write_DS1307(0x5, bin2bcd(month));
   write_DS1307(0x6, bin2bcd(year));
}


