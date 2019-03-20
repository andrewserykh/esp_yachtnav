/* v2.04
GPS и компас вынесены на nano
WS обработка связи websocket
NMEA класс создания сообщений

В процессе создания Якорный аларм
---
https://github.com/Links2004/arduinoWebSockets
*/
#include <Wire.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include "SSD1306.h"          // OLED display
#include <Preferences.h>      // EEPROM
#include <WebSocketsServer.h> // WebSocket
#include <ESPmDNS.h>          // OTA update
#include <WiFiUdp.h>          // OTA update
#include <ArduinoOTA.h>       // OTA update
#include "ESP32_Servo.h"      // Servo PWM motor
#include "html.h"             // Содержимое web интерфейса
#include "modbusrtu.h"        // Протокол ModbusRTU
#include "gpsnav.h"           // Класс GPS навигации

#define AccelerateMotorPin  27  //ШИМ мотора управления акселератором (ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33)
  
#define GEAR_N 0      //Передача N
#define GEAR_F 1      //Передача F
#define GEAR_R 2      //Передача R

#define MODE_WIFI     0
#define MODE_MOTOR    1
#define MODE_SAIL     2
#define MODE_ANCHOR   3
#define MODE_NAV      4
#define MODE_SETUP    9
#define MODE_OTA      99

#define T_MS_NANO     500   //ms интервал опроса периферии

Preferences prefs;  //EEPROM объект

int ZOOM = 10;
char *ssid = "CalypsoYacht";    // Название сети WiFi
char *password = "rulezzzz";    // Пароль для подключения
char *OTAssid = "nextflight";   // Название сети WiFi
char *OTApassword = "rulezzzz"; // Пароль для подключения
SSD1306  display(0x3c, 4, 15);  //OLED - GPIO4-SDA, 15-SCL, 16-RST
WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
HardwareSerial SerialNano(2);
Servo servoAccelerate;
char linebuf[80];
int charcount = 0;
int WifiClientsCount = 0; //Количество подключенных клиентов wifi
int MODE;                 //Текущий режим работы
float SOG;
int SOGkmh;               //Speed over ground (Kn)
int HDG;                  //Heading
int COG;                  //Course over ground
int M_THROTTLE = 0;       //Motor throttle (0-100%)
int M_GEAR;               //Motor gear
bool AP;                  //Autopilot Heading
bool ANCHOR;              //Якорь опущен
modbusrtu devModbus;
char SerialNanoIn[64];    //буфер приема 
byte SerialNanoInLen;     //заполнение буфера
long SerialNanoInMillis;
gpsnav  GPS;
gpspoi  PoiAnchor;
long ms_update, ms_nano, ms_wifi, ms_btn0;
bool isAJAX = false;      //Этот ответ сервера - AJAX-ответ
//временные переменные - удалить:
int tempCnt=0;

#include "ico_array.h";
#include "display_content.h";
#include "websocket_event.h";

void setup() {
  Serial.begin(115200);
  SerialNano.begin(9600, SERIAL_8N1, 5, 17); // BAUD,PARITY,RX,TX

  prefs.begin("setup", false); //инициализация non-voltage storage
  MODE = prefs.getUInt("mode", 0);
  COG = (float)prefs.getUInt("cog", 0);
  ZOOM = prefs.getUInt("zoom", 10);
  ANCHOR = prefs.getBool("anchor",false);
  prefs.end();

  pinMode(0, INPUT);        //build-in btn
  pinMode(25, OUTPUT);      //bulid-in led
  pinMode(16, OUTPUT);      //RST

  servoAccelerate.attach(AccelerateMotorPin, 1000, 2000); //for MG995 large servo, use 1000us and 2000us
    
  digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);    // set GPIO16 low to reset OLED HIGH to running OLED
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  if (MODE==MODE_OTA) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "On The Air Mode");
    display.display();
    display.setFont(ArialMT_Plain_10);
#include "ota.h"
    while(1){
        ArduinoOTA.handle();

        if (digitalRead(0) == HIGH) ms_btn0 = millis();

        if (millis() - ms_btn0 > 50){
              prefs.begin("setup", false); //инициализация non-voltage storage
              prefs.putUInt("mode", MODE_WIFI);
              prefs.end();
              delay(500);
              ESP.restart();
        }
    }//while 1
  } //MODE=MODE_OTA

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "YachtNavigation");
  display.drawString(1, 0, "YachtNavigation");
  display.display();

  WiFi.softAP(ssid, password);
  
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  delay(1000);

  //MODE = MODE_WIFI;
  ms_update = ms_wifi = ms_nano = millis();
}


