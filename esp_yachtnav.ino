/* v2.10
GPS и компас вынесены на nano
WS обработка связи websocket
NMEA класс создания сообщений
Обмен с nano по короткому протоколу

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
#include "gpsnav.h"           // Класс GPS навигации

#define AccelerateMotorPin  27  //ШИМ мотора управления акселератором (ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33)
  
#define GEAR_N          0      //Передача N
#define GEAR_F          1      //Передача F
#define GEAR_R          2      //Передача R

#define MODE_WIFI       0
#define MODE_MOTOR      1
#define MODE_SAIL       2
#define MODE_ANCHOR     3
#define MODE_NAV        4
#define MODE_SETUP      9
#define MODE_OTA        99

#define T_MS_NANO       300   //ms интервал отправки команд

Preferences prefs;  //EEPROM объект

int ZOOM = 10;
char *ssid = "CalypsoYacht";    // Название сети WiFi
char *password = "rulezzzz";    // Пароль для подключения
char *OTAssid = "Escobar";      // Название сети WiFi для OTA
char *OTApassword = "rulezzzz"; // Пароль для подключения
SSD1306  display(0x3c, 4, 15);  //OLED - GPIO4-SDA, 15-SCL, 16-RST
WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
HardwareSerial SerialNano(2);
Servo servoAccelerate;
char linebuf[80];
int charcount = 0;
int WifiClientsCount = 0; //Количество подключенных клиентов wifi

int   MODE;               //Текущий режим работы
float SOG;                //Speed over ground (knots)
int   SOGkmh;             //Speed over ground (kmh)
int   HDG;                //Heading
int   COG;                //Course over ground
int   M_THROTTLE = 0;     //Motor throttle (0-100%)
int   M_GEAR;             //Motor gear
bool  AP;                 //Autopilot Heading
bool  ANCHOR;             //Якорь опущен
int   ANCHOR_DRIFT;       //Смещение от точки якорения
int   ANCHOR_DRIFT_MAX;   //Максимальное смещение

struct {
  int Current;            //Текущее положение (0-100%)
  int Set;                //Заданное положение (0-100%)
  float CurAcc;           //Текущее положение с высокой точностью
  long ms_tmax;           //Время хода от 0 до 100
  long ms_start;          //Время начала движения
} RUDDER;

bool  LINKERROR;           //Нет связи с Arduino-Nano

char SerialNanoIn[64];    //буфер приема 
byte SerialNanoInLen;     //заполнение буфера
long SerialNanoInMillis;
gpsnav GPS;
gpspoi PoiAnchor;
long ms_update, ms_nano, ms_wifi, ms_btn0;
bool isAJAX = false;      //Этот ответ сервера - AJAX-ответ


struct { //Структура пакета данных измерений передаваемых из nano в esp
  byte Header; //заголовок пакета 0x65
  byte Length; //длина пакета
  uint16_t CompassCourse;
  uint16_t GpsCourse;
  uint16_t GpsSpeedKmh;
  float GpsSpeedKnots;
  float GpsLat;
  float GpsLng;
  uint16_t GpsAlt;
  uint16_t GpsHour;
  uint16_t GpsMinute;
  uint16_t GpsSecond;
  uint16_t GpsDay;
  uint16_t GpsMonth;
  uint16_t GpsYear;
} datanano;

struct { //Структура пакета команд передаваемых из esp в nano
  byte Header;
  byte Length;
  byte Relay;
} dataesp;

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
  ANCHOR_DRIFT_MAX = prefs.getUInt("drift", 20);
  RUDDER.Current = prefs.getUInt("rudder",0);
  RUDDER.Set = prefs.getUInt("rudder",0);
  RUDDER.ms_tmax = prefs.getUInt("rudtmax",10000);
  prefs.end();

  pinMode(0,INPUT);       //build-in btn
  pinMode(25,OUTPUT);     //bulid-in led
  pinMode(16,OUTPUT);     //RST
  
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
  
  RUDDER.CurAcc = (float)RUDDER.Current;
  dataesp.Relay=0x00;
  ms_update = ms_wifi = ms_nano = millis();
  RUDDER.ms_start = millis();
}


void loop() {

//---Блок Управления

  servoAccelerate.write(map(M_THROTTLE, 0, 100, 0, 180)); //Управлением приводом акселератора

  if (RUDDER.Current != RUDDER.Set) { //Заданное положение румпеля не соответсвует текущему
    if (RUDDER.Current>100) RUDDER.Current = 100;
    if (RUDDER.Current<0) RUDDER.Current = 0;
    
    if (RUDDER.Current < RUDDER.Set){
      bitWrite ( dataesp.Relay,0,HIGH );
      bitWrite ( dataesp.Relay,1,LOW );
      RUDDER.CurAcc = ( ( (millis()-RUDDER.ms_start) * 100.0) / RUDDER.ms_tmax ) + RUDDER.CurAcc;
      RUDDER.Current = (int) RUDDER.CurAcc;
      RUDDER.ms_start = millis();
    }
    if (RUDDER.Current > RUDDER.Set){
      bitWrite ( dataesp.Relay,0,LOW );
      bitWrite ( dataesp.Relay,1,HIGH );
      RUDDER.CurAcc = RUDDER.CurAcc - ( ((millis()-RUDDER.ms_start) * 100.0) / RUDDER.ms_tmax );
      RUDDER.Current = (int) RUDDER.CurAcc;
      RUDDER.ms_start = millis();      
    }
  } else { //Заданное положение румпеля не равно текущему
    RUDDER.ms_start = millis();
    bitWrite ( dataesp.Relay,0,LOW );
    bitWrite ( dataesp.Relay,1,LOW ); 
  }//Заданное положение румпеля не равно текущему

  if (millis() - ms_nano > T_MS_NANO) { //отправка команд на Arduino-Nano
    dataesp.Header = 0x69;  //Заголовок пакета "E"
    dataesp.Length = (byte)sizeof(dataesp);
    for (int i=0; i<sizeof(dataesp); i++) SerialNano.write( ((char*)&dataesp)[i] );
    ms_nano = millis();

    //Расчет текущего положения по времени
    /*
    if (RUDDER.Current < RUDDER.Set){
      RUDDER.CurAcc = ( ( (millis()-RUDDER.ms_start) * 100.0) / RUDDER.ms_tmax ) + RUDDER.CurAcc;
      RUDDER.Current = (int) RUDDER.CurAcc;
      RUDDER.ms_start = millis();
    }
    if (RUDDER.Current > RUDDER.Set){
      RUDDER.CurAcc = RUDDER.CurAcc - ( ((millis()-RUDDER.ms_start) * 100.0) / RUDDER.ms_tmax );
      RUDDER.Current = (int) RUDDER.CurAcc;
      RUDDER.ms_start = millis();
    }
    */
    //(millis() - ms_start)
  }

