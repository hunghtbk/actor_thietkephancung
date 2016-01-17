/* 
   Mach actor thuc hien nhiem vu canh bao chay theo 
   _ tin hieu dieu khien qua uart giao tiep voi node mang zigbee
   _ hien thi gia tri ra led
   _ quay servo theo vi tri dinh san
   _ bat coi bao khi tin hieu dat nguong   
  */
#include<p18f46k20.h>
#include <pwm.h>
#include <delays.h>


#pragma config FOSC=INTIO67,WDTEN=OFF,LVP=OFF,PBADEN=OFF,MCLRE=ON  // cau hinh dao dong noi
//char led7[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
//char led[]= {0x00,0x01,0x02,0x04,0x08,0x30};
char index,VITRI;
 unsigned int a;

#define VITRI_1	68      // set vi tri cho servo qua tinh toan trang 162 datasheet
#define VITRI_2	80      // gia tri nay chinh la duty cycle
#define VITRI_3	96     
// vd  can xung 1.5ms de quay goc 90 do, chu ki dao dong 16ms => duty cycle = 1.5/16*4(PR2+1)=96
#define VITRI_4	112
#define VITRI_5	125


void Init_UART();
void Init_Tran_UART();
void Init_Rec_UART();
void Interrupt_service();
void Display();
void Tran_UART(unsigned char ch)
	{
	while(!TXSTAbits.TRMT);
	TXREG = ch;
    }
void Osc()     // cau hinh dao dong 1Mhz
{
	OSCCONbits.IRCF2=0;
	OSCCONbits.IRCF1=1;
	OSCCONbits.IRCF0=1;
}

void Pwm_init() 
{   RCONbits.IPEN = 1; // Enable priority levels on interrupts 
    IPR1bits.RCIP = 1;
    INTCONbits.GIEH = 1;
	OpenTimer2(0x06);  // chon gia tri 16 prescale cho timer2
	OpenPWM2(0xff);	   // set gia tri thanh ghi PR2 la 0xff
}

void main()
{  TRISB=0x00;
   TRISA=0x00;
   TRISDbits.TRISD7=0; // chan dieu khien coi bao
   
   a=0;
   index=0x03; 
   Init_UART();
   Init_Tran_UART();
   Init_Rec_UART();
 
   Osc();
   Pwm_init();
  
   VITRI=VITRI_3;
   //putrsUSART ((const far rom char*)"\r\nNguy hiem muc 3\r\n");
	

	while(1)
	{
    LATAbits.LATA0= (index );// lay 4 bit chua gia tri cua index truyen sang 7447, tranh chan ngat int0
    LATBbits.LATB1= (index >>1);
    LATBbits.LATB2= (index >>2);
    LATBbits.LATB3= (index >>3);
    LATDbits.LATD7= a;
    SetDCPWM2(VITRI);
    }
    while(1);
}
#pragma code interrupt_vector=0x08
void interrupt_vector()
{
	_asm
	goto Interrupt_service
	_endasm
}
#pragma interrupt Interrupt_service
void Interrupt_service()
{
	Display();  // ham phuc vu ngat
	PIR1bits.RCIF = 0; // xoa co ngat nhan uart
	
}

void Init_UART(void)
{   
	BAUDCONbits.BRG16 =1;//16-bit Baud Rate Generator is used (SPBRGH:SPBRG)
	SPBRGH:SPBRG = 12;// tra bang 18-5 toc do baurate 19200
	TXSTAbits.SYNC =0;
	TXSTAbits.BRGH =1;
	
    RCSTAbits.SPEN=1;  // cho phep noi tiep voi cac cong o che do uart
    TRISCbits.TRISC6=0;// RC6 la chan rx
	TRISCbits.TRISC7=1;// RC7 la chan tx
	
	                   
	

}
void Init_Tran_UART(void) 
{
    BAUDCONbits.CKTXP=0; // Idle state for transmit (TX) is high
    TXSTAbits.TXEN =1;   // cho phep truyen uart
    TXREG = 0x00;        // lam rong thanh ghi truyen
}
void Init_Rec_UART(void)
{
	PIE1bits.RCIE=1;     // cho phep ngat khi nhan uart
	INTCONbits.GIE=1;    // cho phep ngat toan cuc khi IPEN =1 set boi thanh ghi RCON
	INTCONbits.PEIE=1;   // cho phep ngat ngoai vi
	RCSTAbits.CREN =1;   // cho phep nhan uart khong dong bo
}

void Display()
{  
	char data=RCREG;
//	TXREG=data;
	
	switch(data)
	{
	case 0b10000001:    // 
    //  case '1':  
		index=0b0001;
	//	LATDbits.LATD7=0; 
	    a=0  ;
        VITRI=VITRI_5;	// servo quay nguoc	neu quay xuoi doi vi tri 5 voi 1 doi xung
      
        Tran_UART(0b10000001);    // tra lai gia tri cho node zigbee
        while (!PIR1bits.TXIF);

		break;
		
   	case 0b10000010:
        //	case '2':  
	    index=0b0010;
	    //LATDbits.LATD7=0;
	    a=0;
	    VITRI=VITRI_4;
    	//  Delay1KTCYx(1);	
	    Tran_UART(0b10000010);
	    while (!PIR1bits.TXIF);
	   	break;
	   	
	case 0b10000011:
	    //case '3':  
	    index=0b0011;
	    //LATDbits.LATD7=0;
        a=0;
        VITRI=VITRI_3;
     	//  Delay1KTCYx(1);	
	    Tran_UART(0b10000011);
	    while (!PIR1bits.TXIF);
		break;

	case 0b10000100:
    	//case '4':  
  	    index=0b0100;
  	    //LATDbits.LATD7=0;
  	    a=0;
  	    VITRI=VITRI_2;	
     	//  Delay1KTCYx(1);
     	Tran_UART(0b10000100);
     	while (!PIR1bits.TXIF);
    	//	putrsUSART ((const far rom char*)"\r\nNguy hiem muc 4\r\n");
		break;	
	
	case 0b10000101:
    	//case '5':  
	    index=0b0101;
	    a=1;
	    //LATDbits.LATD7=1;
        VITRI=VITRI_1;
    	//  Delay1KTCYx(1);
	    Tran_UART(0b10000101);
	   	while (!PIR1bits.TXIF);  
		break;
	
	default:  Tran_UART(0b10101010); // tin hieu tra lai bao loi 
		break;

		

	}
}