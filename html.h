#include <WiFi.h>

#ifndef html_h
#define html_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

void html_http200(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");  
  client.println("Connection: close");
  client.println();
}

void html_http200ajax(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");  
  client.println("Connection: close");
  client.println();
}

void html_headbegin(WiFiClient client){
  client.println("<!DOCTYPE HTML><html><head>");
  //client.println("<meta http-equiv='Refresh' content='5' />");
}
void html_headend(WiFiClient client){
  client.println("</head>");
}
void html_headajax_nav(WiFiClient client){
  client.println("<script type='text/javascript'>setInterval(\"callAjax('ajaxnav', updateNav)\", 1000);</script>");
  client.println("<script>");
  client.println("function callAjax(url,cFunction){var xhttp;xhttp=new XMLHttpRequest();");
  client.println("xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){cFunction(this);}};");
  client.println("xhttp.open(\"GET\",url,true);xhttp.send();}");
  client.println("function updateNav(xhttp) {document.getElementById('navdata').innerHTML=xhttp.responseText;}");
  client.println("</script>");
}

void html_headstyle (WiFiClient client, int ZOOM){
  client.println("<style>");
  client.print("body{width:90%;max-width:1080px;margin-left:auto;margin-right:auto;font-size:");
  client.print( (float)ZOOM/10.0 );
  client.println("em;background:#000;color:#fff;font-family:Arial;fill:#fff;text-align:center;}");
  client.println("a{color:#77bbff;text-decoration:none;}");
  //client.println("a {background:#77bbff;color:#000;border-radius: 5px;border:0px;font-size:0.5em;overflow: hidden;}");
  client.println(".s{background:#77bbff;color:#000;border-radius: 5px;border:0px;font-size:0.5em;overflow: hidden;padding-top:1.5em;margin: 1px;}");
  client.println(".on {background:#aaffaa;color:#000;}");
  client.println(".red {background:#ffaaaa;color:#000;}");
  client.println(".off {background:#aaa;color:#000;}");
  client.println(".menu {background: #333333;position:fixed;bottom:0;left:0px;right:0px;text-align:center;}");
  client.println(".t0 {font-size: 0.5em;color: #aaa;}");
  client.println(".t1 {padding-top: 0.5em;font-size: 1.5em;color: #5599ff;}");
  client.println(".t3 {font-size: 2.1em;color: #88ccff;font-weight: bold;}");
  client.println("</style>");
}

void html_bodybegin (WiFiClient client){
  client.println("<body>");
}

void html_bodyend (WiFiClient client){
  client.println("</body></html>");
}

