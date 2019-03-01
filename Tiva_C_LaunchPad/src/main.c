#include "inc/tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LCD_PORT                         GPIO_PORTB_BASE
#define LCD_PORT_ENABLE                  SYSCTL_PERIPH_GPIOB
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define D4                               GPIO_PIN_4
#define D5                               GPIO_PIN_5
#define D6                               GPIO_PIN_6
#define D7                               GPIO_PIN_7


#define pin0 0x01
#define pin1 0x02
#define pin2 0x04
#define pin3 0x08
#define pin4 0x10
#define pin5 0x20
#define pin6 0x40
#define pin7 0x80


int sayi1=0;
int sayi2=0;
int sayi3=0;
int sayi4=0;
int paratam = 0;
int parakurus = 0;
int yirmilik;
int onluk;
int beslik;
int birlik;
int yarimlik;
int ceyreklik;
int metelik;
int delik;
int kurusluk;


void init_port() {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // Port Anin saati aktifleştir
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // Port Dnin saati aktifleştir
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // Port Enin saati aktifleştir

	delay = SYSCTL_RCGC2_R; // Gecikme

	GPIO_PORTA_AMSEL_R = 0x00; 		// A PORTU ANALOG KULLANIMINA KAPAT
	GPIO_PORTA_PCTL_R = 0x00000000;
	GPIO_PORTA_DIR_R = 0xAB; 		// 2 4 6 İNPUT , DİĞERLERİ OUTPUT
	GPIO_PORTA_AFSEL_R = 0x00;
	GPIO_PORTA_DEN_R =0xFF; 		// DİJİTAL KULLANIM AKTİFLEŞTİR

	GPIO_PORTD_AMSEL_R = 0x00; 		// D PORTU ANALOG KULLANIMINA KAPAT
	GPIO_PORTD_PCTL_R = 0x00000000;
	GPIO_PORTD_DIR_R = 0xBB; 		// 2 6 İNPUT DİĞERLERİ OUTPUT
	GPIO_PORTD_AFSEL_R = 0x00;
	GPIO_PORTD_DEN_R =0xFF; 		// DİJİTAL KULLANIM AKTİFLEŞTİR

	GPIO_PORTE_AMSEL_R = 0x00; 		// E PORTU ANALOG KULLANIMINA KAPAT
	GPIO_PORTE_PCTL_R = 0x00000000;
	GPIO_PORTE_DIR_R = 0xEB; 		// 2 4 İNPUT DİĞERLERİ OUTPUT
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_DEN_R =0xFF; 		// DİJİTAL KULLANIM AKTİFLEŞTİR

}

//https://github.com/mtayyip/Tiva-C-Series-TM4C123G-LaunchPad-16x2-LCD/blob/master/lcdScreen/LCD.c
//internet sitesindeki kütüphaneden yararlanıldı
void Lcd_init() {	// LCD KURMA
    SysCtlPeripheralEnable(LCD_PORT_ENABLE);
    GPIOPinTypeGPIOOutput(LCD_PORT, 0xFF);
    SysCtlDelay(50000);
    GPIOPinWrite(LCD_PORT, RS,  0x00 );

    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x20 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

    Lcd_Temizle();
    Lcd_Putch(0x28,0);//fonksiyon secimi
    Lcd_Putch(0x06,0);//ekleme modu
    Lcd_Putch(0x80,0);//kursör konumu
    Lcd_Putch(0x0C,0);//kursörü kapat
    Lcd_Temizle();
}

void Lcd_Goto(char x, char y){	//	KURSOR BELİRTİLEN KONUMA GİDER
    if(x==1)
        Lcd_Putch(0x80+((y-1)%16),0);
    else
        Lcd_Putch(0xC0+((y-1)%16),0);
}

void Lcd_Temizle(){
    Lcd_Putch(0x01,0); //0X01 TEMİZLEME KOMUTU
    SysCtlDelay(10);
}

void Lcd_Puts( char* s){ //STRİNG YAZDIRMA
    while(*s)
        Lcd_Putch(*s++,1);
}

