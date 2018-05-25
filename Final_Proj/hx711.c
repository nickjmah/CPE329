/*
 * hx711.c
 *
 *  Created on: May 24, 2018
 *      Author: Jason Zhou
 */

#include "hx711.h"

void initHX711(void)
{
    HX711_STRUCT->DIR &= ~HX711_DO;     //set data in to input
    HX711_STRUCT->DIR |= HX711_CLK;     //set clock out to output
    HX711_STRUCT->OUT &= ~HX711_CLK;    //clear clock out
}

void powerDown(void)
{
    HX711_STRUCT->OUT |= HX711_CLK;     //hold clock out high (for at least 60us)
}

uint32_t readCount(void)
{
    uint32_t count;                         //data input variable
    uint8_t i;                              //init iterator for the loop
//    HX711_STRUCT->IN |= HX711_DO; //this is probably wrong and might not need to be here
    HX711_STRUCT->OUT &= ~HX711_CLK;        //set clock low for normal operation
    count = 0;                              //clear data variable
    while(HX711_STRUCT->IN && HX711_DO);    //block loop until HX711 is ready to send
    for(i=0; i<24; i++)                     //define loop to check data with gain of 128
    {
        HX711_STRUCT->OUT |= HX711_CLK;     //set clock high to output data
        count = count << 1;                 //shift count
        HX711_STRUCT->OUT &= ~HX711_CLK;    //set clock low to finish output data instruction
        if(HX711_STRUCT->IN && HX711_DO)
            count++;                        //increment data variable if data input was high
    }
    HX711_STRUCT->OUT |= HX711_CLK;         //25th clock cycle to end data transmission
    count = count ^ 0x800000;               //two's complement conversion I think?
    HX711_STRUCT->OUT &= ~HX711_CLK;
    return count;
}



