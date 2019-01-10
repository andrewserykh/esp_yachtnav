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
html_navdata(client,0,SOG,HDG,COG,GPS_H,GPS_M,GPS_S); //0 - вызов первичный, не из ajax запроса

if (MODE==MODE_SAIL) html_sail(client,AP);
if (MODE==MODE_MOTOR) html_motor(client,AP,M_GEAR,M_THROTTLE);
if (MODE==MODE_SETUP) html_setup(client);

html_menu(client);
html_bodyend(client);
/*
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");  
client.println("Connection: close");
client.println();
          
client.println("<!DOCTYPE HTML><html><head>");

//client.println("<meta http-equiv='Refresh' content='2' />");

//---Start AJAX script

client.println("<script type='text/javascript'>setInterval(\"callAjax('ajaxnav', updateNav)\", 3000);</script>");
client.println("<script>");
client.println("function callAjax(url,cFunction){var xhttp;xhttp=new XMLHttpRequest();");
client.println("xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){cFunction(this);}};");
client.println("xhttp.open(\"GET\",url,true);xhttp.send();}");
client.println("function updateNav(xhttp) {document.getElementById('navdata').innerHTML=xhttp.responseText;}");
client.println("</script>");

//---End AJAX script
client.println("<style>");
client.println("body{width:90%;max-width:1080px;margin-left:auto;margin-right:auto;font-size:1.5em;background:#000;color:#fff;font-family:Arial;fill:#fff;text-align:center;}");
client.println("a{color:#77bbff;text-decoration:none;}");
client.println("button {background:#77bbff;color:#000;border-radius: 5px;border:0px;font-size:0.5em;overflow: hidden;}");
client.println(".on {background:#aaffaa;color:#000;}.red {background:#ffaaaa;color:#000;}.off {background:#aaa;color:#000;}");
client.println(".menu {background: #333333;position:fixed;bottom:0;left:0px;right:0px;text-align:center;}");
client.println(".t0 {font-size: 0.5em;color: #aaa;}.t1 {padding-top: 0.5em;font-size: 1.5em;color: #5599ff;}.t3 {font-size: 2.1em;color: #88ccff;font-weight: bold;}");
client.println("</style>");
client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");

client.println("<body>");
client.println("<p><u>Calypso <span style='color:#77bbff;'><i>yacht</i></span></u></p>");
client.print("<table width='100%'  style='padding:0 10vw 0 10vw;'><tr align='center'><td>");
client.print("<svg width='4vh' height='4vh' viewBox='0 0 24 24'><path d='M20.021 12.593c-.141-.427-.314-.844-.516-1.242l-2.454 1.106c.217.394.39.81.517 1.242l2.453-1.106zm-12.573-.903c.271-.354.58-.675.919-.957l-1.89-1.969c-.328.294-.637.615-.918.957l1.889 1.969zm1.715-1.515c.379-.221.781-.396 1.198-.523l-1.034-2.569c-.41.142-.812.318-1.198.524l1.034 2.568zm-2.759 3.616c.121-.435.288-.854.498-1.25l-2.469-1.066c-.197.403-.364.822-.498 1.25l2.469 1.066zm9.434-6.2c-.387-.205-.79-.379-1.2-.519l-1.024 2.573c.417.125.82.299 1.2.519l1.024-2.573zm2.601 2.13c-.282-.342-.59-.663-.918-.957l-1.89 1.969c.339.282.647.604.918.957l1.89-1.969zm-5.791-3.059c-.219-.018-.437-.026-.649-.026s-.431.009-.65.026v2.784c.216-.025.434-.038.65-.038.216 0 .434.012.649.038v-2.784zm-.648 14.338c-1.294 0-2.343-1.049-2.343-2.343 0-.883.489-1.652 1.21-2.051l1.133-5.606 1.133 5.605c.722.399 1.21 1.168 1.21 2.051 0 1.295-1.049 2.344-2.343 2.344zm12-6c0 2.184-.586 4.233-1.61 5.999l-1.736-1.003c.851-1.471 1.346-3.174 1.346-4.996 0-5.523-4.477-10-10-10s-10 4.477-10 10c0 1.822.495 3.525 1.346 4.996l-1.736 1.003c-1.024-1.766-1.61-3.815-1.61-5.999 0-6.617 5.383-12 12-12s12 5.383 12 12z'/></svg>");
client.print("SOG</td><td>");
client.print("<svg width='4vh' height='4vh' viewBox='0 0 24 24' fill-rule='evenodd'><path d='M12 0c6.623 0 12 5.377 12 12s-5.377 12-12 12-12-5.377-12-12 5.377-12 12-12zm0 2c5.519 0 10 4.481 10 10s-4.481 10-10 10-10-4.481-10-10 4.481-10 10-10zm1.476 12.955c.988-.405 1.757-1.211 2.116-2.216l2.408-6.739-6.672 2.387c-1.006.36-1.811 1.131-2.216 2.119l-3.065 7.494 7.429-3.045zm-.122-4.286c.551.551.551 1.446 0 1.996-.551.551-1.445.551-1.996 0-.551-.55-.551-1.445 0-1.996.551-.551 1.445-.551 1.996 0z'/></svg>");
client.print("HDG</td><td>");
client.print("<svg width='4vh' height='4vh' viewBox='0 0 24 24'><path d='M24 11h-2.051c-.469-4.725-4.224-8.48-8.949-8.95v-2.05h-2v2.05c-4.725.47-8.48 4.225-8.949 8.95h-2.051v2h2.051c.469 4.725 4.224 8.48 8.949 8.95v2.05h2v-2.05c4.725-.469 8.48-4.225 8.949-8.95h2.051v-2zm-11 8.931v-3.931h-2v3.931c-3.611-.454-6.478-3.32-6.931-6.931h3.931v-2h-3.931c.453-3.611 3.32-6.477 6.931-6.931v3.931h2v-3.931c3.611.454 6.478 3.319 6.931 6.931h-3.931v2h3.931c-.453 3.611-3.32 6.477-6.931 6.931zm1-7.931c0 1.104-.896 2-2 2s-2-.896-2-2 .896-2 2-2 2 .896 2 2z'/></svg>");
client.print("COG</td></tr>");
client.print("</table>");

client.println("<div id='navdata'>");

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

client.println("</div>");

client.print("<section>DIRECTION</section><a href='dir1m' class='s'>-1</a>");
client.print("<a href='apOFF' class='s");
if (!AP) client.print("1");
client.print("'>STANDBY</a>");
client.print("<a href='dir1p' class='s'>+1</a><section>.</section><a href='dir10m' class='s'>-10</a>");
client.print("<a href='apON' class='s");
if (AP) client.print("1");
client.print("'>AUTO</a>");
client.print("<a href='dir10p' class='s'>+10</a>");

client.print("<section>GEAR</section>");
client.print("<a href='gearR' class='s");
if (M_GEAR==GEAR_R) client.print("1");
client.print("'>R</a>");

client.print("<a href='gearN' class='s");
if (M_GEAR==GEAR_N) client.print("1");
client.print("'>N</a>");
client.print("<a href='gearF' class='s");
if (M_GEAR==GEAR_F) client.print("1");
client.print("'>F</a>");

client.println("<section>THROTTLE</section>");
for (int i=0;i<10;i++) {
  client.print("<a href='");
  client.print("spd");
  client.print(i);
  client.print("s");
  client.print("' class='s");
  if (M_THROTTLE>=(i*10)) client.print("1");
  client.print("' style='width:8vw;height:5vw;font-size:3vw;'>");
  client.print(i);
  client.print("</a>");
}
  client.print("<a href='spd100s' class='s");
  if (M_THROTTLE==100) client.print("1");
  client.print("' style='width:8vw;height:5vw;font-size:3vw;'>F</a>");

client.println("</body></html>");
*/

#endif
