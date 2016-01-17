//////// Standard Header file for the PIC16F1936 device ////////////////
#device PIC16F1936
#nolist
//////// Program memory: 8192x14  Data RAM: 260  Stack: 16
//////// I/O: 25   Analog Pins: 14
//////// Data EEPROM: 256
//////// C Scratch area: 77   ID Location: 8000
//////// Fuses: LP,XT,HS,EC,NOWDT,WDT,PUT,NOPUT,PROTECT,NOPROTECT,MCLR
//////// Fuses: NOMCLR,CPD,NOCPD,BROWNOUT,BROWNOUT_SW,BROWNOUT_NOSL
//////// Fuses: NOBROWNOUT,IESO,NOIESO,FCMEN,NOFCMEN,WDT_SW,WDT_NOSL
//////// Fuses: INTRC_IO,ECL,ECM,ECH,CLKOUT,NOCLKOUT,NOWRT,WRT_EECON200
//////// Fuses: WRT_EECON1000,WRT,VCAP_A0,VCAP_A5,VCAP_A6,NOVCAP,PLL,PLL_SW
//////// Fuses: STVREN,NOSTVREN,BORV19,BORV27,LVP,NOLVP,DEBUG,NODEBUG
//////// 
////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT()
// Constants used to identify pins in the above are:

#define PIN_A0  96
#define PIN_A1  97
#define PIN_A2  98
#define PIN_A3  99
#define PIN_A4  100
#define PIN_A5  101
#define PIN_A6  102
#define PIN_A7  103

#define PIN_B0  104
#define PIN_B1  105
#define PIN_B2  106
#define PIN_B3  107
#define PIN_B4  108
#define PIN_B5  109
#define PIN_B6  110
#define PIN_B7  111

#define PIN_C0  112
#define PIN_C1  113
#define PIN_C2  114
#define PIN_C3  115
#define PIN_C4  116
#define PIN_C5  117
#define PIN_C6  118
#define PIN_C7  119

#define PIN_E3  131

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int8
#define BOOLEAN int1

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Constants returned from RESTART_CAUSE() are:
#define WDT_FROM_SLEEP  3     
#define WDT_TIMEOUT     11    
#define MCLR_FROM_SLEEP 19    
#define MCLR_FROM_RUN   27    
#define NORMAL_POWER_UP 25    
#define BROWNOUT_RESTART 26   

////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER_0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Constants used for SETUP_TIMER_0() are:
#define T0_INTERNAL   0
#define T0_EXT_L_TO_H 32
#define T0_EXT_H_TO_L 48

#define T0_DIV_1      8
#define T0_DIV_2      0
#define T0_DIV_4      1
#define T0_DIV_8      2
#define T0_DIV_16     3
#define T0_DIV_32     4
#define T0_DIV_64     5
#define T0_DIV_128    6
#define T0_DIV_256    7


#define T0_8_BIT      0     

#define RTCC_INTERNAL   0      // The following are provided for compatibility
#define RTCC_EXT_L_TO_H 32     // with older compiler versions
#define RTCC_EXT_H_TO_L 48
#define RTCC_DIV_1      8
#define RTCC_DIV_2      0
#define RTCC_DIV_4      1
#define RTCC_DIV_8      2
#define RTCC_DIV_16     3
#define RTCC_DIV_32     4
#define RTCC_DIV_64     5
#define RTCC_DIV_128    6
#define RTCC_DIV_256    7
#define RTCC_8_BIT      0     

// Constants used for SETUP_COUNTERS() are the above
// constants for the 1st param and the following for
// the 2nd param:

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT(), RESTART_WDT()
// WDT base is 1ms
//
#define WDT_ON      0x1000
#define WDT_OFF     0x2000

