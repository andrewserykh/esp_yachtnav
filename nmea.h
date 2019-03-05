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
    String message;     //буфер приема сообщения nmea
 
    uint8_t queryCount();           //количество запросов в сообщении
    String query(uint8_t number);   //разбор входящего пакета
    bool crcValid ();               //проверка контрольной суммы пакета
    //функции отправки
    bool GPZDA (int hour,int minute,int second,int day,int month,int year); //создание сообщения Время и дата $GPZDA, hhmmss.s, xx, xx, xxxx, xx, xx  *hh <CR><LF> 
  //private:
    int _crc (String message);   //расчет контрольной суммы
    String _zerohead (int number); //перевод числа в число с ведущим нулем ( 1 -> 01 )
};

#endif // #ifndef modbusrtu_h
