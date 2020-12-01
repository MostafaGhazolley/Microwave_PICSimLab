/* 
 * File:   ONSatus.h
 * Author: Mosta
 *
 * Created on March 1, 2020, 2:37 PM
 */

#ifndef _MSTATUS_H_
#define	_MSTATUS_H_

extern volatile unsigned char STATEF;
extern unsigned char TIME[5];// from main
unsigned char DOOR =0; //Door default state is closd!

void DOORSTATE(void);
void LampON(unsigned char);
void PAUSED(void);
void MDONE (void);
void ONStatus(unsigned int ,unsigned char * );

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MSATUS_H */

