/*******************************************

Function return the keycode of keypressed
on the Keypad. Keys are numbered as follows

    [    00    ]     [01]     [02]
    [    03    ]     [04]     [05]
    [    06    ]     [07]     [08]
    [  << (1)  ]     [10]  [ (2) >>  ]
  
    Arguments:
        None
 
    Returns:
      Any number between 0x30->0x3A (#0->9 in ASCII-table)
 *      1 -> * (on Keypad) and that will representing an arrow to the Left direction on LCD 
 *      2 -> # (on Keypad) and that will representing an arrow to the Right direction on LCD 
      
so to print out/send a hexa-decimel number to the LCD function we have to add (0x31+....) 
to the returned number (in return of the keypad function!) so we can match the output (1) in ASCII-table ....
using the Keypad 4x3 ((4R & 3C)) the last row ((# , 0 , * )) has to get special "if" conditions
in order to printout the matching of the ASCII-table... 

      (hex 0xFF) if NO key-pressed.
  
      Precondition:
      None. Can be called without any setup.
      
*******************************************/

#include "Keypad.h"
#include <xc.h>

unsigned char GetKeyPressed(void)
 {  
   
     __delay_ms(15);
    unsigned char c; // local Var. for Rows and columns of the keypad...
    
    RKEYPAD_TRIS =0x0F; //Input Rows
    CKEYPAD_TRIS =0xF8; // Output columns
        for(c=0;c<3;c++)
        {  
           PORTB &=~(1<<c);
           __delay_ms(70);
            if(!(RKEYPAD_PIN01 ))
            {   
                __delay_ms(50);
                if(!(RKEYPAD_PIN01 )){
                while(!(RKEYPAD_PIN01 ));
                return (0x31+c);
                }};    
          if(!(RKEYPAD_PIN02))
             {
               __delay_ms(50);
               if(!(RKEYPAD_PIN02)){
              while(!(RKEYPAD_PIN02));
             return (0x31+3+c);
               }};    
            if(!(RKEYPAD_PIN03 ))
                {
                 __delay_ms(50);
                  if(!(RKEYPAD_PIN03 )){
                while(!(RKEYPAD_PIN03));
                 return (0x31+6+c);
                  }} ;   
            if(!(RKEYPAD_PIN04))
                {
                 __delay_ms(50);
                 if(!(RKEYPAD_PIN04)){
                while(!(RKEYPAD_PIN04));
                if(c==0){
                return (1); // LEFT
                }
                if(c==1){
                    return (0x30);
                }
                if(c==2){
                    return (2);// RIGHT
                }
                 }}; 
          PORTB |=(1<<c);
        };
 
  return 0XFF; //Indicate No key pressed
}



