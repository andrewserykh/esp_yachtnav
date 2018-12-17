/*
* - Класс для работы с устройствами I2C на ESP32
*/

#ifndef i2cdev_h
#define i2cdev_h

#include "Wire.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define HMC5883_ADDR 0x0D

#define Mode_Standby    0b00000000
#define Mode_Continuous 0b00000001

#define ODR_10Hz        0b00000000
#define ODR_50Hz        0b00000100
#define ODR_100Hz       0b00001000
#define ODR_200Hz       0b00001100

#define RNG_2G          0b00000000
#define RNG_8G          0b00010000

#define OSR_512         0b00000000
#define OSR_256         0b01000000
#define OSR_128         0b10000000
#define OSR_64          0b11000000

class i2cdev
{
public:
  bool error;
  void compassBegin();
  float getHeading();  
  int16_t compassX;
  int16_t compassY;
  int16_t compassZ;
private:
  TwoWire compass=TwoWire(1); 
  void writeReg(byte Reg,byte val);
  void setMode(uint8_t mode,uint8_t odr,uint8_t rng,uint8_t osr);  

};

#endif // #ifndef i2cdev_h