void html_menu (WiFiClient client){
  client.println("<div class='menu'>");
  client.println("<a href='mode_motor'><svg viewBox='0 0 512 512' width='1.6em' height='1.6em'><path d='M509.555,376.149c-7.765-23.087-34.933-45.887-74.538-62.556c-3.815-1.604-8.214,0.186-9.822,4.004    c-1.606,3.817,0.186,8.215,4.004,9.822c35.116,14.779,59.841,34.783,66.14,53.512c2.876,8.552,1.987,16.673-2.719,24.825    c-41.39,71.697-65.011,76.354-73.25,75.056c-10.04-1.607-18.147-14.811-22.828-37.182    c-14.347-68.562-54.064-102.655-69.759-113.757c4.056-6.081,7.251-12.778,9.417-19.922c16.855-2.129,39.774,0.444,63.898,7.276    c0.684,0.194,1.371,0.286,2.048,0.286c3.27,0,6.277-2.156,7.211-5.458c1.129-3.985-1.186-8.132-5.172-9.26    c-24.171-6.845-46.794-9.663-65.043-8.221c0.183-2.141,0.298-4.302,0.298-6.49c0-34.809-23.685-64.176-55.779-72.87    c1.275-15.261,9.239-64.427,58.775-108.756c15.559-13.922,32.491-34.746,24.793-54.896C358.215,27.966,319.313,16,251.602,16    c-15.507,0-28.489,6.738-37.542,19.485c-22.698,31.96-17.717,97.003-1.066,144.737c1.365,3.911,5.642,5.974,9.551,4.61    c3.911-1.364,5.975-5.641,4.611-9.552c-20.108-57.64-15.455-110.571-0.868-131.11C232.582,35.308,240.862,31,251.602,31    c82.779,0,98.64,18.125,101.618,25.917c3.629,9.5-3.752,23.125-20.784,38.364c-52.368,46.863-61.922,98.483-63.643,117.478    c-1.604-0.102-3.217-0.172-4.847-0.172c-5.845,0-11.529,0.69-16.995,1.953c-2.374-3.116-4.71-6.651-6.972-10.575    c-2.07-3.59-6.656-4.819-10.243-2.75c-3.588,2.069-4.819,6.656-2.75,10.244c1.673,2.9,3.395,5.626,5.151,8.181    c-25.773,12.027-43.685,38.178-43.685,68.447c0,6.649,0.873,13.098,2.494,19.245c-14.038,6.582-60.488,24.069-123.4,3.402    c-19.836-6.516-46.331-10.769-59.932,5.972c-15.929,19.606-6.839,59.285,27.018,117.931c8.802,15.249,23.08,22.97,41.269,22.97    c13.207,0,28.477-4.072,45.203-12.29c3.718-1.826,5.251-6.32,3.424-10.038c-1.826-3.719-6.319-5.249-10.038-3.425    c-15.51,7.62-29.683,11.35-41.003,10.757c-11.744-0.604-20.204-5.666-25.865-15.473c-41.393-71.702-33.627-94.502-28.367-100.976    c6.413-7.892,21.9-8.31,43.609-1.18c23.824,7.827,45.44,10.627,64.188,10.626c33.621-0.001,57.984-9.006,69.19-14.169    c3.291,6.655,7.536,12.753,12.553,18.125c-10.02,23.842-36.126,53.572-66.573,75.455c-3.364,2.417-4.131,7.103-1.713,10.467    c1.465,2.039,3.764,3.124,6.096,3.124c1.516,0,3.046-0.459,4.37-1.411c31.151-22.388,57.39-51.758,69.631-77.352    c12.277,8.642,27.221,13.738,43.341,13.738c20.774,0,39.614-8.436,53.278-22.062c12.733,8.88,51.077,40.37,64.638,105.179    c4.275,20.436,13.843,45.513,35.138,48.921c1.573,0.251,3.16,0.377,4.771,0.377c23.941-0.004,52.118-27.797,83.838-82.745    C512.496,401.325,513.824,388.84,509.555,376.149z M263.946,348.585c-33.358,0-60.496-27.14-60.496-60.499    s27.138-60.499,60.496-60.499c33.357,0,60.496,27.14,60.496,60.499S297.303,348.585,263.946,348.585z' fill='#006DF0'/></svg></a>");
  client.println("<a href='mode_sail'><svg viewBox='0 0 60 60' width='1.6em' height='1.6em'><path d='M56,48h-5H37v-1h13c0.341,0,0.659-0.174,0.843-0.462c0.184-0.287,0.208-0.648,0.064-0.958L35.944,13.279    C35.97,13.189,36,13.099,36,13V1c0-0.553-0.448-1-1-1s-1,0.447-1,1v8.083l-0.093-0.2c-0.167-0.36-0.506-0.603-0.93-0.58    c-0.397,0.01-0.752,0.253-0.903,0.622L32,9.106V5c0-0.45-0.301-0.845-0.735-0.964c-0.434-0.121-0.895,0.066-1.125,0.453l-22,37    c-0.184,0.309-0.188,0.692-0.01,1.005C8.308,42.807,8.641,43,9,43h9.146l-1.072,2.622c-0.041,0.099-0.058,0.203-0.066,0.308    c-0.002,0.025-0.002,0.049-0.001,0.074c0,0.114,0.014,0.228,0.053,0.336c0.001,0.003,0.003,0.006,0.005,0.009    c0.027,0.073,0.061,0.143,0.106,0.209c0.022,0.032,0.051,0.058,0.076,0.087c0.009,0.011,0.012,0.025,0.022,0.036    c0.007,0.007,0.016,0.009,0.023,0.016c0.085,0.087,0.183,0.156,0.293,0.206c0.022,0.01,0.043,0.017,0.065,0.025    C17.76,46.971,17.877,47,18,47h17v1H10c-0.351,0-0.677,0.185-0.857,0.485l-6,10c-0.186,0.309-0.19,0.693-0.013,1.008    C3.307,59.807,3.64,60,4,60h38c5.358,0,9.447-5.09,9.948-10H56c0.552,0,1-0.447,1-1S56.552,48,56,48z M33.057,11.806l5.013,10.822    c-3.436,7.541-4.027,8.043-9.571,12.742c-1.953,1.656-4.472,3.793-7.885,6.879L33.057,11.806z M10.758,41L30,8.639v5.36L18.963,41    H10.758z M20.559,45c4.122-3.772,7.033-6.239,9.234-8.104c5.329-4.518,6.349-5.388,9.381-11.885L48.435,45H20.559z M42,58H5.766    l1.8-3h40.221C46.355,56.767,44.329,58,42,58z M49.045,53.009C49.029,53.008,49.016,53,49,53H8.766l1.8-3H35h2h12.939    C49.815,51.022,49.505,52.047,49.045,53.009z' fill='#006DF0'/></svg></a>");
  client.println("<a href='mode_anchor'><svg viewBox='0 0 520 520' width='1.6em' height='1.6em'><path d='m497.9,342.2l-48.8-64.4-63.3,49.8 13.5,16.6 35-28.6-1.8,18.3c-17.9,80.6-86.4,140.2-166.1,145v-290.4h49.8v-20.8h-49.8v-63.5c20-4.8 35.3-23.2 35.3-45.5 0-26-20.8-46.7-45.7-46.7-24.9,0-45.7,20.8-45.7,46.7 0,22.2 15.3,40.7 35.3,45.5v63.5h-49.8v20.8h49.8v290.4c-79.7-4.8-148.2-64.4-166.1-145l-1.3-17.9 34.5,28.3 13.5-16.6-63.3-49.8-48.8,64.4 16.6,12.5 27.3-35.9 1.7,18.3v1c19.8,93.2 102.9,161.8 196.3,161.8s176.5-68.6 196.2-162l2.2-18.7 26.9,35.3 16.6-12.4zm-266.8-283.4c-2.84217e-14-14.5 11.4-26 24.9-26 13.5,0 24.9,11.4 24.9,26 0,14.5-11.4,26-24.9,26-13.5-0.1-24.9-11.5-24.9-26z' fill='#006DF0'/></svg></a>");
  client.println("<a href='mode_nav'><svg viewBox='-5 -5 68 68' width='1.6em' height='1.6em'><path d='M44.394,13.091L8.633,29.503c-0.399,0.184-0.632,0.605-0.574,1.041s0.393,0.782,0.826,0.854l15.833,2.653l1.809,14.95   c0.054,0.438,0.389,0.791,0.824,0.865c0.057,0.01,0.113,0.015,0.169,0.015c0.375,0,0.726-0.211,0.896-0.556l17.291-34.882   c0.188-0.38,0.117-0.837-0.178-1.141S44.776,12.914,44.394,13.091z M28.11,45.438l-1.496-12.369   c-0.054-0.44-0.391-0.793-0.828-0.866l-13.362-2.239L42.66,16.087L28.11,45.438z' fill='#006DF0'/><path d='M30,0C13.458,0,0,13.458,0,30s13.458,30,30,30s30-13.458,30-30S46.542,0,30,0z M30,58C14.561,58,2,45.439,2,30   S14.561,2,30,2s28,12.561,28,28S45.439,58,30,58z' fill='#006DF0'/></svg></a>");
  client.println("<a href='mode_setup'><svg  viewBox='0 0 220 200' width='1.6em' height='1.6em'><path d='M113.711,202.935H92.163c-3.242,0-4.373,0.007-15.421-27.364l-8.532-3.468       c-23.248,10.547-26,10.547-26.92,10.547h-1.779l-1.517-1.303l-15.275-14.945c-2.323-2.319-3.128-3.124,8.825-30.137       l-3.479-8.231C0,117.977,0,116.81,0,113.496V92.37c0-3.31,0-4.355,27.972-15.171l3.479-8.249       c-12.644-26.602-11.774-27.428-9.28-29.776l16.427-16.105l2.04-0.064c2.48,0,11.681,3.357,27.371,9.981l8.507-3.454       C86.758,2.054,88.015,2.058,91.246,2.058h21.548c3.228,0,4.363,0.004,15.411,27.382l8.546,3.443       c23.212-10.533,26-10.533,26.927-10.533h1.768l1.517,1.281l15.275,14.92c2.323,2.344,3.117,3.146-8.836,30.17l3.489,8.278       c28.101,10.014,28.101,11.177,28.101,14.498v21.101c0,3.232,0,4.37-28.008,15.192l-3.457,8.256       c12.58,26.487,11.749,27.317,9.394,29.69l-16.552,16.205l-2.051,0.057c-2.469,0-11.649-3.361-27.317-9.992l-8.557,3.457       C118.174,202.935,117.007,202.935,113.711,202.935z M94.403,194.213h16.996c1.95-3.976,6.166-14.516,9.541-23.595l0.68-1.807       l15.475-6.249l1.664,0.705c9.223,3.933,20.124,8.292,24.372,9.631l11.943-11.681c-1.517-4.205-6.116-14.494-10.264-23.173       l-0.837-1.764l6.403-15.285l1.743-0.673c9.316-3.586,20.11-8.013,24.143-10.032V93.88c-4.08-1.918-14.831-6.009-24.096-9.294       l-1.814-0.648l-6.445-15.3l0.769-1.725c3.965-8.947,8.375-19.501,9.788-23.753l-11.975-11.706       c-3.865,1.349-14.688,5.987-23.817,10.153l-1.7,0.78l-15.475-6.238l-0.691-1.721c-3.658-9.13-8.203-19.716-10.253-23.635       H93.569c-1.961,3.965-6.163,14.509-9.53,23.585l-0.669,1.797l-15.432,6.27l-1.664-0.712       c-9.244-3.926-20.167-8.278-24.429-9.616L29.923,43.805c1.496,4.198,6.109,14.48,10.243,23.159l0.848,1.768L34.579,84.01       l-1.732,0.669c-9.301,3.582-20.077,8.006-24.111,10.017v16.431c4.08,1.925,14.82,6.027,24.079,9.326l1.8,0.655l6.446,15.249       l-0.769,1.721c-3.965,8.94-8.371,19.48-9.788,23.724l12,11.742c3.854-1.36,14.663-5.998,23.803-10.168l1.711-0.784       l15.443,6.277l0.691,1.721C87.821,179.723,92.352,190.291,94.403,194.213z M102.495,137.653       c-19.759,0-35.849-15.772-35.849-35.159c0-19.372,16.087-35.134,35.849-35.134c19.748,0,35.799,15.765,35.799,35.134       C138.294,121.881,122.243,137.653,102.495,137.653z M102.495,76.09c-14.956,0-27.113,11.846-27.113,26.405       c0,14.569,12.154,26.426,27.113,26.426c14.931,0,27.078-11.857,27.078-26.426C129.569,87.936,117.426,76.09,102.495,76.09z' fill='#006DF0'/></svg></a>");
  client.println("</div>");
}

