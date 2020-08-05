#ifndef ota_h
#define ota_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

  WiFi.mode(WIFI_STA);
  WiFi.begin(OTAssid, OTApassword);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    display.drawStringMaxWidth(9, 18, 128,"Connection Failed! Rebooting...");
    display.display();
    delay(5000);
    prefs.begin("setup", false); //инициализация non-voltage storage
    prefs.putUInt("mode", MODE_WIFI);
    prefs.end();
    delay(1000);    
    ESP.restart();
  }

  // ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname("yachtnav");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //Serial.println("Start updating " + type);
    display.clear();
    display.drawStringMaxWidth(0, 18, 128,"Uploading...");
    display.display();
    
  });
  ArduinoOTA.onEnd([]() {
    display.drawStringMaxWidth(0, 40, 128,"Done.");
    display.display();
    prefs.begin("setup", false); //инициализация non-voltage storage
    prefs.putUInt("mode", 0);
    prefs.end();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    display.drawProgressBar(0, 32, 120, 10, (progress / (total / 100)));    
    display.display();
    
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  display.setLogBuffer(5, 30);
  display.println("Ready");
  display.println(WiFi.localIP());
  display.drawLogBuffer(0, 18);
  display.display();
  
#endif