void loop() {

  if (MODE == MODE_OTA) {
    prefs.begin("setup", false); //инициализация non-voltage storage
    prefs.putUInt("mode", MODE_OTA);
    prefs.end();
    delay(1000);
    ESP.restart();
  }
  
  servoAccelerate.write(map(M_THROTTLE, 0, 100, 0, 180));

  if (millis() - ms_update > 3000) {
    if (MODE == MODE_WIFI) display_WIFI();
    if (MODE == MODE_SAIL||MODE == MODE_MOTOR) display_NAV();
    ms_update = millis();
  }

  if (millis() - ms_nano > T_MS_NANO) {
    devModbus.make(01, 4, 0, 16);
    SerialNano.write(devModbus.packetout, devModbus.packetout_len);
    ms_nano = millis();
  }
//  if (millis() - ms_compass > 1000) {
//    HDG = Compass.getHeading();
//    ms_compass = millis();
//  }

  if (digitalRead(0) == HIGH) ms_btn0 = millis();

  if (millis() - ms_btn0 > 50 && MODE != MODE_MOTOR) MODE = MODE_MOTOR;
  if (millis() - ms_btn0 > 1000 && MODE != MODE_WIFI) MODE = MODE_WIFI;
  if (millis() - ms_btn0 > 2000) MODE = MODE_OTA;

    webSocket.loop();

    WiFiClient client = server.available();
    if (client) {
      WifiClientsCount++;
      //MODE = MODE_MOTOR;
      //display_NAV();
      memset(linebuf, 0, sizeof(linebuf));
      charcount = 0;

      boolean currentLineIsBlank = true; // HTTP-запрос заканчивается пустой строкой:
      while (client.connected()) {
        if (client.available()) { //оттяжечка на обработку ответа компаса
          char c = client.read();
          //Serial.write(c);
          linebuf[charcount] = c; // считываем HTTP-запрос, символ за символом:
          if (charcount < sizeof(linebuf) - 1) charcount++;
          if (c == '\n' && currentLineIsBlank) {
            if (!isAJAX) {
#include "html_view.h"
            }
            isAJAX = false;
            break;
          }
          if (c == '\n') {
            currentLineIsBlank = true;
#include "web_get.h"
            //display_NAV();
            currentLineIsBlank = true;
            memset(linebuf, 0, sizeof(linebuf));
            charcount = 0;
          } else if (c != '\r') {
            currentLineIsBlank = false;
          }
        }
      }
      delay(1); // даем веб-браузеру время, чтобы получить данные
      client.stop(); // закрываем соединение
      WifiClientsCount--;
    }
  //  ms_wifi = millis();

  while (SerialNano.available() > 0) {
    char SerialChar = (char)SerialNano.read();
    devModbus.packet[devModbus.packet_length] = SerialChar;
    devModbus.packet_length++;

    SerialNanoInMillis = millis();
  }
  
  if (devModbus.packet_length > 0 && (millis() - SerialNanoInMillis > 100)) {
    if (devModbus.ispacket()) {
      HDG = devModbus.getint(0);
      GPS.hdg = devModbus.getint(2);      
      SOGkmh = devModbus.getint(4);
      SOG = devModbus.getfloat(6);
      GPS.lat = devModbus.getfloat(10);
      GPS.lng = devModbus.getfloat(14);
      GPS.alt = devModbus.getint(18);
      GPS.hour = devModbus.getint(20);
      GPS.minute = devModbus.getint(22);
      GPS.second = devModbus.getint(24);
      GPS.day = devModbus.getint(26);
      GPS.month = devModbus.getint(28);
      GPS.year = devModbus.getint(30);
       //for (int q = 0; q < devModbus.packet_length; q++) { //вывод принятого пакета на консоль
       //Serial.print((byte)devModbus.packet[q], HEX); Serial.print(" ");
       //} Serial.println(" ");
    }
    devModbus.packet_length=0;
  } 
  

} //loop

String twoDigits(int x) {
  if (x < 10) return "0" + String(x);
  else return String(x);
}
