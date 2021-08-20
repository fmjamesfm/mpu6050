#include "I2C_dev.h"
#include <bcm2835.h>
#include <stdint.h>
#include <stdio.h>


int initI2C(){
  // I2C begin if specified

    if (!bcm2835_i2c_begin())
    {
      printf("bcm2835_i2c_begin failed. Are you running as root??\n");
      return 1;
    }

  bcm2835_i2c_setClockDivider(I2C_DEFAULT_CLOCK_DIVIDER);
  return 0;
}

uint8_t I2CReadByte(){
      char data;
      bcm2835_i2c_read(&data, 1);
      return (uint8_t) data;
}

void I2CWriteByte(uint8_t byte){
  bcm2835_i2c_write((char*) &byte, 1);
}

uint8_t I2CReadReg8(uint8_t reg){
      char data;
      bcm2835_i2c_write((char*)&reg, 1);
      bcm2835_i2c_read(&data, 1);
      return (uint8_t) data;
}

void I2CWriteReg8(uint8_t reg, uint8_t data){
      uint8_t buf[2] = {reg, data};
      bcm2835_i2c_write((char*)&buf, 2);

}
