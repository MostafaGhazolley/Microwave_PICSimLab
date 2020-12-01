#include "ADC.h"
#include <xc.h>
#include <pic16f877a.h>

  
  /*
   * Page 127 DATASHEET PIC 16f877a, bits config. (#bits)
   * ADCON0 REGISTER>>> clk pre.(2b):ANALOG CHANNEL Sel. (3b): Status (1b): reserved (1b):On (1)/Off (0) (1b)
   * ADCON0 Used to Turn ON the ADC, Select the Sampling Freq and also Start the conversion. 
   * ADCON1 REGISTER>>> Right/Left justifier (1b) : ...
   * ADCON1 : Used to configure the gpio pins for ADC 
   * ADRESH : Holds the higher byte of ADC result 
   * ADRESL : Holds the lower byte of ADC result 
   
   */
void  ADC_Read(unsigned char *Temp){ //take the Temp[] array address from main  (initialized by zero!)
    //Knowing that the change of the values of the array, will change in the main!
    static unsigned int ADC_Var; //holding the ADC row variable

    ADCON0=0x01; 
    ADCON1=0x02; //0000 0010
    
    __delay_ms(100);
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);
    ADC_Var= ((((ADRESH)<<2)|(ADRESL>>6))); 
        //+0x30 in order to print the number in hexa.
  ADC_Var= ((ADC_Var *( (double) 300/1023))+50);   //Scaling the value from ADC to 50->350°C
  Temp[0]=((ADC_Var%1000)/100)+0x30;  // X00°C (digit X)...
  Temp[1]=((ADC_Var%100)/10)+0x30;    // 0X0°C (digit X)...
  Temp[2]=(ADC_Var%10)+0x30;          // 00X°C (digit X)...
  Temp[3]=0b11011110; //'°';
  Temp[4]='C';
    
}



