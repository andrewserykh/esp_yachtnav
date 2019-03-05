// https://github.com/Links2004/arduinoWebSockets

#include <WebSocketsServer.h> // WebSocket

#ifndef websocket_event_h
#define websocket_event_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            //Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                //num, ip[0], ip[1], ip[2], ip[3], payload

        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
             {              
              char temp_chars[length+1];
              memcpy(temp_chars,payload,length+1);              
              nmea wsReciever;
              wsReciever.message = String (temp_chars);

              webSocket.sendTXT( num, wsReciever.message );

              String temp(wsReciever.parseCount());
              
              webSocket.sendTXT( num, temp );
             }
        
            // send message to client
            //webSocket.sendTXT(num, "direct message here from esp32");

            // send data to all connected clients
            //webSocket.broadcastTXT("$PWPEQ,ZDA,GGA,POVER*30");
            
            break;
        case WStype_BIN:
            //Serial.printf("[%u] get binary length: %u\n", num, length);
            //hexdump(payload, length);
            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }

}

#endif
