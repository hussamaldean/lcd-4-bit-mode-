//interface hitachi lcd with stm32 using bare metal
//PC4 through PC7 for 4-bit data line

//PB5 as RS pin
//PB6 as RW pin
//PB7 as enable pin
//-----------------------
#define RS 0x20
#define RW 0x40
#define EN 0x80

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
//-------------
#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
//#include "String"
void delay(int delayms);
void LCD_init(void);
void GPIO_init(void);
void display(void);
void lcd_com(unsigned char c);
void lcd_data(char c);
void setCursor(int a, int b);
void setRowOffsets(int row0, int row1, int row2, int row3);
void LCD_Disp( char *p);
void lcd_nipple_write(char data, unsigned char control);
//--------
unsigned int _displayfunction;
unsigned int _displaycontrol;
unsigned int _displaymode;
unsigned int _initialized;
unsigned int _numlines;
unsigned int _row_offsets[4];
unsigned long seconds=0;
char str[10000];
void delay(int ms);
void LCD_init(void);

int main(void)
{

delay(500);
GPIO_init();
LCD_init();	
delay(1000);
setCursor(7,0);LCD_Disp("STM32");
setCursor(5,1);LCD_Disp("Bare Metal");
setCursor(7,2);LCD_Disp("LCD2004");
//setCursor(0,3);LCD_Disp("Interface");
	while(1)
{
sprintf(str,"%lu",seconds);
seconds++;
setCursor(0,3);LCD_Disp(str);	
delay(1000);
}
}

//initialize the lcd
void LCD_init(void)
{

lcd_com(0x01);
delay(1);
lcd_com(0x02);
delay(1);
lcd_com(0x28);
delay(4);
lcd_com(0x06);
delay(1);
lcd_com(0x0c);
delay(1);
lcd_com(0x80);
delay(4);
}
//initialize the gpio pins for the lcd
void GPIO_init(void){
RCC->AHB1ENR=RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN;
GPIOB->MODER|=GPIO_MODER_MODER5_0|GPIO_MODER_MODER7_0;
GPIOB->BSRR=(RS<<16)|(EN<<16);;
GPIOC->MODER|=GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0;
}
void lcd_nipple_write(char data, unsigned char control)
{
GPIOC->BSRR|=0x00F00000; //clear data pins
GPIOC->BSRR=data&0xF0;
if(control &RS)
{
GPIOB->BSRR=RS;
}
else{GPIOB->BSRR=RS<<16;}

GPIOB->BSRR=EN;
delay(0);
GPIOB->BSRR=(EN<<16);
delay(0);
}

void delay(int ms)
{
	SysTick->LOAD=16000-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;
}
//lcd commands
void lcd_com(unsigned char c)
	{
lcd_nipple_write(c&0xF0,0); //upper data
lcd_nipple_write(c<<4,0); //lower data
if(c<4){delay(2);}
else{delay(1);}
}
// send command to the lcd
void lcd_data(char c)
	{
lcd_nipple_write(c&0xF0,RS);
lcd_nipple_write(c<<4,RS);
delay(1);
}

void setCursor(int a, int b)
{
	int i=0;
	switch(b){
	case 0:lcd_com(0x80);break;
	case 1:lcd_com(0xC0);break;
	case 2:lcd_com(0x94);break;
	case 3:lcd_com(0xd4);break;}
	for(i=0;i<a;i++)
	lcd_com(0x14);
}
void LCD_Disp( char *p)
{
	while(*p!='\0')
	{
		lcd_data(*p);
		p++; delay(1);
	}
}