#define WDT_1MS     0x100
#define WDT_2MS     0x101
#define WDT_4MS     0x102
#define WDT_8MS     0x103
#define WDT_16MS    0x104
#define WDT_32MS    0x105
#define WDT_64MS    0x106
#define WDT_128MS   0x107
#define WDT_256MS   0x108
#define WDT_512MS   0x109
#define WDT_1S      0x10A
#define WDT_2S      0x10B
#define WDT_4S      0x10C
#define WDT_8S      0x10D
#define WDT_16S     0x10E
#define WDT_32S     0x10F
#define WDT_64S     0x110
#define WDT_128S    0x111
#define WDT_256S    0x112

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x05
#define T1_EXTERNAL         0x87
#define T1_EXTERNAL_SYNC    0x83
#define T1_FOSC             0x45

#define T1_CLK_OUT          8

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE           0x8000
#define T1_GATE_INVERTED  0xC000
#define T1_GATE_TOGGLE    0xA000
#define T1_GATE_SINGLE    0x9000
#define T1_GATE_TIMER0    0x8100
#define T1_GATE_COMP1     0x8200
#define T1_GATE_COMP2     0x8300


#define T1_GATE_B5          0x000
#define T1_GATE_C4          0x100

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6

////////////////////////////////////////////////////////////////// Timer 4
// Timer 4 Functions: SETUP_TIMER_4, GET_TIMER4, SET_TIMER4
// Constants used for SETUP_TIMER_4() are:
#define T4_DISABLED         0
#define T4_DIV_BY_1         4
#define T4_DIV_BY_4         5
#define T4_DIV_BY_16        6

////////////////////////////////////////////////////////////////// Timer 6
// Timer 6 Functions: SETUP_TIMER_6, GET_TIMER5, SET_TIMER5
// Constants used for SETUP_TIMER_6() are:
//      (or (via |) together constants from each group)
#define T6_DISABLED         0
#define T6_DIV_BY_1         4
#define T6_DIV_BY_4         5
#define T6_DIV_BY_16        6

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                         0
#define CCP_CAPTURE_FE                  4
#define CCP_CAPTURE_RE                  5
#define CCP_CAPTURE_DIV_4               6
#define CCP_CAPTURE_DIV_16              7
#define CCP_COMPARE_SET_ON_MATCH        8
#define CCP_COMPARE_CLR_ON_MATCH        9
#define CCP_COMPARE_INT                 0xA
#define CCP_COMPARE_RESET_TIMER         0xB
#define CCP_PWM                         0xC
#define CCP_PWM_PLUS_1                  0x1c
#define CCP_PWM_PLUS_2                  0x2c
#define CCP_PWM_PLUS_3                  0x3c
#word   CCP_1    =                      getenv("SFR:CCPR1L")
#byte   CCP_1_LOW=                      getenv("SFR:CCPR1L")
#byte   CCP_1_HIGH=                     getenv("SFR:CCPR1H")
// The following should be used with the ECCP unit only (or these in)
#define CCP_PWM_H_H                     0x0c
#define CCP_PWM_H_L                     0x0d
#define CCP_PWM_L_H                     0x0e
#define CCP_PWM_L_L                     0x0f

#define CCP_PWM_FULL_BRIDGE             0x40
#define CCP_PWM_FULL_BRIDGE_REV         0xC0
#define CCP_PWM_HALF_BRIDGE             0x80

#define CCP_SHUTDOWN_ON_COMP1           0x100000
#define CCP_SHUTDOWN_ON_COMP2           0x200000
#define CCP_SHUTDOWN_ON_COMP            0x300000
#define CCP_SHUTDOWN_ON_INT0            0x400000
#define CCP_SHUTDOWN_ON_COMP1_INT0      0x500000
#define CCP_SHUTDOWN_ON_COMP2_INT0      0x600000
#define CCP_SHUTDOWN_ON_COMP_INT0       0x700000

#define CCP_SHUTDOWN_AC_L               0x000000
#define CCP_SHUTDOWN_AC_H               0x040000
#define CCP_SHUTDOWN_AC_F               0x080000

#define CCP_SHUTDOWN_BD_L               0x000000
#define CCP_SHUTDOWN_BD_H               0x010000
#define CCP_SHUTDOWN_BD_F               0x020000

#define CCP_SHUTDOWN_RESTART            0x80000000


