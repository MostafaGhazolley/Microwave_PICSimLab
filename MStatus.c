#include "MStatus.h"
#include <xc.h>
#include "LCD.h"
#include "TimerDelay.c"
#include <pic16f877a.h>





void ONStatus(unsigned int Time,unsigned char * ptrTemp){
    unsigned char i =0;
    ADCON1=0x0F;
    TRISCbits.TRISC0=1;
    TRISC&=(~(1<<5)) & (~(1<<2)); // Heater and fan or outputs
     while(TIME+1!=0){
         if(STATEF<=5){
             lcd_init();
             PrintLINE1(0,10,"Microwave");
             Move_Cursor_R(2);
             for(i=0;i<5;i++){
                 send_a_character(TIME[i]);
                 }
         }
         else if(STATEF==6){
             PrintLINE1(0,11,"Microwave  ");
         }
         else{}//nothing needed!
   
    PrintLINE2(0,4,"ON! ");
    for(i=0;i<5;i++){
        send_a_character(*(ptrTemp+i));
    }
   
    DOORSTATE();
    PORTCbits.RC5=1; // Heater ON!
    PORTCbits.RC2=1; //Fan ON! Servo motor!
    InitTimer();
    for(Time+1;((Time+1)!=0)&&(DOOR==0)>0;Time--)
    {
        DOORSTATE();
        /***************
         * Time+1 so no overflow occurs
         * Minutes = Seconds%60
         * and to show first digit %10 (so the number be from 0->9)
         * 2nd digit #%100/10 *ex. 356%100 = 56/10 = 5...
        ****************/
        TIME[0]=((((Time)/60)%100)/10)+0x30;  
        TIME[1]=(((Time)/60)%10)+0x30;    
        TIME[3]=((((Time)%60)%100)/10)+0x30;         
        TIME[4]=(((Time)%60)%10)+0x30;
        MONDelay();// Microwave On Delay almost 1 sec,checking if door opened in between
        Move_Cursor_To(0,11);
        send_a_command(0x0C);// Cursor off/Blinking off/Display On!
        for(i=0;i<5;i++)
        {
            send_a_character(TIME[i]);
        }
        LampON(1);
    }
    
    if((TIME+1)!=0)
    {
        PAUSED();
    }//End of If

  
    }//End of running process of microwave
    MDONE(); // Microwave done fun.
}//End of OnStatue fun.

void PAUSED(void){
    LampON(0);
    PORTCbits.RC5=0; // Heater OFF!
    PORTCbits.RC2=0; //Fan OFF! Servo motor!
    PrintLINE1(0,13,"DOOR OPEN");
    PrintLINE2(0,16,"PAUSED!! ");
    while(DOOR)
    {
        DOORSTATE();
    }
    STATEF=6;
}

void MDONE (void){// Microwave Done!!
    LampON(0);
    unsigned char i=0;
    PORTCbits.RC5=0; // Heater OFF!
    PORTCbits.RC2=0; //Fan OFF! Servo motor!
    for(i=0;i<5;i++){
    lcd_init();
    PrintLINE1(0,15,"   FOOD READY");
    PrintLINE2(0,15,"   Bon appetit");
    __delay_ms(2000);
    }
    STATEF=0;//reset all operations
}





void DOORSTATE(void){
    TRISB4|=1;
    PORTB&=0xEF;
    TRISCbits.TRISC0=0;
    if(!PORTBbits.RB4)
    {
        __delay_ms(60);
            if(!PORTBbits.RB4){
                while(!PORTBbits.RB4){};
                    __delay_ms(100);
                    DOOR=~DOOR; 
                        if(DOOR)
                        { //Open
                            PORTCbits.RC0=1;
                        }
                        else{ // closed
                            PORTCbits.RC0=0;
                            }
            }
    }


}

void LampON(unsigned char LampF)
{
    if(LampF==1)
    {  
         TRISEbits.TRISE0=0;
         PORTEbits.RE0|=1;
    }
    else
    {

         PORTEbits.RE0&=0;
    }
}