#include "MPU6050.h"
#include "I2C_dev.h"
#include <stdio.h>
#include <iostream>

void MPU6050::reset(){
  
  short uc = I2CReadReg8(REG_PWR_MGMT_1);
  
  std::cout << std::hex << uc << "\n";
  I2CWriteReg8(REG_PWR_MGMT_1, 0x00);

  uc = I2CReadReg8(REG_USER_CTRL);
  std::cout << std::hex << uc << "\n";
  I2CWriteReg8(REG_CONFIG, 0x00);
}

void MPU6050::setInterruptPin(uint8_t intpin){
    int_pin = intpin;
    //input
    bcm2835_gpio_fsel(int_pin, BCM2835_GPIO_FSEL_INPT);
}

void MPU6050::setAddress(uint8_t addr){
  i2c_address = addr;
}

void MPU6050::getAccelData(){
	data.ax = getReading(REG_X);
	data.ay = getReading(REG_Y);
	data.az = getReading(REG_Z);
}

void MPU6050::getAccelDataFromFIFO(){
	data.ax = getFIFOReading();
	data.ay = getFIFOReading();
	data.az = getFIFOReading();
}

int MPU6050::init(){
  if (initI2C()){
      return 1;
  }
  bcm2835_i2c_setSlaveAddress(i2c_address);

  reset();

  // put accelerometer data only in FIFO
  I2CWriteReg8(REG_FIFO, 0x08);
  resetFIFO();
  I2CWriteReg8(REG_SMPRT_DIV, (uint8_t) 79); // 100 Hz 


  return 0;
}

bool MPU6050::isDataReady(){
	uint8_t reading = I2CReadReg8(REG_INT_STATUS);

	return (bool) (reading & 0x01);
}

int MPU6050::init(uint8_t addr){

  i2c_address=addr;
  return init();


}

void MPU6050::setInterrupt(bool enable){
  // enable/disable interrupt on data ready
  I2CWriteReg8(REG_INT, (uint8_t) (enable & 0x01) );
}


MPU6050::~MPU6050(){
  reset();
  bcm2835_i2c_end();
}

void MPU6050::waitDataReady(){
  int i = 0;
  
  for(i=0;i<4000000;i++){
      if(bcm2835_gpio_lev(int_pin) == 1)
          break;
  }
  
  if(i >= 4000000){
     printf("Time out ...\r\n");
  }
}

float MPU6050::getReading(uint8_t startReg){
	high  = I2CReadReg8(startReg );
	low = I2CReadReg8(startReg + 1);
	output = ((int16_t)((high << 8) | low )) / 16384.0;
	return output;
}

float MPU6050::getFIFOReading(){
	high  = I2CReadReg8(REG_FIFO_READ);
	low = I2CReadReg8(REG_FIFO_READ);
	output = ((int16_t)((high << 8) | low )) / 16384.0;
	return output;
}

uint16_t MPU6050::getFIFOLength(){
	high = I2CReadReg8(REG_FIFO_LEN_H);
	low = I2CReadReg8(REG_FIFO_LEN_L);
	return (uint16_t) (high << 8) | low;
}

void MPU6050::resetFIFO(){
		// enable & reset FIFO
		I2CWriteReg8(REG_USER_CTRL, 0b00000100);
		I2CWriteReg8(REG_USER_CTRL, 0b01000000);
}