//---Конец Блока Управления

  if (MODE == MODE_OTA) {
    prefs.begin("setup", false); //инициализация non-voltage storage
    prefs.putUInt("mode", MODE_OTA);
    prefs.end();
    delay(1000);
    ESP.restart();
  }

  if (millis() - ms_update > 3000) {
    if (MODE == MODE_WIFI) display_WIFI();
    if (MODE == MODE_SAIL||MODE == MODE_MOTOR) display_NAV();
    prefs.begin("setup", false);
    prefs.putUInt("rudder", RUDDER.Current);
    prefs.end();
    ms_update = millis();
  }

  if (digitalRead(0) == HIGH) ms_btn0 = millis();
  if (millis() - ms_btn0 > 50 && MODE != MODE_MOTOR) MODE = MODE_MOTOR;
  if (millis() - ms_btn0 > 1000 && MODE != MODE_WIFI) MODE = MODE_WIFI;
  if (millis() - ms_btn0 > 2000) MODE = MODE_OTA;

    webSocket.loop();

    WiFiClient client = server.available();
    if (client) {
      WifiClientsCount++;
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
    SerialNanoIn[SerialNanoInLen] = SerialChar;
    SerialNanoInLen++;

    SerialNanoInMillis = millis();
  }
  
  if (SerialNanoInLen > 0 && (millis() - SerialNanoInMillis > 100)) {
    
    //for (int q = 0; q < SerialNanoInLen; q++) { //вывод принятого пакета на консоль
    //Serial.print((byte)SerialNanoIn[q], HEX); Serial.print(" ");
    //} Serial.println(" ");
    
    if (SerialNanoIn[0] == 0x65){ //Проверка заголовка пакета 0x65
      if (sizeof(datanano)>=SerialNanoInLen) for (int i=0; i<SerialNanoInLen; i++) ((byte*)&datanano)[i] = SerialNanoIn[i]; //загрузка структуры данных
      LINKERROR = false;
      HDG = datanano.CompassCourse;
      GPS.hdg = datanano.GpsCourse;      
      SOGkmh = datanano.GpsSpeedKmh;
      SOG = datanano.GpsSpeedKnots;
      GPS.lat = datanano.GpsLat;
      GPS.lng = datanano.GpsLng;
      GPS.alt = datanano.GpsAlt;
      GPS.hour = datanano.GpsHour;
      GPS.minute = datanano.GpsMinute;
      GPS.second = datanano.GpsSecond;
      GPS.day = datanano.GpsDay;
      GPS.month = datanano.GpsMonth;
      GPS.year = datanano.GpsYear;

      if (ANCHOR){ //если якорь опущен, расчет смещения от точки якорения
        ANCHOR_DRIFT = GPS.distance(GPS.lat,GPS.lng,PoiAnchor.lat,PoiAnchor.lng);
      } //anchor
    } //0x65
    
    SerialNanoInLen=0;
  } 
  

} //loop

String twoDigits(int x) {
  if (x < 10) return "0" + String(x);
  else return String(x);
}
