/* 
 * File:   ADC.h
 * Author: Mosta
 *
 * Created on February 29, 2020, 2:45 PM
 */

#ifndef _ADC_H_
#define	_ADC_H_

extern unsigned char Temp[5];//Zeros {X00°C,0X0°C,00X°C,°,C}

void  ADC_Read(unsigned char *Temp);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

