/*
 * lcd.c
 *
 *  Created on: Apr 8, 2018
 *      Author: Nick
 */

#include "lcd.h"

void writeCommand(uint8_t cmd, uint8_t init)
{
    //TODO: perform a check if busy first(warning might not be compatible with init function as it currently is)
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = cmd & (0xf0); //set output port to cmd
    delay_us(4, FREQ_48000_KHZ); //enable pulsewidth

    P3->OUT &= ~EN; //clear enable
    P4->OUT = cmd & (0x00); //set output port to cmd
    delay_us(4, FREQ_48000_KHZ); //enable pulsewidth

//    cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    P4->OUT = (cmd<<4); //set output port to cmd
    delay_us(4, FREQ_48000_KHZ); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable
    P4->OUT = 0x00; //reset output
    delay_us(1000, FREQ_48000_KHZ);
    //sends a command to the lcd
}
void clearDisplay()
{
    writeCommand(CLEAR_DISP, 0);//clears display
}
void returnHome()
{
    writeCommand(RET_HOME, 0);//returns home
}
void setEntryMode(uint32_t direction, uint32_t dispShift)
{
    writeCommand(ENTRY_MODE_SET | direction | dispShift, 0);//sets entry mode based off of parameters
}
void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink)
{
    writeCommand(DISP_CTRL | disp | cursor | cursorBlink, 0);
}
void shift(uint32_t cursorShift, uint32_t direction)
{
    writeCommand(CURS_DISP_SHFT | cursorShift | direction, 0);
}
void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font)
{
    writeCommand(FUNCSET | dataLen | numLines | font, 1);
}
void setCGRAM(uint32_t address)
{
    writeCommand(CGRAM | address, 0);
}
void setDDRAM(uint32_t address)
{
    writeCommand(DDRAM | address, 0);
}
uint8_t checkBusy()
{
    uint8_t busy;
    P4->DIR &= ~(BIT7);//setting DB7 to read
    P3->OUT |= RW;
    busy = readData() & ((uint8_t)BIT7);
    return busy;
}
void writeData(uint32_t data)
{
    P3->OUT |= RS;
    writeCommand(data, 0);
    P3->OUT &= ~RS;
}
uint8_t readData()
{
    int data;
    P4->DIR &= 0x0f;//setting all pins to read
    P3->OUT = RS|RW;//setting RS and RW to read
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    delay_us(1, FREQ_48000_KHZ);
    data = P4->OUT; //set output port to cmd
    delay_us(4, FREQ_48000_KHZ); //enable pulsewidth

    P3->OUT &= ~EN; //clear enable
    data = data << 4;
    delay_us(4, FREQ_48000_KHZ); //enable pulsewidth

//    cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    delay_us(1, FREQ_48000_KHZ);
    data |= P4->OUT;
    delay_us(2, FREQ_48000_KHZ); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable
    return data;
    }
void halfBitInit()
{
    P3->DIR |= (RS|RW|EN);
    P4->DIR = 0xf0;
    P3->OUT &= ~(RS|RW|EN);
    //sending func set, in 8 bit mode
    delay_ms(50, FREQ_48000_KHZ);
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = FUNCSET | ENABLE4BIT;
    delay_us(4, FREQ_48000_KHZ);

    P3->OUT &= ~EN; //clear enable
    P4->OUT = (0x00);
    delay_us(4, FREQ_48000_KHZ);
    //resending func set, which needs to be sent twice for some reason
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    dispOnSet(DISPLAY_ON, CURSOR_OFF, CURSOR_NO_BLINK);

    clearDisplay();
    delay_ms(3, FREQ_48000_KHZ);
    setEntryMode(0x02, 0);
    delay_ms(1, FREQ_48000_KHZ);
}
void writeString(char* string)//takes in a pointer to a string
{
    //TODO:Add in location select to start the string
    while(*string)
    {
        writeData(*string);//send in character at string address
        string++;//increment string address. Is this legal?
    }
    return;
}
void rowShiftDown(){
    setDDRAM(ROW_SHFT);
}