#define CCP_TIMER2            0x0000000
#define CCP_TIMER4            0x1000000
#define CCP_TIMER6            0x2000000

#word   CCP_2    =                      getenv("SFR:CCPR2L")
#byte   CCP_2_LOW=                      getenv("SFR:CCPR2L")
#byte   CCP_2_HIGH=                     getenv("SFR:CCPR2H")
#word   CCP_3    =                      getenv("SFR:CCPR3L")
#byte   CCP_3_LOW=                      getenv("SFR:CCPR3L")
#byte   CCP_3_HIGH=                     getenv("SFR:CCPR3H")
#word   CCP_4    =                      getenv("SFR:CCPR4L")
#byte   CCP_4_LOW=                      getenv("SFR:CCPR4L")
#byte   CCP_4_HIGH=                     getenv("SFR:CCPR4H")
#word   CCP_5    =                      getenv("SFR:CCPR5L")
#byte   CCP_5_LOW=                      getenv("SFR:CCPR5L")
#byte   CCP_5_HIGH=                     getenv("SFR:CCPR5H")

#define CCP3_B5             0x100
#define CCP2_C1             0x000
#define CCP2_B3             0x100

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IN
// Constants used in SETUP_SPI() are:
#define SPI_MASTER       0x20
#define SPI_SLAVE        0x24
#define SPI_L_TO_H       0
#define SPI_H_TO_L       0x10
#define SPI_CLK_DIV_4    0
#define SPI_CLK_DIV_16   1
#define SPI_CLK_DIV_64   2
#define SPI_CLK_T2       3
#define SPI_SS_DISABLED  1

#define SPI_SS_A5             0x000
#define SPI_SS_A0             0x100

#define SPI_SAMPLE_AT_END 0x8000
#define SPI_XMIT_L_TO_H  0x4000

////////////////////////////////////////////////////////////////// UART
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS           2
#define UART_DATA              4
////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT
// Constants used in setup_comparator() are:
#define NC_NC_NC_NC  0x00FF07
#define NC_NC  0x00FF07
#define A0_A3_A1_A2  0x30FF04
#define A0_VR_A1_VR  0x30FF02
#define A3_VR_A2_VR  0x30FF0A
#define A0_A2_A1_A2  0x30FF03
#define A0_A2_A1_A2_OUT_ON_A4_A5  0x00CF06
#define NC_NC_C1_C0_OUT_ON_A5  0x00DF05
#define NC_NC_A1_06_OUT_ON_A5  0x00DF0D
#define A3_A2_A1_A2  0x30FF09

// The following may be OR'ed with the above
#define COMP_DISABLE_WAKEUP   1
#define COMP_INVERT           0x10
#define COMP_T1_SYNC          0x1000000
#define COMP_T1_GATE          0x2000000

#bit C1OUT = 0x9C.6
#bit C2OUT = 0x9C.7

////////////////////////////////////////////////////////////////// VREF
// Constants used in setup_vref() are:
//
#define VREF_1v024 0x41  // Turn on 1.024V ref
#define VREF_2v048 0x42  // Turn on 2.048V ref
#define VREF_4v096 0x43  // Turn on 4.096V ref

////////////////////////////////////////////////////////////////// VREF
// Constants used in setup_low_volt_detect() are:
//
#define LVD_45 0x17
#define LVD_42 0x16
#define LVD_40 0x15
#define LVD_23 0x14
#define LVD_22 0x13
#define LVD_21 0x12
#define LVD_20 0x11
#define LVD_19 0x10


////////////////////////////////////////////////////////////////// DAC
// Digital to Analog Functions: SETUP_DAC(), DAC_WRITE()
// Constants used in SETUP_DAC() are:
#define DAC_OFF  0
#define DAC_VDD  0x80
#define DAC_VREF 0x81
#define DAC_VR   0x82
#define DAC_OUTPUT 0x40

////////////////////////////////////////////////////////////////// LCD
// LCD Functions: SETUP_LCD, LCD_SYMBOL, LCD_LOAD, LCD_CONTRAST
// Constants used in setup_lcd() are:

