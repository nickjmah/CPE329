/*
 * lcd.c
 *
 *  Created on: Apr 8, 2018
 *      Author: Nick
 */

#include "lcd.h"

void writeCommand(uint8_t cmd)
{
    uint32_t freq = FREQ_48000_KHZ;
    set_DCO(freq);


    P3->OUT &= ~(BIT5|BIT6|BIT7); //clearing enable port?
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = cmd &(0b11110000); //set output port to cmd
    delay_us(1, freq); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable

    delay_us(1, freq); //enable pulsewidth

    cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    P4->OUT = cmd; //set output port to cmd
    delay_us(1, freq); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable
    P4->OUT = 0x00; //reset output
    //sends a command to the lcd
}
void clearDisplay()
{
    writeCommand(CLEAR_DISP);
}
void returnHome()
{
    writeCommand(RET_HOME);
}
void setEntryMode(uint32_t direction, uint32_t dispShift)
{
    writeCommand(ENTRY_MODE_SET | direction | dispShift);
}
void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink)
{
    writeCommand(DISP_CTRL | disp | cursor | cursorBlink);
}
void shift(uint32_t cursorShift, uint32_t direction)
{
    writeCommand(CURS_DISP_SHFT | cursorShift | direction);
}
void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font)
{
    writeCommand(FUNCSET | dataLen | numLines | font);
}
void setCGRAM(uint32_t address)
{
    writeCommand(CGRAM | address);
}
void setDDRAM(uint32_t address)
{
    writeCommand(DDRAM | address);
}
uint32_t checkBusy()
{

}
void writeData(uint32_t data)
{

}
uint32_t readData()
{

}


