#include <xc.h>
#include <pic16f877a.h>
extern void DOORSTATE(void);
//#include "TimerDelay.h"

#ifndef _XTAL_FREQ

#define _XTAL_FREQ 16000000UL

#endif

extern volatile unsigned char STATEF;
extern unsigned char TMR1Counter;

/*
 * using timer1 16-bit functions learning purposes, and useful interrupt flag
 * that will keep me checking if the Door open or not while iam in
 * instead of using __delay_ms()...
 * 
 * Timer1 -> 16bit and max prescaling for it (8), TMRI(H&L) max value = 65536 (0->65533)
 * T1CON:	This registers is used to configure the TIMER1 Prescalar, Clock Source etc
 * TMRIH	This register holds the higher 8-bits of timer value. TMR1H and TMR1L are used in pair to increment from 0000 - FFFFh
 * TMRIL	This register holds the lower 8-bits of timer value. TMR1H and TMR1L are used in pair to increment from 0000 - FFFFh
 * PIR1	This register contains the Timer1 overflow flag(TMR1IF).
 * PIE1	This register contains the Timer1 Interrupt Enable flag(TMR1IE).
 *                                                    ( delay we need in sec *  _XTAL_FREQ )
 * the Register Value we need to set TMRI(H&L) =65535-(------------------------------------)
 *                                                    (     PreScaling value * 4           )
 * Hint* i choose internal clock from TICON reg, which will be (internal clk/4) 
 * thats why i divide the _XTAL_FREQ over (4)
 * 
 * MaxDelay=65536*(preScaler*4)/_XTAL_FREQ = 0.131072 sec (in my assumption 16M Fosc and 8 prescaler
 * 
 * if the value became negative, which would be max, than that timer can delay...
 * we could divide that delay into loop with counter++ flag, to Equaler numbers (counter value)...
 * 
 */
 
void InitTimer(void)
{
    T1CON = (1<<5) | (1<<4); // Timer1 with external freq and 8 as prescalar      
    TMR1IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    
}

void MONDelay(void)
{
     TMR1ON = 1;     //Start Timer1 
    unsigned char i=0;
     for( i=0 ; i<6 ;i++)
     {  
        
        TMR1Counter=0;
        TMR1H=0x0B;     // Load the time value(0xBDC) for 100ms delay +3cycles for the instruction in between
        TMR1L=0xDF;
                        /*Load the timer Value, (Note: Timervalue is (0xBDF) instaed of (0xBDC) as the
                       TImer1 needs 3 instruction Cycles to start incrementing TMR1 */
        while (TMR1Counter!=1){
        DOORSTATE();} //60 ms delay extra from this func //waiting the flag change from ISR >>in main
        TMR1IF=0;       // Clear timer interrupt flag, to start another cycle..
     }
    TMR1ON = 0;     //stop Timer1 
}