void html_navdata (WiFiClient client, bool refresh, int MODE, float SOG, float HDG, float COG, bool ANCHOR, int ANCHOR_DRIFT, bool LINKERROR, int RUDDER_CUR){ // refresh=1, если вызов первоначальный, а не из ajax обновления
  if (!refresh) client.println("<div id='navdata'>");  
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,2em);margin-top: -10px;'>");
  client.print("<div class='t1'>");
  if (!LINKERROR) { client.print((int)SOG); client.println("<sup>kn</sup>"); } else { client.print("NO"); }
  client.print("</div>");
  client.print("<div class='t3'>");
  if (!LINKERROR) { client.print((int)HDG);   client.println("<sup>o</sup>"); } else { client.print("LINK"); }
  client.print("</div>");
  client.print("<div class='t1'>");
  client.print((int)COG);
  client.println("<sup>o</sup></div>");
  client.println("</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,0.5em);'>");  
  client.println("<div class='t0' style='margin:1em;'>SOG</div>");
  client.println("<div class='t0' style='margin:1em;'>HDG</div>");
  client.println("<div class='t0' style='margin:1em;'>COG</div>");
  client.println("</div>");
  client.println("<br>");
  client.print("<div class='t0' style='display: grid;grid-template-columns:repeat(1,1fr);grid-template-rows:repeat(1,2em);'>");
  client.print("<div>RUDDER: ");
  client.print(RUDDER_CUR);
  client.print("%</div>");  
  client.print("</div>");    
  if (ANCHOR) {
    client.print("<div style='display: grid;grid-template-columns:repeat(1,1fr);grid-template-rows:repeat(1,2em);'>");
    client.print("<div>DRIFT: ");
    client.print(ANCHOR_DRIFT);
    client.print(" m</div>");  
    client.print("</div>");    
  } //ANCHOR
  if (!refresh) client.println("</div>");
}

