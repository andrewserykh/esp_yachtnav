#ifndef nav_html_h
#define nav_html_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

//вызываются функции из html.h

html_http200(client);
html_headbegin(client);

html_headajax_nav(client);
html_headstyle(client,ZOOM);
html_headend(client);

html_bodybegin(client);
html_navdata(client,0,MODE,SOG,HDG,COG,ANCHOR,ANCHOR_DRIFT,LINKERROR,RUDDER.Current); //0 - вызов первичный, не из ajax запроса

if (MODE==MODE_SAIL) html_sail(client,AP,RUDDER.Set);
if (MODE==MODE_MOTOR) html_motor(client,AP,M_GEAR,M_THROTTLE,RUDDER.Set);
if (MODE==MODE_SETUP) html_setup(client,LINKERROR,RUDDER.ms_tmax,AP_Deadzone,AP_OutH,AP_DiffH,AP_Kzero);
if (MODE==MODE_ANCHOR) html_anchor(client,ANCHOR,GPS.lat,GPS.lng, PoiAnchor.lat, PoiAnchor.lng,ANCHOR_DRIFT_MAX);
if (MODE==MODE_NAV) html_nav(client, GPS.lat, GPS.lng,GPS.hour,GPS.minute,GPS.second);

html_menu(client);
html_bodyend(client);

#endif
