/* 
 * File:   Project Main.c
 * Author: Mostaaa Ghazolley
 *
 * Created on February 28, 2020, 5:35 PM
 */

// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#define _XTAL_FREQ 16000000UL //for __delay_ms()...
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>
//#include <stdio.h>
#include <pic16f877a.h>
#include "LCD.c"
#include "Keypad.c"
#include "ADC.c" // .C !!!!!!!!!!!!!!!!
#include "MStatus.c" //.C Microwave Status
//#include "TimerDelay.c"//.c

unsigned char TMR1Counter=0; //Gobal TimerDelay.h var, used in ISR!
unsigned char Temp[5];//Zeros {X00°C,0X0°C,00X°C,°,C}
unsigned char digit=0; //the index of Time array, pointing where the user can modify time thro. keypad numbers/arrows
unsigned char HeatSET=0; //Flag to exit Getting heat from ADC Loop..
unsigned char TIME[5]={"00:00"};  //{(X0:00),(0X:00),:,(00:X0),(00:0X)}
static unsigned int TIMEinSec=0; // holds the time entered in sec.
extern unsigned char Key; // defined and intialized in Keypad files!
volatile unsigned char STATEF=0; //My state flag to the microwave process.

void __interrupt()  ISR_H();


void main(void) {
    
    TRISB=0x18; //pin5 and pin4 inputs PushButtons
    TRISC=0x01;
    TRISD&=0x00;
    TRISE&=0x00;
    
    lcd_init();
    LCD_Start(); // Welcome screen!
    __delay_ms(1000);
 

    while(1)
    {  
      
        while((digit<5)&&(STATEF<=1))// Setting time loop (digits from 0 to 5)
        { 
            DOORSTATE();
                if(STATEF==0){
                    SETTIME_LCD(); //LCD Setting time on first time ((change latter with 1 function to print all screens...)
                    digit=0; //Timing digits var. 00:00 
                }
            STATEF=1;//time is setting now!
            
            if(!PORTBbits.RB5){
                __delay_ms(60);
                if(!PORTBbits.RB5){
                    while(!PORTBbits.RB5){};
                        __delay_ms(100);
                        STATEF=2; // 2 means time is set!
                }
            }
        
            if(digit==2)
            { //When reaching to the ":" on the timer setting, will skip it!
                ++digit; 
                Move_Cursor_To(0,digit);
                send_a_command(0x0F);//display ON & Blinking On & Cursor On
            } 
            Key=0xFF; // initiate the Key Var. to the default ((0xFF))
            Key=GetKeyPressed(); //Get the position of pressed key...
            __delay_ms(5);
            if((Key<=0x3B) && (Key >=0x30) && (digit==3))
            { //In the 3rd digit 00:X0 Max X is 5.. (seconds)
                if(Key>=0x36)
                { 
                    TIME[digit]=0x35; 
                    Key=0x35;
                }
                else
                { 
                    TIME[digit]=Key;
                }
            digit++;
            LCD_Print_Hex(Key);
            }
            else if((Key<=0x3B) && (Key >=0x30))
            {
                TIME[digit]=Key;
                digit++;
                LCD_Print_Hex(Key);
            }
            else if (Key==1)
            { //<< Arrow to the left 
               digit--;
               if(digit==0xFF)//digit will be 0-1 =0xFF which is unsigned char! and cursor move to the last digit
                {
                   digit=4;
                   Move_Cursor_To(0,digit);
                   send_a_command(0x0F);//display ON & Blinking On & Cursor On
                } 
                else if (digit==2)// 00:00 if went to the ":" digit, cursor go to the left again!
                { 
                   digit=1;
                   Move_Cursor_To(0,digit);
                   send_a_command(0x0F);//display ON & Blinking On & Cursor On
                }  
                else // else move normally to the left
                {
                    Move_Cursor_L(1);
                } 
            }
            else if (Key==2)
            { // same as left arrow, goes with right arrow
                digit++;
                if(digit>=5)
                {
                    digit=0;
                    Move_Cursor_To(0,digit);
                    send_a_command(0x0F);//display ON & Blinking On & Cursor On
                }
                 else if(digit==2)
                 {
                     digit=3;
                     Move_Cursor_To(0,digit);
                     send_a_command(0x0F);//display ON & Blinking On & Cursor On
                  }
                 else 
                 {
                    Move_Cursor_R(1);
                 }
                }
            else{};//nothing
            
            Key=0xFF; //Reset the Keypad entry
        
            if(digit>=5)
            {
               digit=0;
               Move_Cursor_To(0,digit);
               send_a_command(0x0F);//display ON & Blinking On & Cursor On
            }
        
        } // END OF SETTING TIME ON LCD LOOP!!!
         
        
      
        while (STATEF<=3)
        {
            DOORSTATE(); //check door ((Polling))
            if(STATEF==2)
            { // means time is set and now we move to heat setting
                lcd_init();
                SETHEAT_LCD();
            }
            STATEF=3; //initialized LCD heat and now stay in loop till heat is set
            
            ADC_Read(Temp); //passing the Temp[] array to the sub. fun. filling elements with Temp digit by digit from ADC potentiometer.
            ADC_LCD(Temp); //Printing the Temp. On LCD
            
            if(!PORTBbits.RB5){
                __delay_ms(60);
                if(!PORTBbits.RB5)
                {
                    while(!PORTBbits.RB5);
                    __delay_ms(100);
                    STATEF=4; // Heat is set
                }
            }
        }
        
        TIMEinSec=(((TIME[0]-0x30)*600)+((TIME[1]-0x30)*60)+((TIME[3]-0x30)*10)+((TIME[4]-0x30))); 
        //-0x30 to back from hexa to number 0>9
        if(TIMEinSec>=10)
        { // MicroWave On
        ONStatus(TIMEinSec,Temp);  //sending time in sec + Temp array!    
        }
        else if(STATEF==4) // so that when food is done, will not come here
        { //Reset again, put time again from start!
            lcd_init();
            PrintLINE1(0,13,"Time is Small");
            PrintLINE2(0,16,"<ReEnter time>");
            STATEF=0; //restart the flagging var, to 0
            __delay_ms(2000);
            __delay_ms(2000);
        }
        else{}// nothing...
                
    }// End of While 1
    
}// End of main

void __interrupt()  ISR_H() // short as possible
{
    if(TMR1IF==1)
    {
        TMR1Counter=1;   // change
        TMR1H=0x8A;     // Load the time value(0xBDC) for 40ms delay +3cycles for the instruction in between
        TMR1L=0xD0;
        TMR1IF=0;
    }
}
