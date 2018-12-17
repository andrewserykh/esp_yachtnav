/*
* - Класс для работы с устройствами I2C на ESP32
*/
#include "i2cdev.h"

void i2cdev::writeReg(byte Reg,byte val){
  compass.beginTransmission(HMC5883_ADDR); //start talking
  compass.write(Reg); // Tell the HMC5883 to Continuously Measure
  compass.write(val); // Set the Register
  compass.endTransmission();
}

void i2cdev::setMode(uint8_t mode,uint8_t odr,uint8_t rng,uint8_t osr)
{
  writeReg(0x09,mode|odr|rng|osr);
}

void i2cdev::compassBegin(){
  compass.begin(21,22,400000);
  writeReg(0x0B,0x07); //0x0B,0x01
  setMode(Mode_Continuous,ODR_200Hz,RNG_8G,OSR_512);  
}

float i2cdev::getHeading(){
  uint8_t buffer[6];
  float fx,fy,fz;
  compass.beginTransmission(HMC5883_ADDR);
  compass.write(0x00);
  compass.endTransmission();

  if(compass.requestFrom(HMC5883_ADDR, 6) == 6){
    buffer[0]=compass.read();
    buffer[1]=compass.read();
    buffer[2]=compass.read();
    buffer[3]=compass.read();
    buffer[4]=compass.read();
    buffer[5]=compass.read();
  }
  compassX = (buffer[1] << 8) | buffer[0];
  compassY = (buffer[3] << 8) | buffer[2];
  compassZ = (buffer[5] << 8) | buffer[4];

  int m_Scale = 4.35;  //gauss == 8.1
  fx = compassX * m_Scale;
  fy = compassY * m_Scale;
  fz = compassZ * m_Scale;

  float heading = atan2(fy, fx);
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
  heading = heading *57.3;
  return heading;
}


