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
        //webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
             {              
              char chars_in[length+1];
              memcpy(chars_in,payload,length+1);
              String str_in = String(chars_in);

              if ( str_in.equalsIgnoreCase("get eng") ) { //выдать текущие параметры двигателя
                String msg="{\"eng\":{\"rpm\":"+String(tempCnt)+",\"err\":0}}";
                tempCnt++;
                if (tempCnt>70) tempCnt = 0;
                webSocket.sendTXT(num,msg);
              }
              
             } //case
        
            //webSocket.sendTXT(num, "direct message here from esp32");
            //webSocket.broadcastTXT("Broadcast debug message");
            
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
