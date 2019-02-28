#include "SSD1306.h"

#ifndef display_content_h
#define display_content_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

void display_DEBUG(){
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.display();
}

void display_WIFI(){
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawXbm(0, 2, 8, 8, icoWifi);
  display.drawString(9,0,ssid);
  display.drawXbm(0, 2+10, 8, 8, icoPassword);
  display.drawString(9,10,password);
  display.drawXbm(0, 2+10+10, 8, 8, icoClient);
  display.drawString(9,10+10,String(WifiClientsCount));
  display.drawString(0,10+10+10,"UTC "+String(GPS_H)+":"+String(GPS_M)+":"+String(GPS_S) );
  display.display();
}

void display_NAV(){
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,0,"HDG");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0,8,String((int)HDG));

    display.setFont(ArialMT_Plain_10);
    display.drawString(64,0,"COG");
    display.setFont(ArialMT_Plain_24);
    display.drawString(64,8,String((int)COG));

    display.setFont(ArialMT_Plain_10);
    display.drawString(0,32,"SOG");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0,32+8,String(SOG));

    display.setFont(ArialMT_Plain_10);
    display.drawString(64,32,"THR");
    display.setFont(ArialMT_Plain_24);
    display.drawString(64,32+8,String(M_THROTTLE));

    display.setFont(ArialMT_Plain_24);
    if (M_GEAR==GEAR_N) display.drawString(112,32+8,"N");
    if (M_GEAR==GEAR_R) display.drawString(112,32+8,"R");
    if (M_GEAR==GEAR_F) display.drawString(112,32+8,"F");

    display.setFont(ArialMT_Plain_16);
    display.setColor(WHITE);
    display.drawRect(104, 0, 24, 18);    
    if (AP) display.drawString(105,0,"AP");

    display.display();
}



#endif
