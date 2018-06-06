/** \file lcd.c
 * \brief Enables LCD functionality in 4-bit mode
 *
 * This file contains all of the functions required to operate the lcd in 4-bit
 * mode. Note, none of the reading is able to work.
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#include "lcd.h"

void writeCommand(uint8_t cmd)
{
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    P4->OUT = cmd & (0xf0); //set output port to cmd
    delay_us(4, sysFreq); //delay for 4 microseconds

    P3->OUT &= ~EN; //clear enable
    P4->OUT = cmd & (0x00); //set output port to cmd
    delay_us(4, sysFreq); //enable pulsewidth

//  cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    P4->OUT = (cmd<<4); //set output port to cmd
    delay_us(4, sysFreq); //delay for 4 microseconds
    P3->OUT &= ~EN; //clear enable
    P4->OUT = 0x00; //reset output
    delay_us(1000, sysFreq); //delay for 1000 microseconds
    //sends a command to the lcd
}

void clearDisplay()
{
    writeCommand(CLEAR_DISP);//clears display
}

void returnHome()
{
    writeCommand(RET_HOME);//returns home
}

void setEntryMode(uint32_t direction, uint32_t dispShift)
{
    writeCommand(ENTRY_MODE_SET | direction | dispShift);
    //sets entry mode based off of parameters
}

void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink)
{
    writeCommand(DISP_CTRL | disp | cursor | cursorBlink);
    //sets display and cursor on settings
}

void shift(uint32_t cursorShift, uint32_t direction)
{
    writeCommand(CURS_DISP_SHFT | cursorShift | direction);
    //shifts cursor or display
}

void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font)
{
    writeCommand(FUNCSET | dataLen | numLines | font);
    //sets display parameters
}

void setCGRAM(uint32_t address)
{
    writeCommand(CGRAM | address);
    //sets CGRAM address
}

void setDDRAM(uint32_t address)
{
    writeCommand(DDRAM | address);
    //sets DDRAM address
}

uint8_t checkBusy()
{
    uint8_t busy;                       //A busy flag
    P4->DIR &= ~(BIT7);                 //Setting DB7 to read
    P3->OUT |= RW;                      //Enable RW
    busy = readData() & ((uint8_t)BIT7);//read data and mask only DB7
    return busy;
}

void writeData(uint32_t data)
{
    P3->OUT |= RS;      //Enable RS
    writeCommand(data); //Send data to LCD
    P3->OUT &= ~RS;     //Clear RS
}

uint8_t readData()
{
    int data;
    P4->DIR &= 0x0f;//setting all pins to read
    P3->OUT = RS|RW;//setting RS and RW to read
    P4->OUT = 0x00; //reset output
    P3->OUT |= EN; //set enable
    delay_us(1, sysFreq);
    data = P4->OUT; //set output port to cmd
    delay_us(4, sysFreq); //enable pulsewidth

    P3->OUT &= ~EN; //clear enable
    data = data << 4;
    delay_us(4, sysFreq); //enable pulsewidth

//    cmd = cmd<<4; //shift over by 4 bits
    P3->OUT |= EN; //set enable
    delay_us(1, sysFreq);
    data |= P4->OUT;
    delay_us(2, sysFreq); //enable pulsewidth
    P3->OUT &= ~EN; //clear enable
    return data;
}

void halfBitInit()
{
    P3->DIR |= (RS|RW|EN);          //set RS, RW, EN as outputs
    P4->DIR = 0xf0;                 //set the lcd data pins as outputs
    P3->OUT &= ~(RS|RW|EN);         //clear RS,RW,EN
    //sending func set, in 8 bit mode
    delay_ms(50, sysFreq);   //delay of 50ms
    P4->OUT = 0x00;                 //reset output
    P3->OUT |= EN;                  //set enable
    P4->OUT = FUNCSET | ENABLE4BIT; //send functionset with 4 bit mode
    delay_us(4, sysFreq);    //delay for 4 microseconds
    P3->OUT &= ~EN;                 //clear enable
    P4->OUT = (0x00);               //clear data pins
    delay_us(4, sysFreq);    //delay for 4 microseconds
    //resending func set, which needs to be sent twice for some reason
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    dispOnSet(DISPLAY_ON, CURSOR_OFF, CURSOR_NO_BLINK); //set display settings
    clearDisplay();                 //clear the display
    delay_ms(3, sysFreq);    //delay for 3 ms
    setEntryMode(CURSOR_INCREMENT_RIGHT, 0);
    //set the entry mode with the cursor incrementing right and no display
    //shift
    delay_ms(1, sysFreq);    //delay for 1 ms
}

void writeString(char* string)//takes in a pointer to a string
{
    //TODO:Add in location select to start the string
    while(*string)  //dereferences pointer and checks if null char
                    //loop continues while the char is not null
    {
        writeData(*string);//send in character at string address
        string++;//increment the pointer address
    }
    return;
}

void rowShiftDown(){
    setDDRAM(ROW_SHFT); //change the DDRAM address to be second row,
                        //first column
}