// LCD Multiplex Mode (first param)
#define LCD_STATIC         0x0000000080
#define LCD_MUX12          0x0000000081
#define LCD_MUX13          0x0000000082
#define LCD_MUX14          0x0000000083
#define LCD_DISABLED       0x0000000000

//      Any of the following may be or'ed in with the first parametter
// LCD Sleep Mode
#define LCD_HALT_IDLE      0x0000000040
#define LCD_CONTINUE_IDLE  0x0000000000   //default

// LCD Clock Source
#define LCD_FOSC           0x0000000000   //default
#define LCD_TIMER1         0x0000000004
#define LCD_LFINTOSC       0x0000000008

// LCD Waveform Type
#define LCD_TYPE_B         0x0000008000
#define LCD_TYPE_A         0x0000000000  //default

// LCD Bias Mode (MUX12 & MUX13 Only)
#define LCD_BIAS12         0x0000004000
#define LCD_BIAS13         0x0000000000  //default

// LCD Internal Reference Enable
#define LCD_REF_ENABLED    0x0000800000
#define LCD_REF_DISABLED   0x0000000000  //default

// LCD Internal Reference Source
#define LCD_REF_VDD        0x0000000000  //default
#define LCD_REF_FVR        0x0000400000

// LCD Internal Reference Ladder Idle
#define LCD_FVR_DISABLED   0x0000200000
#define LCD_FVR_IGNORE     0x0000000000 //default

// LCD Voltage Pin Enable
#define LCD_VOLTAGE3       0x0000080000
#define LCD_VOLTAGE2       0x0000040000
#define LCD_VOLTAGE1       0x0000020000
#define LCD_NO_VOLTAGE     0x0000000000 //default

// LCD Ladder A Controls
#define LCD_A_NO_POWER     0x0000000000 //default
#define LCD_A_LOW_POWER    0x0040000000
#define LCD_A_MED_POWER    0x0080000000
#define LCD_A_HIGH_POWER   0x00C0000000

// LCD Ladder B Controls
#define LCD_B_NO_POWER     0x0000000000 //default
#define LCD_B_LOW_POWER    0x0010000000
#define LCD_B_MED_POWER    0x0020000000
#define LCD_B_HIGH_POWER   0x0030000000

// LCD Ladder A Time Interval
#define LCD_B_ONLY         0x0000000000  //default
#define LCD_A_CLOCK1       0x0001000000
#define LCD_A_CLOCK2       0x0002000000
#define LCD_A_CLOCK3       0x0003000000
#define LCD_A_CLOCK4       0x0004000000
#define LCD_A_CLOCK5       0x0005000000
#define LCD_A_CLOCK6       0x0006000000
#define LCD_A_CLOCK7       0x0007000000

// Constants used in lcd_symbol() are:
#define COM0 0
#define COM1 24
#define COM2 48
#define COM3 72



////////////////////////////////////////////////////////////////// INTERNAL RC
// Constants used in setup_oscillator() are:
// First param:
#define OSC_31KHZ   0
#define OSC_31250   (2*8)
#define OSC_62KHZ   (4*8)
#define OSC_125KHZ  (5*8)
#define OSC_250KHZ  (6*8)
#define OSC_500KHZ  (7*8)
#define OSC_1MHZ    (11*8)
#define OSC_2MHZ    (12*8)
#define OSC_4MHZ    (13*8)
#define OSC_8MHZ    (14*8)
#define OSC_16MHZ   (15*8)
// The following may be OR'ed in with the above using |
#define OSC_TIMER1  1
#define OSC_INTRC   2
#define OSC_NORMAL  0
// The following may be OR'ed in with the above using |
#define OSC_PLL_ON  0x80
#define OSC_PLL_OFF 0


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC()
// Constants used for SETUP_ADC() are:
#define ADC_OFF                0              // ADC Off
#define ADC_CLOCK_DIV_2    0x100
#define ADC_CLOCK_DIV_4     0x40
#define ADC_CLOCK_DIV_8     0x10
#define ADC_CLOCK_DIV_32    0x20
#define ADC_CLOCK_DIV_16    0x50
#define ADC_CLOCK_DIV_64    0x60
#define ADC_CLOCK_INTERNAL  0x30              // Internal 2-6us

