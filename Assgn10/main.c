#include "msp.h"
#include "dco.h"
#include "i2c.h"
#include "freq.h"
#include "delay.h"
/**
 * main.c
 */

#define EEPROM_I2C_ADDR 0b1010000
#define EEPROM_READ 0
#define EEPROM_WRITE 1

uint32_t sysFreq = FREQ_48000_KHZ;
void init(void)
{
    set_DCO(sysFreq);
    initI2C();
}
void main(void)
{
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
    uint16_t memAddr = 0;
    uint8_t RxData = 0;
    uint8_t msgPacket[3] = {((memAddr & 0xFF00)>>8), (memAddr & 0x00FF),0xAA};//write to address 0, send AA
    uint8_t recAddr[2] = {(((memAddr-1) & 0xFF00)>>8),((memAddr-1) & 0x00FF)};
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	init();

	sendI2C(EEPROM_I2C_ADDR, msgPacket, sizeof(msgPacket));
	delay_ms(5, sysFreq);
	sendI2C(EEPROM_I2C_ADDR, recAddr, sizeof(recAddr));
    delay_ms(5, sysFreq);
	readI2C(EEPROM_I2C_ADDR, &RxData);
	if(RxData == 0xAA)
	{
	    P1->OUT |= BIT0;
	}
}
