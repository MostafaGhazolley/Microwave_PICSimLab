

#ifndef _KEYPAD_H_
#define	_KEYPAD_H_


#define _CONCAT(a,b) a##b

#define PORT(x) _CONCAT(PORT,x)
#define TRIS(x) _CONCAT(TRIS,x)


#define RKEYPAD D  //KEYPAD IS ROWS ATTACHED ON PORTD (RD0,RD1,RD2,RD3) ((#4 Pins))..
#define CKEYPAD B  //KEYPAD IS COLUMS ATTACHED ON PORTB (RB0,RB1,RB2) ((3# Pins))..
#define RKEYPAD_PORT PORT(RKEYPAD)
#define CKEYPAD_PORT PORT(CKEYPAD)
#define CKEYPAD_TRIS   TRIS(CKEYPAD) 
#define RKEYPAD_TRIS   TRIS(RKEYPAD)

#define RKEYPAD_PIN01 PORTDbits.RD3
#define RKEYPAD_PIN02 PORTDbits.RD2
#define RKEYPAD_PIN03 PORTDbits.RD1
#define RKEYPAD_PIN04 PORTDbits.RD0

unsigned char GetKeyPressed(void);

/*
#ifndef Key // need to be deleteed!!!!!!!!!!!!!!!!!!!!!! and check after it!!!
unsigned char Key=0xFF;
#endif
*/


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */


#ifndef Key // need to be deleteed!!!!!!!!!!!!!!!!!!!!!! and check after it!!!
unsigned char Key=0xFF;
#endif