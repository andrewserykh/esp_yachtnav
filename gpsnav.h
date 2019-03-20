/*
* - Класс для расчета навигации
*/

#ifndef gpsnav_h
#define gpsnav_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define EARTH_RADIUS 6372795 //радиус земли

class gpsnav
{
  public:
    int hour;
    int minute;
    int second;
    int day;
    int month;
    int year;
    int alt;
    float lat;
    float lng;
    float spd; //скорость в км/ч
    float hdg;

    float distance (float fA, float sA, float fB, float sB);
};

class gpspoi
{
    float lat;
    float lng;
};


#endif // #ifndef gpsnav_h
