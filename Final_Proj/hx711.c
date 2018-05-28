/*
 * hx711.c
 *
 *  Created on: May 24, 2018
 *      Author: Jason Zhou
 */

#include "hx711.h"

static uint32_t OFFSET = 0;
static float SCALE = 1;

void initHX711(void)
{
    HX711_STRUCT->DIR &= ~HX711_DO;     //set data in to input
    HX711_STRUCT->DIR |= HX711_CLK;     //set clock out to output
    HX711_STRUCT->OUT &= ~HX711_CLK;    //clear clock out
}

void powerUp(void)
{
    HX711_STRUCT->OUT &= ~HX711_CLK;
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
    while(HX711_STRUCT->IN & HX711_DO);    //block loop until HX711 is ready to send
    for(i=0; i<24; i++)                     //define loop to check data with gain of 128
    {
        HX711_STRUCT->OUT |= HX711_CLK;     //set clock high to output data
        count = count << 1;                 //shift count
        HX711_STRUCT->OUT &= ~HX711_CLK;    //set clock low to finish output data instruction
        if(HX711_STRUCT->IN & HX711_DO)
            count++;                        //increment data variable if data input was high
    }
    HX711_STRUCT->OUT |= HX711_CLK;         //25th clock cycle to end data transmission
    count = count ^ 0x800000;               //two's complement conversion I think?
    HX711_STRUCT->OUT &= ~HX711_CLK;
    return count;
}

uint32_t readAverage(uint8_t times)
{
    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < times; i++)
    {
        sum += readCount();
    }
    return sum / times;
}

void tare(uint8_t times)
{
    uint32_t sum = readAverage(times);
    setOffset(sum);
}

uint32_t getValue(uint8_t times)
{
    return readAverage(times) - OFFSET;
}

float getUnits(uint8_t times)
{
    return getValue(times) / SCALE;
}

void setScale(float scale)
{
    SCALE = scale;
}

float getScale(void)
{
    return SCALE;
}

void setOffset(uint32_t offset)
{
    OFFSET = offset;
}

uint32_t getOffset(void)
{
    return OFFSET;
}
