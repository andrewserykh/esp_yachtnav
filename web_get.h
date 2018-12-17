#ifndef web_get_h
#define web_get_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 


          if (strstr(linebuf,"GET /spd0s") > 0){
            M_THROTTLE=0;
          }
          else if (strstr(linebuf,"GET /spd1s") > 0){
            M_THROTTLE=10;
          }
          else if (strstr(linebuf,"GET /spd2s") > 0){
            M_THROTTLE=20;
          }
          else if (strstr(linebuf,"GET /spd3s") > 0){
            M_THROTTLE=30;
          }
          else if (strstr(linebuf,"GET /spd4s") > 0){
            M_THROTTLE=40;
          }
          else if (strstr(linebuf,"GET /spd5s") > 0){
            M_THROTTLE=50;
          }
          else if (strstr(linebuf,"GET /spd6s") > 0){
            M_THROTTLE=60;
          }
          else if (strstr(linebuf,"GET /spd7s") > 0){
            M_THROTTLE=70;
          }
          else if (strstr(linebuf,"GET /spd8s") > 0){
            M_THROTTLE=80;
          }
          else if (strstr(linebuf,"GET /spd9s") > 0){
            M_THROTTLE=90;
          }
          else if (strstr(linebuf,"GET /spd100s") > 0){
            M_THROTTLE=100;
          }

          else if (strstr(linebuf,"GET /dir1m") > 0){
            COG=COG-1;
            if (COG<0) COG=360;
          }
          else if (strstr(linebuf,"GET /dir10m") > 0){
            COG=COG-10;
            if (COG<0) COG=360-(COG*-1);
          }
          else if (strstr(linebuf,"GET /dir1p") > 0){
            COG=COG+1;
            if (COG>360) COG=0;
          }
          else if (strstr(linebuf,"GET /dir10p") > 0){
            COG=COG+10;
            if (COG>360) COG=0+(360-COG);
          }
          else if (strstr(linebuf,"GET /apON") > 0){
            AP=true;
          }
          else if (strstr(linebuf,"GET /apOFF") > 0){
            AP=false;
          }
          else if (strstr(linebuf,"GET /gearN") > 0){
            M_GEAR=GEAR_N;
          }
          else if (strstr(linebuf,"GET /gearR") > 0){
            M_GEAR=GEAR_R;
          }
          else if (strstr(linebuf,"GET /gearF") > 0){
            M_GEAR=GEAR_F;
          }
          else if (strstr(linebuf,"GET /ajaxnav") > 0){ // Запрошен ajax /xmlnav      
            isAJAX=true;      
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");  
            client.println("Connection: close");
            client.println();                     
          
            client.print("<table width='100%'  style='padding:0 10vw 0 10vw;'>");
            client.println("<tr style='color:#77bbff;font-size:8vw;' align='center'>");
            client.print("<td>");
            client.print(SOG);
            client.println("</td>");
            client.print("<td id='hdg'>");
            client.print(HDG);
            client.println("<sup>o</sup></td>");
            client.print("<td>");
            client.print((int)COG);
            client.println("<sup>o</sup></td>");
            client.println("</tr><tr align='center'>");
            client.print("<td>");
            client.print(SOGkmh);
            client.print("km/h</td><td></td><td></td><td></td>");
            client.println("</tr></table>");
          }
#endif
