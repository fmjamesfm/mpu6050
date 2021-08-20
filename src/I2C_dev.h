#ifndef _I2C_DEV_H
#define _I2C_DEV_H

#include <stdint.h>

#define I2C_DEFAULT_CLOCK_DIVIDER BCM2835_I2C_CLOCK_DIVIDER_2500

int initI2C();
uint8_t I2CReadByte();
void I2CWriteByte(uint8_t byte);
uint8_t I2CReadReg8(uint8_t reg);
void I2CWriteReg8(uint8_t reg, uint8_t data);

#endif
