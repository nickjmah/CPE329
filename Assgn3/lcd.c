/*
 * lcd.c
 *
 *  Created on: Apr 8, 2018
 *      Author: Nick
 */

#include "lcd.h"

void writeCommand(uint8_t cmd)
{
    //TODO: fix all of the timings to be more reasonable
    //TODO: perform a check if busy first(warning might not be compatible with init function as it currently is)
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = cmd & (0xf0); //set output port to cmd
    delay_us(100, FREQ_48000_KHZ); //enable pulsewidth

    P3->OUT &= ~EN; //clear enable
    P4->OUT = cmd & (0x00); //set output port to cmd
    delay_us(100, FREQ_48000_KHZ); //enable pulsewidth

//    cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    P4->OUT = (cmd<<4); //set output port to cmd
    delay_us(100, FREQ_48000_KHZ); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable
    P4->OUT = 0x00; //reset output
    delay_us(1000, FREQ_48000_KHZ);
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
    return 0;
}
void writeData(uint32_t data)
{
    P3->OUT |= RS;
    writeCommand(data);
    P3->OUT &= ~RS;
}
uint32_t readData()
{
    return 0 ;
}
void halfBitInit()
{
    //sending func set, but only half
    delay_ms(150, FREQ_48000_KHZ);
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = FUNCSET | ENABLE4BIT; //set output port to cmd
    delay_us(100, FREQ_48000_KHZ); //enable pulsewidth

    P3->OUT &= ~EN; //clear enable
    P4->OUT = (0x00); //set output port to cmd
    delay_us(100, FREQ_48000_KHZ); //enable pulsewidth
    //resending func set, which needs to be sent twice for some reason
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    dispOnSet(DISPLAY_ON, CURSOR_ON, CURSOR_BLINK);

    clearDisplay();
    delay_ms(3, FREQ_48000_KHZ);
    setEntryMode(0x02, 0);
    delay_ms(1, FREQ_48000_KHZ);
}
void writeString(char string[])
{
    //TODO:Add in location select to start the string
    char character = string[0];
    int  i;
    i=0;
    while(character !='\0')
    {

        writeData(string[i]);
        i++;
        character = string[i];
    }
    return;
}

