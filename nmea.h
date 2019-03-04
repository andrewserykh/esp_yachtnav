/*
* - Класс обработки сообщений NMEA
*/

#ifndef nmea_h
#define nmea_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class nmea
{
  public:
    byte packet[128];   //принятый пакет (макс 96 байт)
    int packet_length;  //длина принятого пакета
    String message;     //буфер сообщения nmea

    bool GPZDA (int hour,int minute,int second,int day,int month,int year); //создание сообщения Время и дата $GPZDA, hhmmss.s, xx, xx, xxxx, xx, xx  *hh <CR><LF> 
  private:
    int _crc (String message);   //расчет контрольной суммы
    String _zerohead (int number); //перевод числа в число с ведущим нулем ( 1 -> 01 )
};

#endif // #ifndef modbusrtu_h