// Constants used in SETUP_ADC_PORTS() are:
#define ALL_ANALOG  0xffff0007     // A0 A1 A2 A3 A5 E0 E1 E2 B0 B1 B2 B3 B4 B5  
#define sAN13         0x20000000    // B5
#define sAN12         0x01000000    // B0
#define sAN11         0x10000000    // B4
#define sAN10         0x02000000    // B1
#define sAN9          0x08000000    // B3
#define sAN8          0x04000000    // B2
#define sAN7             0x40000    // E2  
#define sAN6             0x20000    // E1  
#define sAN5             0x10000    // E0  
#define sAN4                0x20    // A5
#define sAN3                 0x8    // A3
#define sAN2                 0x4    // A2
#define sAN1                 0x2    // A1
#define sAN0                 0x1    // A0
#define NO_ANALOGS       0x00000    // None

// One of the following may be OR'ed in with the above using |
#define VSS_VDD              0x0000        //| Range 0-Vdd
#define VSS_VREF             0x0200        //| Range 0-Vref
#define VSS_1V024            0x1200        //| Range 0-1.024V
#define VSS_2V048            0x2200        //| Range 0-2.048V
#define VSS_4V096            0x3200        //| Range 0-4.096V


// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6





////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      CLEAR_INTERRUPT(), INTERRUPT_ACTIVE(),
//                      EXT_INT_EDGE()
//
// Constants used in EXT_INT_EDGE() are:
#define L_TO_H              0x40
#define H_TO_L                 0
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0x0BC0
#define INT_RB                    0x31FF0B08
#define INT_EXT_L2H               0x50000B10
#define INT_EXT_H2L               0x60000B10
#define INT_EXT                   0x300B10
#define INT_TIMER0                0x300B20
#define INT_TIMER1                0x309101
#define INT_TIMER2                0x309102
#define INT_CCP1                  0x309104
#define INT_SSP                   0x309108
#define INT_TBE                   0x309110
#define INT_RDA                   0x309120
#define INT_AD                    0x309140
#define INT_TIMER1_GATE            0x309180
#define INT_SPARE                 0x309202
#define INT_LCD                   0x309204
#define INT_BUSCOL                0x309208
#define INT_EEPROM                0x309210
#define INT_COMP                  0x309220
#define INT_COMP2                 0x309240
#define INT_OSC_FAIL              0x309280
#define INT_TIMER4                0x309302
#define INT_TIMER6                0x309308
#define INT_CCP3                  0x309310
#define INT_CCP4                  0x309320
#define INT_CCP5                  0x309340
#define INT_CCP2                  0x309201
#define INT_RB0                   0x30010B08
#define INT_RB0_L2H               0x10010B08
#define INT_RB0_H2L               0x20010B08
#define INT_RB1                   0x30020B08
#define INT_RB1_L2H               0x10020B08
#define INT_RB1_H2L               0x20020B08
#define INT_RB2                   0x30040B08
#define INT_RB2_L2H               0x10040B08
#define INT_RB2_H2L               0x20040B08
#define INT_RB3                   0x30080B08
#define INT_RB3_L2H               0x10080B08
#define INT_RB3_H2L               0x20080B08
#define INT_RB4                   0x30100B08
#define INT_RB4_L2H               0x10100B08
#define INT_RB4_H2L               0x20100B08
#define INT_RB5                   0x30200B08
#define INT_RB5_L2H               0x10200B08
#define INT_RB5_H2L               0x20200B08
#define INT_RB6                   0x30400B08
#define INT_RB6_L2H               0x10400B08
#define INT_RB6_H2L               0x20400B08
#define INT_RB7                   0x30800B08
#define INT_RB7_L2H               0x10800B08
#define INT_RB7_H2L               0x20800B08

#list
