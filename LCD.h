/* 
 * File:   ONSatus.h
 * Author: Mosta
 *
 * Created on March 1, 2020, 2:37 PM
 */

#ifndef _LCD_H_
#define	_LCD_H_

#include <xc.h>
#include <pic16f877a.h>


#define LCD_EN  PORTEbits.RE1   //LCD Enable Pin.
#define LCD_RS  PORTEbits.RE2   //LCD Register Select Pin.



void send_a_command (unsigned char command);  // LCD Commands..
void send_a_character(unsigned char character);  //LCD outPut a charcter...

void Move_Cursor_L(unsigned char); // it shows the steps of moving to the left
void Move_Cursor_R(unsigned char); // it shows the steps of moving to the Right
void Move_Cursor_To(unsigned char,unsigned char); //move w\o showing on LCD the steps(moving in general on Line 1 or 2 repsctievely,u can choose by putting # to the required line)

void LCD_Start(void);
void SETTIME_LCD(void);
void SETHEAT_LCD(void);
void LCD_Print_Hex(unsigned char);
void lcd_init(void);


void PrintLINE1(unsigned char,unsigned char,unsigned char *); //place - Lenght  String...
void PrintLINE2(unsigned char,unsigned char,unsigned char *);


void ADC_LCD(unsigned char *);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

