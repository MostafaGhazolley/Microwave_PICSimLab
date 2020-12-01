/******************
 * in PICGenios Kit the R/W Pin is set to ground (as write only to LCD)...

 * sending a character or a command to the LCD devise 
 * PORTS used are D (for data)& E(pin 1 and 2 for EN and RS respectively)....

******************/


#include "LCD.h"




void send_a_command (unsigned char command)
{
    PORTD=command;
    LCD_RS = 0;   
    LCD_EN = 1;   
    __delay_ms(10);
    LCD_EN = 0;   
    PORTD &=0;
}

void send_a_character (unsigned char character)
{
    PORTD=character;
    LCD_EN = 1;     
    LCD_RS = 1;     
    __delay_ms(10);
    LCD_EN = 0;  
    LCD_RS = 0;
    PORTD &=0;
  
}

void Move_Cursor_R(unsigned char NumShiftingR){
    unsigned char i;
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
      //Previous lines are to set the Pins for the LCD as outputs in case of changing...
    __delay_ms(60);
    
    send_a_command(0x0F); //display ON & Blinking On & Cursor On
    for(i=0;i<NumShiftingR;i++){
    send_a_command(0x14); //shift cursor to the right 
    }
    send_a_command(0x0F); //display ON & Blinking On & Cursor On
    __delay_ms(80);
}

void Move_Cursor_L(unsigned char NumShiftingL){
    unsigned char i;
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
    //Previous lines are to set the Pins for the LCD as outputs in case of changing...
    __delay_ms(60);
    send_a_command(0x0F);//display ON & Blinking On & Cursor On
    for(i=0;i<NumShiftingL;i++){
    send_a_command(0x10);//shift cursor to the Left
    }
    send_a_command(0x0F);//display ON & Blinking On & Cursor On
    __delay_ms(80);
}
 
void Move_Cursor_To(unsigned char L1,unsigned char L2){
    TRISD  &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
    //Previous lines are to set the Pins for the LCD as outputs in case of changing...
    __delay_ms(60);
    if(L1==0){
    send_a_command(0xC0+L2);
    }
    if(L2==0){
    send_a_command(0x38+L1);
    }
     send_a_command(0x0C);// Cursor off/Blinking off/Display On!
    __delay_ms(50);
    L1=0,L2=0;
}

void LCD_Start(void){
    unsigned char a[16] = "Welcome!       ." ;//First row one LCD
    unsigned char b[16] = "Microwave  P1.3M" ;//Second Row on LCD
    unsigned char i =0;
    send_a_command(0x01);   /* Sending all Clear Command */
    send_a_command(0x0E);   /* Set Screen and cursor ON! */
    __delay_ms(10);

   // First Line!...
    for (i=0; (i<=15) && (*(a+i)!='\0');i++){
        send_a_character(a[i]);
    }
          send_a_command(0xC0); //cursor go to Second line...  
     for (i=0; (i<=15) && (*(b+i)!='\0');i++){
        send_a_character(b[i]);
    }
}

void lcd_init(void)
{   
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &= 0;
    TRISE2 &= 0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
    __delay_ms(20);
	send_a_command(0x08);   //Display Off Cursor Off (Clearing display without clearing DDRAM Content)
	send_a_command(0x0F);   //Display blinking Cursor
	send_a_command(0x01);   //Clear Display (also Clear all DDRAM content)
	send_a_command(0x38);   //16*2 Line LCD
    send_a_command(0x80);   //Set Display Buffer RAM address 
    __delay_ms(3);
}

void LCD_Print_Hex(unsigned char KEY){
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
 
    __delay_ms(3);
    //send_a_command(0xC0 +1);
   
    
    if (KEY !=0xFF){ //0xFF is the default return if no key pressed...
            send_a_character(KEY);
           __delay_ms(10);
        
    }
}

void SETTIME_LCD(void){
    unsigned char a[16] = "   <SET Time>   " ;//First row one LCD
    unsigned char i =0;
    send_a_command(0x01);   /* Sending all Clear Command */
    send_a_command(0x0E);   /* Set Screen and cursor ON! */
    __delay_ms(10);

   // First Line!...
    for (i=0; (i<=15) && (*(a+i)!='\0');i++)
         {
        send_a_character(a[i]);
        }
    send_a_command(0xC0); //cursor go to Second line...  
    send_a_character(0x30);//could be written send_a_character('0');
    send_a_character(0x30);//0
    send_a_character(0x3A);//:
    send_a_character(0x30);//0
    send_a_character(0x30);//0
    Move_Cursor_R(4);
    send_a_character('M'); //M
    send_a_character('i');//i
    send_a_character('n');//n
    send_a_character(':');//:
    send_a_character('S');//S
    send_a_character('e');//e
    send_a_character('c');//c
             //Printing on the 2nd Line 00:00
    send_a_command(0xC0); //cursor go to Second line...
    send_a_command(0x0F);//display ON & Blinking On & Cursor On
    __delay_ms(60);
}



void SETHEAT_LCD(void){
    unsigned char a[16] = "  <<SET HEAT>>  " ;//First row one LCD
    unsigned char b[7] = "Pot.P1)" ;//First row one LCD
    unsigned char i =0;
    send_a_command(0x01);   /* Sending all Clear Command */
    send_a_command(0x0E);   /* Set Screen and cursor ON! */
    __delay_ms(10);

   // First Line!...
    for (i=0; (i<=15) && (*(a+i)!='\0');i++){
        
        send_a_character(a[i]);
    }
          send_a_command(0xC0+10); //cursor go to Second line...  
          for (i=0; (i<=10) && (*(b+i)!='\0');i++){
        
        send_a_character(b[i]);
    }
}

void ADC_LCD(unsigned char * ptrTemp){
    unsigned char i=0;
    static unsigned char firsttime=2; //for first time print out str[4] and str[5] which holds '°C'
    
    //Previous lines are to set the Pins for the LCD as outputs in case of changing...
    if(firsttime==2){ // delay for first time, changing ports to LCD defaults
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
        __delay_ms(70); 
    }
    send_a_command(0xC0); //start second line
    send_a_command(0x0C);// Cursor off/Blinking off/Display On!
   // First Line!...
    for (i=0; (i<(3+firsttime)) && (*(ptrTemp+i)!='\0');i++){
        
        send_a_character(ptrTemp[i]);
      
    }
       firsttime=0; // the change will be on the first 3 index of the array..
}

void PrintLINE1(unsigned char place,unsigned char length,unsigned char * ptrSTR){
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
    __delay_ms(70);
    send_a_command(0x80+place); //start second line
    send_a_command(0x0C);// Cursor off/Blinking off/Display On!
    for (; ((*(ptrSTR+place))!='\0')&&(place<length);place++){
      
        send_a_character(ptrSTR[place]);
        
    }
      
}

void PrintLINE2(unsigned char place,unsigned char length,unsigned char * ptrSTR){ //no blinking cursor on typing
    TRISD &=0x00; /* PortD are the 8-Output data pins to LCD... */
    TRISE1 &=0;
    TRISE2 &=0; /* RE2 -> Register Select , RE1 -> E ((Set pin 0,1 as output pins))...*/
    __delay_ms(70);
    send_a_command(0xC0+place); //start second line
    send_a_command(0x0C);// Cursor off/Blinking off/Display On!
    for (; ((*(ptrSTR+place))!='\0')&&(place<length);place++){
        
        send_a_character(ptrSTR[place]);
        
    }
    
}