void html_setup (WiFiClient client, bool LINKERROR,long RUDDER_TMAX){
  if (LINKERROR)   client.println("<div class='t0' style='color:#ff0000;'>LINK ERROR!!! CHECK WIRE TO NAVIGATION PLC!</div>");  
  client.println("<div class='t0'>ZOOM</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(4,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='zoom10' class='s'>1.0</a>");
  client.println("<a href='zoom13' class='s'>1.3</a>");
  client.println("<a href='zoom15' class='s'>1.5</a>");
  client.println("<a href='zoom20' class='s'>2.0</a>");
  client.println("</div>");
  client.println("<div class='t0'>WIFI UPDATE</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(1,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='mode_ota' class='s'>OTA UPDATE</a>");
  client.println("</div>");
  client.println("<div class='t0' style='margin-top: 0px;'>RUDDER 0..100% MOVE TIME</div>"); //-20px
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='rtmax1m' class='s'>-1 s</a>");
  client.print("<div>");
  client.print(RUDDER_TMAX/1000);
  client.print(" s</div>");
  client.println("<a href='rtmax1p' class='s'>+1 s</a>");
  client.println("</div>");
  client.println("<a class='t0' href='rtset0p'>RESET RUDDER MOVE TIME</a>");
  
}

void html_sail (WiFiClient client, bool AP,int RUDDER_SET){
  const String on = "on";
  const String off = "off";
  client.println("<div class='t0' style='margin-top: 0px;'>DIRECTION</div>"); //-20px
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(2,2em);'>");
  client.println("<a href='hdghold' class='s'>HDG HOLD</a>");
  client.print("<a href='apON' class='s ");
  if (AP) {client.print(on);} else {client.print(off);}
  client.println("' >AP ON</a>");
  client.println("<a href='SETcog' class='s'>SET COG</a>");
  client.println("<a href='cog1' class='s'>COG 1</a>");
  client.print("<a href='apOFF' class='s ");
  if (!AP) {client.print(on);} else {client.print(off);} 
  client.println("' >STANDBY</a>");
  client.println("<a href='cog2' class='s'>COG 2</a>");
  client.println("</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(4,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='dir10m' class='s'>-10</a>");
  client.println("<a href='dir1m' class='s'>-1</a>");
  client.println("<a href='dir1p' class='s'>+1</a>");
  client.println("<a href='dir10p' class='s'>+10</a>");
  client.println("</div>");
  client.println("<div class='t0'>RUDDER</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(11,1fr);grid-template-rows:repeat(1,2em);'>");
  client.print("<a class='s ");
  if (RUDDER_SET<10) {client.print(on);} else {client.print(off);}
  client.print("' href='rud0p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=10 && RUDDER_SET<20) {client.print(on);} else {client.print(off);}
  client.print("' href='rud1p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=20 && RUDDER_SET<30) {client.print(on);} else {client.print(off);}
  client.print("' href='rud2p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=30 && RUDDER_SET<40) {client.print(on);} else {client.print(off);}
  client.print("' href='rud3p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=40 && RUDDER_SET<50) {client.print(on);} else {client.print(off);}
  client.print("' href='rud4p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=50 && RUDDER_SET<60) {client.print(on);} else {client.print(off);}
  client.print("' href='rud5p'>|</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=60 && RUDDER_SET<70) {client.print(on);} else {client.print(off);}
  client.print("' href='rud6p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=70 && RUDDER_SET<80) {client.print(on);} else {client.print(off);}
  client.print("' href='rud7p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=80 && RUDDER_SET<90) {client.print(on);} else {client.print(off);}
  client.print("' href='rud8p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=90 && RUDDER_SET<100) {client.print(on);} else {client.print(off);}
  client.print("' href='rud9p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=100) {client.print(on);} else {client.print(off);}  
  client.print("' href='rud10p'>&rarr;</a>");
  client.println("</div>");
  client.println("<div class='t0'>PATTERNS</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='#' class='s'>TACK</a>");
  client.println("<a href='#' class='s'>WIND HOLD</a>");
  client.println("<a href='#' class='s'>GYBE</a>");
  client.println("</div>");
}

void html_motor (WiFiClient client, bool AP,int M_GEAR,int M_THROTTLE, int RUDDER_SET){
  const String on = "on";
  const String off = "off";
  client.println("<div class='t0' style='margin-top: 0px;'>DIRECTION</div>"); //-20px
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(2,2em);'>");
  client.println("<a href='hdghold' class='s'>HDG HOLD</a>");
  client.print("<a href='apON' class='s ");
  if (AP) {client.print(on);} else {client.print(off);}
  client.println("' >AP ON</a>");
  client.println("<a href='SETcog' class='s'>SET COG</a>");
  client.println("<a href='cog1' class='s'>COG 1</a>");
  client.print("<a href='apOFF' class='s ");
  if (!AP) {client.print(on);} else {client.print(off);} 
  client.println("' >STANDBY</a>");
  client.println("<a href='cog2' class='s'>COG 2</a>");
  client.println("</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(4,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='dir10m' class='s'>-10</a>");
  client.println("<a href='dir1m' class='s'>-1</a>");
  client.println("<a href='dir1p' class='s'>+1</a>");
  client.println("<a href='dir10p' class='s'>+10</a>");
  client.println("</div>");
  client.print("<div class='t0'>RUDDER ");
  client.print(RUDDER_SET);
  client.println("%</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(11,1fr);grid-template-rows:repeat(1,2em);'>");
  client.print("<a class='s ");
  if (RUDDER_SET<10) {client.print(on);} else {client.print(off);}
  client.print("' href='rud0p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=10 && RUDDER_SET<20) {client.print(on);} else {client.print(off);}
  client.print("' href='rud1p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=20 && RUDDER_SET<30) {client.print(on);} else {client.print(off);}
  client.print("' href='rud2p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=30 && RUDDER_SET<40) {client.print(on);} else {client.print(off);}
  client.print("' href='rud3p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=40 && RUDDER_SET<50) {client.print(on);} else {client.print(off);}
  client.print("' href='rud4p'>&larr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=50 && RUDDER_SET<60) {client.print(on);} else {client.print(off);}
  client.print("' href='rud5p'>|</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=60 && RUDDER_SET<70) {client.print(on);} else {client.print(off);}
  client.print("' href='rud6p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=70 && RUDDER_SET<80) {client.print(on);} else {client.print(off);}
  client.print("' href='rud7p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=80 && RUDDER_SET<90) {client.print(on);} else {client.print(off);}
  client.print("' href='rud8p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=90 && RUDDER_SET<100) {client.print(on);} else {client.print(off);}
  client.print("' href='rud9p'>&rarr;</a>");
  client.print("<a class='s ");
  if (RUDDER_SET>=100) {client.print(on);} else {client.print(off);}  
  client.print("' href='rud10p'>&rarr;</a>");
  client.print("</div>");
  client.print("<div class='t0'>GEAR/THROTTLE</div>");
  client.print("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,2em);'>");
  client.print("<a href='gearR' class='s ");
  if (M_GEAR==2) {client.print(on);} else {client.print(off);}  //2=GEAR_R
  client.println("'>R</a>");
  client.print("<a href='gearN' class='s ");
  if (M_GEAR==0) {client.print(on);} else {client.print(off);}  //0=GEAR_N
  client.println("'>N</a>");
  client.print("<a href='gearF' class='s ");
  if (M_GEAR==1) {client.print(on);} else {client.print(off);}  //1=GEAR_F
  client.println("'>F</a>");
  client.println("</div>");

  client.println("<div style='display: grid;grid-template-columns:repeat(11,1fr);grid-template-rows:repeat(1,2em);'>");
  for (int i=0;i<10;i++) {
    client.print("<a href='");
    client.print("spd");
    client.print(i);
    client.print("s");
    client.print("' class='s ");
    if (M_THROTTLE>=(i*10)) {client.print(on);} else {client.print(off);}
    client.print("'>");
    client.print(i);
    client.print("</a>");
  }
  client.print("<a href='spd100s' class='s ");
  if (M_THROTTLE==100) {client.print(on);} else {client.print(off);}
  client.print("'>F</a>");
  client.println("</div>");
}

void html_anchor (WiFiClient client, bool ANCHOR, float GPS_LAT1, float GPS_LNG1,float GPS_LAT2, float GPS_LNG2, int ANCHOR_DRIFT_MAX){
  const String on = "on";
  const String off = "off"; 
  client.println("<div style='display: grid;grid-template-columns:repeat(1,1fr);grid-template-rows:repeat(3,2em);'>");
  client.print("<a href='anchorON' class='s ");
  if (!ANCHOR) {client.print(on);} else {client.print(off);}
  client.println("' >SET ANCHOR</a>");
  client.print("<a href='anchorOFF' class='s ");
  if (ANCHOR) {client.print(on);} else {client.print(off);} 
  client.println("' >CANCEL</a>");
  client.println("</div>");
  client.println("<div class='t0' style='margin-top: 0px;'>DRIFT</div>"); //-20px
  client.println("<div style='display: grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(1,2em);'>");
  client.println("<a href='drift1m' class='s'>-1</a>");
  client.print("<div>");
  client.print(ANCHOR_DRIFT_MAX);
  client.print(" m</div>");
  client.println("<a href='drift1p' class='s'>+1</a>");
  client.println("</div>");
}

void html_nav (WiFiClient client, float GPS_LAT, float GPS_LNG, int GPS_H, int GPS_M, int GPS_S){
  const String on = "on";
  const String off = "off"; 
  client.println("<div style='display: grid;grid-template-columns:repeat(2,1fr);grid-template-rows:repeat(2,2em);'>");
  client.println("<div class='t0' style='margin-top: 0px;'>LAT</div>"); //-20px
  client.println("<div class='t0' style='margin-top: 0px;'>LNG</div>"); //-20px
  client.print("<div>");
  client.print(GPS_LAT);
  client.print("</div>");
  client.print("<div>");
  client.print(GPS_LNG);
  client.print("</div>");
  client.print("</div>");
  client.println("<div style='display: grid;grid-template-columns:repeat(1,1fr);grid-template-rows:repeat(2,2em);'>");
  client.println("<div class='t0' style='margin-top: 0px;'>TIME UTC</div>"); //-20px
  client.print("<div>");
  if (GPS_H<10) client.print("0");
  client.print(GPS_H);
  client.print(":");
  if (GPS_M<10) client.print("0");
  client.print(GPS_M);
  client.print(".");
  if (GPS_S<10) client.print("0");
  client.print(GPS_S);
  client.print("</div>");
}


#endif
