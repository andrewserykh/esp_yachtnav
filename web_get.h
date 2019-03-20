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
            prefs.begin("setup", false);
            prefs.putUInt("cog", (int)COG);
            prefs.end();                        
          }
          else if (strstr(linebuf,"GET /dir10m") > 0){
            COG=COG-10;
            if (COG<0) COG=360-(COG*-1);
            prefs.begin("setup", false);
            prefs.putUInt("cog", (int)COG);
            prefs.end();                                    
          }
          else if (strstr(linebuf,"GET /dir1p") > 0){
            COG=COG+1;
            if (COG>360) COG=0;
            prefs.begin("setup", false);
            prefs.putUInt("cog", (int)COG);
            prefs.end();                                    
          }
          else if (strstr(linebuf,"GET /dir10p") > 0){
            COG=COG+10;
            if (COG>360) COG=0+(360-COG);
            prefs.begin("setup", false);
            prefs.putUInt("cog", (int)COG);
            prefs.end();                                    
          }
          else if (strstr(linebuf,"GET /SETcog") > 0){
            COG=HDG;
            prefs.begin("setup", false);
            prefs.putUInt("cog", (int)COG);
            prefs.end();                                    
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
            html_http200ajax(client);
            html_navdata(client,1,MODE,SOG,HDG,COG,GPS.hour,GPS.minute,GPS.second); //1-вызов из ajax
          }
          else if (strstr(linebuf,"GET /mode_motor") > 0){
            MODE=MODE_MOTOR;
            prefs.begin("setup", false);
            prefs.putUInt("mode", MODE);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /mode_sail") > 0){
            MODE=MODE_SAIL;
            prefs.begin("setup", false);
            prefs.putUInt("mode", MODE);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /mode_anchor") > 0){
            MODE=MODE_ANCHOR;
            prefs.begin("setup", false);
            prefs.putUInt("mode", MODE);
            prefs.end();            
          }
          else if (strstr(linebuf,"GET /mode_nav") > 0){
            MODE=MODE_NAV;
            prefs.begin("setup", false);
            prefs.putUInt("mode", MODE);
            prefs.end();           
          }
          else if (strstr(linebuf,"GET /mode_setup") > 0){
            MODE=MODE_SETUP;
            prefs.begin("setup", false);
            prefs.putUInt("mode", MODE);
            prefs.end();            
          }
          else if (strstr(linebuf,"GET /mode_ota") > 0){
            MODE=MODE_OTA;
          }
          else if (strstr(linebuf,"GET /zoom10") > 0){
            ZOOM=10;
            prefs.begin("setup", false);
            prefs.putUInt("zoom", ZOOM);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /zoom13") > 0){
            ZOOM=13;
            prefs.begin("setup", false);
            prefs.putUInt("zoom", ZOOM);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /zoom15") > 0){
            ZOOM=15;
            prefs.begin("setup", false);
            prefs.putUInt("zoom", ZOOM);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /zoom20") > 0){
            ZOOM=20;
            prefs.begin("setup", false);
            prefs.putUInt("zoom", ZOOM);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /anchorON") > 0){
            ANCHOR=true;
            prefs.begin("setup", false);
            prefs.putUInt("anchor", ANCHOR);
            prefs.end();
          }
          else if (strstr(linebuf,"GET /anchorOFF") > 0){
            ANCHOR=false;
            prefs.begin("setup", false);
            prefs.putUInt("anchor", ANCHOR);
            prefs.end();
          }
          
#endif