void Lcd_Putch(unsigned char c,int type) { 	// 0 İSE KOMUT , 1 İSE KARAKTER
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0xf0) );
    if(type==1)GPIOPinWrite(LCD_PORT, RS, 0x01);
    else GPIOPinWrite(LCD_PORT, RS, 0x00);
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
    if(type==1)GPIOPinWrite(LCD_PORT, RS, 0x01);
    else GPIOPinWrite(LCD_PORT, RS, 0x00);
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);
}
void hesapla(int paratam,int parakurus){
		yirmilik = paratam / 20 ;
	    paratam = paratam % 20 ;
	    if(yirmilik>0) {
	        parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(yirmilik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - yirmilik");
	        SysCtlDelay(SysCtlClockGet());
	    }

	    onluk = paratam / 10 ;
	    paratam = paratam % 10 ;
	    if(onluk>0) {
	    	parayazdir(paratam,parakurus);
	    	Lcd_Goto(2,1);
	        Lcd_sayi(onluk);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - onluk");
	        SysCtlDelay(SysCtlClockGet());
	    }

	    beslik = paratam / 5 ;
	    paratam = paratam % 5 ;
	    if(beslik>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(beslik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - beslik");
	        SysCtlDelay(SysCtlClockGet());
	    }
	    birlik = paratam / 1 ;
	    paratam = paratam % 1 ;
	    if(birlik>0) {
	    	parayazdir(paratam,parakurus);
	    	Lcd_Goto(2,1);
	        Lcd_sayi(birlik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - birlik");
	        SysCtlDelay(SysCtlClockGet());
	    }

	    yarimlik = parakurus / 50 ;
	    parakurus= parakurus % 50 ;
	    if(yarimlik>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(yarimlik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - yarimlik");
	        SysCtlDelay(SysCtlClockGet());
	    }
	    ceyreklik = parakurus / 25 ;
	    parakurus= parakurus % 25 ;
	    if(ceyreklik>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(ceyreklik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - ceyreklik");
	        SysCtlDelay(SysCtlClockGet());
	    }
	    metelik = parakurus / 10 ;
	    parakurus= parakurus % 10 ;
	    if(metelik>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(metelik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - metelik");
	        SysCtlDelay(SysCtlClockGet());
	    }
	    delik = parakurus / 5 ;
	    parakurus= parakurus % 5 ;
	    if(delik>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(delik);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - delik");
	        SysCtlDelay(SysCtlClockGet());
	    }

	    kurusluk = parakurus / 1 ;
	    parakurus= parakurus % 1 ;
	    if(kurusluk>0) {
	    	parayazdir(paratam,parakurus);
	        Lcd_Goto(2,1);
	        Lcd_sayi(kurusluk);
	        Lcd_Goto(2,2);
	        Lcd_Puts(" - kurusluk");
	        SysCtlDelay(SysCtlClockGet());
	    }
	    SysCtlDelay(SysCtlClockGet());
}
void Lcd_sayi(int number){		// TAM SAYI YAZDIRMA
	char buf[5];
	SysCtlDelay(SysCtlClockGet()/8);
	itoa(number, buf, 10);
	Lcd_Puts(buf);
}
void parayazdir(int paratam,int parakurus) {
	Lcd_Temizle();
	if(paratam!=0){
		if(paratam<10){
			Lcd_Goto(1,13);
			Lcd_sayi(paratam);
		}else{
			Lcd_Goto(1,12);
			Lcd_sayi(paratam);
		}
	}else if(paratam==0){
		Lcd_Goto(1,12);
		Lcd_Puts("  ");
	}
    Lcd_Goto(1,14);
	Lcd_Puts(".");
	if(parakurus!=0){
		if(parakurus<10){
			Lcd_Goto(1,14);
			Lcd_Puts(".0");
			Lcd_Goto(1,16);
			Lcd_sayi(parakurus);
		}else{
			Lcd_Goto(1,15);
			Lcd_sayi(parakurus);
		}
	}else if(parakurus==0){
		Lcd_Goto(1,14);
		Lcd_Puts("  ");
	}
}

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    init_port();
    Lcd_init();

    while(1){
    	if (!(GPIO_PORTA_DATA_R & pin2) == 0) { 			//BUTON1	ONLAR BASAMAĞI
    	    	GPIO_PORTA_DATA_R |= pin3;					//LED1

    	    	sayi1=(sayi1+1)%10;
    	    	Lcd_Goto(1,12);
    	    	Lcd_sayi(sayi1);

    			GPIO_PORTA_DATA_R &= ~pin3;

  		}
		else if (!(GPIO_PORTA_DATA_R & pin4) == 0) {		//BUTON2	BİRLER BASAMAĞI
				GPIO_PORTA_DATA_R |= pin5;					//LED2

				sayi2=(sayi2+1)%10;
				Lcd_Goto(1,13);
				Lcd_sayi(sayi2);

				GPIO_PORTA_DATA_R &= ~pin5;
    	}
 		else if (!(GPIO_PORTA_DATA_R & pin6) == 0) {		//BUTON3 	NOKTA BUTONU
    	   		GPIO_PORTA_DATA_R |= pin7;					//LED3

    	   		Lcd_Goto(1,14);
    	   		Lcd_Puts(".");

    	    	GPIO_PORTA_DATA_R &= ~pin7;
    	}
    	else if (!(GPIO_PORTD_DATA_R & pin2) == 0) {		//BUTON4	ONDABİRLER BASAMAĞI
    	    	GPIO_PORTD_DATA_R |= pin3;					//LED4

    	    	sayi3=(sayi3+1)%10;
    	    	Lcd_Goto(1,15);
    	    	Lcd_sayi(sayi3);

    	    	GPIO_PORTD_DATA_R &= ~pin3;
    	}
    	else if (!(GPIO_PORTD_DATA_R & pin6) == 0) {		//BUTON5	YUZDEBİRLER BASAMAĞI
    	    	GPIO_PORTE_DATA_R |= pin1;					//LED5

    	    	sayi4=(sayi4+1)%10;
    	    	Lcd_Goto(1,16);
    	    	Lcd_sayi(sayi4);

    	    	GPIO_PORTE_DATA_R &= ~pin1;
    	}
    	else if (!(GPIO_PORTE_DATA_R & pin2) == 0) {		//BUTON6	HESAPLAMA BUTONU
    	    	GPIO_PORTE_DATA_R |= pin3;					//LED6

    	    	 paratam = (sayi1*10)+(sayi2*1);
    	    	 parakurus = (sayi3*10)+(sayi4*1);
    	    	 parayazdir(paratam,parakurus);
    	    	 SysCtlDelay(SysCtlClockGet());
    	    	 hesapla(paratam,parakurus);


    	    	GPIO_PORTE_DATA_R &= ~pin3;
    	}
    	else if (!(GPIO_PORTE_DATA_R & pin4) == 0) {		//BUTON7	RESETLEME BUTONU
    	    	GPIO_PORTE_DATA_R |= pin5;					//LED7

    	    	sayi1=0;
    	    	sayi2=0;
    	    	sayi3=0;
    	    	sayi4=0;

    	    	Lcd_Temizle();

    	    	GPIO_PORTE_DATA_R &= ~pin5;
    	}
    }
}
