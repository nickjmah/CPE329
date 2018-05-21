#include "msp.h"
#include "dco.h"
#include "i2c.h"
#include "freq.h"
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
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
