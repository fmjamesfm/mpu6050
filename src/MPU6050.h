#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <stdint.h>
#include <bcm2835.h>

#define MPU6050_DEFAULT_ADDRESS 0x68

#define REG_INT_STATUS 0x3A
#define REG_X 0x3B
#define REG_Y 0x3D
#define REG_Z 0x3F
#define REG_PWR_MGMT_1 0x6B
#define REG_USER_CTRL 0x6A
#define REG_INT 0x38
#define REG_FIFO 0x23
#define REG_FIFO_READ 0x74
#define REG_FIFO_LEN_H 0x72
#define REG_FIFO_LEN_L 0x73
#define REG_CONFIG 0x1A
#define REG_SMPRT_DIV 0x19


class MPU6050 {
  private:
  	short high, low;
    float output;
    uint8_t i2c_address = MPU6050_DEFAULT_ADDRESS;
    uint8_t int_pin;

  public:
    typedef struct {
      float ax, ay, az;
      float gx, gy, gz;
    } MPU6050_data_t;

  ~MPU6050();
  MPU6050_data_t data;
  float getReading(uint8_t reg);
  float getFIFOReading();
  void getAccelData();
  void getAccelDataFromFIFO();

  int init(uint8_t addr);
  int init();
  uint16_t getFIFOLength();
  void resetFIFO();
  void waitDataReady();
  bool isDataReady();
  void setInterruptPin(uint8_t intpin);
  void setAddress(uint8_t addr);
  void reset();
  void setInterrupt(bool);
};

#endif
