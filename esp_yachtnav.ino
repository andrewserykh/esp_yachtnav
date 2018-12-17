#include <Wire.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include "SSD1306.h"
#include "i2cdev.h"
#include "TinyGPSplus.h"
#include "ESP32_Servo.h"

#define AccelerateMotorPin  27  //ШИМ мотора управления акселератором (ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33)
  
#define GEAR_N 0      //Передача N
#define GEAR_F 1      //Передача F
#define GEAR_R 2      //Передача R

#define MODE_WIFI     0
#define MODE_NAV      1
#define MODE_CONTROL  2

char *ssid = "CalypsoYacht";  // Название сети WiFi
char *password = "rulezzzz";  // Пароль для подключения

SSD1306  display(0x3c, 4, 15); //OLED - GPIO4-SDA, 15-SCL, 16-RST
WiFiServer server(80);
i2cdev Compass;
HardwareSerial SerialGPS(2);
TinyGPSPlus gps_parser;
Servo servoAccelerate;

char linebuf[80];
int charcount = 0;

int WifiClientsCount = 0;     //Количество подключенных клиентов wifi

int MODE;                 //Текущий режим работы

float SOG, SOGkmh;        //Speed over ground (Kn)
float HDG;                //Heading
float COG;                //Course over ground

int M_THROTTLE = 0;       //Motor throttle (0-100%)
int M_GEAR;               //Motor gear

bool AP;                 //Autopilot Heading

int gps_h;
int gps_m;
int gps_s;
double gps_lat, gps_lng;
double gps_alt;
double gps_spd;
double gps_hdg;

long ms_update;
long ms_compass;
long ms_wifi;
long ms_btn0;

bool isAJAX = false; //Этот ответ сервера - AJAX-ответ

#include "ico_array.h";
#include "display_content.h";

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 5, 17); // было 4,15

  pinMode(0, INPUT);        //build-in btn
  pinMode(25, OUTPUT);      //bulid-in led
  pinMode(16, OUTPUT);      //RST
  pinMode(14, INPUT);       //GPS PPS signal

   servoAccelerate.attach(AccelerateMotorPin, 1000, 2000); //for MG995 large servo, use 1000us and 2000us

  Compass.compassBegin();

  digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);    // set GPIO16 low to reset OLED HIGH to running OLED
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "YachtNavigation 1");
  display.setFont(ArialMT_Plain_10);
  //if (i2cCnt>0) { display.drawString(0,16,"+compass"); } else { display.drawString(0,16,"-no compass!"); }
  display.drawString(0, 16 + 10, "+setup");
  display.display();

  WiFi.softAP(ssid, password);

  server.begin();

  delay(1000);

  MODE = MODE_WIFI;
  ms_update = ms_compass = ms_wifi = millis();

}


void loop() {
  servoAccelerate.write(map(M_THROTTLE, 0, 100, 0, 180));

  if (millis() - ms_update > 1000) {
    if (MODE == MODE_WIFI) display_WIFI();
    if (MODE == MODE_NAV) display_NAV();
    ms_update = millis();
  }

  if (millis() - ms_compass > 300) {
    delay(50);
    HDG = Compass.getHeading();
//    delay(50);
    ms_compass = millis();
  }

  if (digitalRead(0) == HIGH) ms_btn0 = millis();
  if (millis() - ms_btn0 > 50 && MODE != MODE_NAV) MODE = MODE_NAV;
  if (millis() - ms_btn0 > 1000 && MODE != MODE_WIFI) MODE = MODE_WIFI;


  if (millis() - ms_wifi > 500) {

    WiFiClient client = server.available();
    if (client) {
      WifiClientsCount++;
      MODE = MODE_NAV;
      //display_NAV();
      memset(linebuf, 0, sizeof(linebuf));
      charcount = 0;

      boolean currentLineIsBlank = true; // HTTP-запрос заканчивается пустой строкой:
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          //Serial.write(c);
          linebuf[charcount] = c; // считываем HTTP-запрос, символ за символом:
          if (charcount < sizeof(linebuf) - 1) charcount++;
          if (c == '\n' && currentLineIsBlank) {
            if (!isAJAX) {
#include "nav_html.h"
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
    ms_wifi = millis();
  } //ms_wifi

  while (SerialGPS.available() > 0) {
    char temp = SerialGPS.read();
    gps_parser.encode(temp);
  }

  /*
    if(gps_parser.time.isValid()) {
        gps_h = gps_parser.time.hour();
        gps_m = gps_parser.time.minute();
        gps_s = gps_parser.time.second();
    }
    if(gps_parser.location.isValid()) {
        gps_lat = gps_parser.location.lat();
        gps_lng = gps_parser.location.lng();
    }
    if(gps_parser.altitude.isValid()) gps_alt = gps_parser.altitude.meters();
    if(gps_parser.speed.isValid()) gps_spd = gps_parser.speed.kmph();
    if(gps_parser.course.isValid()) gps_spd = gps_parser.course.deg();
  */

} //loop

String twoDigits(int x) {
  if (x < 10) return "0" + String(x);
  else return String(x);
}



/*
 * - при опросе компаса и обновлении дисплея происходит конфликт i2c и зависают показания компаса
 * 
 * 
 * 
 */
