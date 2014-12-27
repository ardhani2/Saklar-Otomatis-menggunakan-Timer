/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
© Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 12/24/2014
Author  : Dhani
Company : 
Comments: 


Chip type               : ATmega8535
Program type            : Application
AVR Core Clock frequency: 11.059200 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 128
*****************************************************/

#include <mega8535.h>
#include <delay.h>
#include <stdio.h>
#include <stdlib.h>

// Alphanumeric LCD functions
#include <alcd.h>

//---------------------------Variabel Global----------------------------------//

eeprom int d=0,m=0,j=0; // disimpan di eeprom agar tidak terhapus bila mati 
unsigned int cekTimer=0,detik=0,menit=0,jam=0,flagMenu=0;
unsigned char cdetik[10],cmenit[10],cjam[10];

//----------------------------- display buat led -----------------------------//
 void tampilan(){           
            lcd_gotoxy(9,0);      // posisi pointer pada LCD
            lcd_putsf(":");       // display nilai ascii-nya
            lcd_gotoxy(10,0);
            lcd_puts(cdetik);
            lcd_gotoxy(6,0);
            lcd_putsf(":");
            lcd_gotoxy(7,0);
            lcd_puts(cmenit);
            lcd_gotoxy(5,0);
            lcd_puts(cjam);
 }

// fungsi untuk merubah integer to ascii untuk didisplay ke LCD            
//------------------------------ ITOA variabel set ------------------------//
// variabel untuk display saat set timer
void itoaVarSet(){       
    itoa(d,cdetik); 
    itoa(m,cmenit);
    itoa(j,cjam);
}

//------------------------------ ITOA Variabel Count ----------------------//
// variabel 
void itoaVarCount(){
    itoa(detik,cdetik); 
    itoa(menit,cmenit);
    itoa(jam,cjam);
}
//-------------------------------Menu-------------------------------------//

void menu(){


// konfigurasi tombol b0=inc input, b1=dec input, b2=next setting, b3= COUNTDOWN MULAI

    //setTimer:
    if(flagMenu==0){        // saat flag menu = 0
        PORTA.0=0;          // mematikan sinyal pada pin a.0
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_putsf("Set Timer");   // menampilkan menu setting timer
        delay_ms(1000);
        lcd_clear();
        flagMenu=1;          // set flag menu menuju setting detik
    }
       
    //set_detik:
    while(flagMenu==1){
        lcd_clear();
        itoaVarSet();
        tampilan();           // memanggil fungsi tampilan untuk menampilkan nilai yang akan di countdown
        lcd_gotoxy(0,1);
        lcd_putsf("set detik");
        delay_ms(100);
        if(PINB.0==0){            // ketika pin b.0 ditekan logika nol active low, rangkaian berupa pull up resistor
            delay_ms(150);        // delay diberikan agar tombol memberikan nilai yang tidak kelewatan banyak ketika ditekan
            d=d+1;                // increment d
        }
        if(PINB.1==0){
            delay_ms(150);
            d=d-1;               // decrement
        }
        if(PINB.2==0){
            delay_ms(150);
            flagMenu=2;          // flag menandakan setting menit
            break;
            //goto set_menit;
        }
        if(PINB.3==0){
            delay_ms(150);
            flagMenu=4;          // flag menandakan hitung mundur dimulai
            break;
            //goto countDown;
        }
        if(d==-1){
            d=0;                 // menset nilai d nol ketika mencapai minus satu
        }
    } 
     
//set_menit:
    while(flagMenu==2){
        lcd_clear();
        itoaVarSet();
        tampilan();
        lcd_gotoxy(0,1);
        lcd_putsf("set menit");
        delay_ms(100);
        if(PINB.0==0){
            delay_ms(150);
            m=m+1;
        }
        if(PINB.1==0){
            delay_ms(150);
            m=m-1;
        }
        if(PINB.2==0){
            delay_ms(150);
            flagMenu=3;
            break;
            //goto set_jam;
        }
        if(PINB.3==0){
            delay_ms(150);
            flagMenu=4;
            break;
            //goto countDown;
        }
        if(m==-1){
            m=0;
        }
    } 
    
    //set_jam:        
    while(flagMenu==3){
        lcd_clear();
        itoaVarSet();
        tampilan();
        lcd_gotoxy(0,1);
        lcd_putsf("set jam");
        delay_ms(100);
        if(PINB.0==0){
            delay_ms(150);
            j=j+1;
        }
        if(PINB.1==0){
        delay_ms(150);
        j=j-1;
        }
        if(PINB.2==0){
           delay_ms(150);
            flagMenu=1;
            break;
        //goto set_detik;
        }
        if(PINB.3==0){
            delay_ms(150);
            flagMenu=4;
            break;
            //goto countDown;
        }
        if(j==-1){
            j=0;
            }
    }
    

           
    
//countDown:
    if(flagMenu==4){
            jam=j;            // memasukkan variabel setting ke variabel intterupt untuk dihitung mundur
            menit=m;          // penggunaan interupt untuk mendapatkan satu detik sebenarnya
            detik=d;            
            
         while(1){           // looping countdown
                          
            lcd_clear();
            itoaVarCount();   // mengubah nilai variabel intterupt timer untuk didisplay
            tampilan();
            
            lcd_gotoxy(0,1);           
            lcd_putsf(" >> Countdown");     // pemberitahuan menu countdown
            delay_ms(500);
            
            if(jam==0 && menit==0 && detik==0){     // ketika sudah habis hitung mundurnya
                lcd_clear();
                itoaVarCount();
                tampilan();         // menampilkan nilai jam menit detik yang telah bernilai nol
                
                lcd_gotoxy(0,1);
                lcd_putsf("  Waktu Habis");
                delay_ms(50);
                PORTA.0=1;          // menyalakan sinyal pada port a.0 menuju rangkaian relay      
                delay_ms(3000);
                flagMenu=5;         // menuju menu minta input lagi
                break;
                //goto mintaLagi;
            } 
         }
    }

//mintaLagi:
    while(flagMenu==5){
        lcd_clear(); 
        lcd_gotoxy(0,0);
        lcd_putsf("    Tekan B1");
        lcd_gotoxy(0,1);
        lcd_putsf("  untuk lanjut");
        delay_ms(100);
        if(PINB.1==0){             // menunggu user untuk mengulangi program dengan menekan tombol b.1
            delay_ms(100);
            flagMenu=0;
            break;
            //goto setTimer;
        }
    }
}    
    





// Timer2 output compare interrupt service routine
interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
// Place your code here
TCNT2=0;  // mereset TCNT untuk dicompare lagi
        if(++cekTimer==10000){ // 10 ribu kali untuk mendapatkan satu detik
             detik--;          // detik decrement seterusnya sampai jam
             cekTimer=0;
             if(detik==-1){
                menit--;
                detik=59;
                if(menit==-1){
                    jam--;
                    menit=59;
                }
             }              
        }            
}

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=T State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=0 
PORTA=0x00;
DDRA=0x0F;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0xF0;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 1382.400 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x02;
TCNT2=0x00;
OCR2=0x96;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x80;

// USART initialization
// USART disabled
UCSRB=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 16
lcd_init(16);

// Global enable interrupts
#asm("sei")

// Kode Utama program

PORTA.0=0;                      // meniadakan sinyal pada port a.0
lcd_gotoxy(2,0);
lcd_putsf("Saklar Timer");      // display judul alat
lcd_gotoxy(4,1);
lcd_putsf("Otomatis");
delay_ms(1500); 

while (1)
      {
      // Place your code here
      menu();                // endless loop dengan memanggil fungsi menu
      }
}
