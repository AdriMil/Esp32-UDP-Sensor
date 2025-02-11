#include <Arduino.h>
#include <LittleFS.h>
#include <DebugLog.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include "Sensors/AHT10/AHT10.h"
#include "Wifi/CheckWifiConnection.h"
#include "Wifi/SetUpAccessPoint.h"

Preferences preferences;
AsyncWebServer server(80);   // Create asynchrone web server

const char* ssid_ap = "ESP32";  // Network name in AccessPoint mod
//if password app lower than 8 digit, Access point won't be secured by password.
const char* password_ap = "123456789"; // Password name in AccessPoint mod. 


// String inputSSID ;
// String inputPassword ;  

void setup() {

  Serial.begin(115200);

  initAHT10(); //init AHT10 sensor (T° and H sensor)

  // Init LittleFS
  if (!LittleFS.begin()) {
    LOG_ERROR("Erreur LittleFS...");
    return;
  }
    LOG_INFO("LittleFS monté avec succès.");

  preferences.begin("wifi", false); //Start Notebook wifi where some data (inputSSID, inputPassword) will be store.
  //Check if wifi connection is possible
  int wifi_state = wifiCheckup(preferences);

  if(wifi_state == 200 ){
    return;
}
  // If no existing wifi credentials or wifi cannot connect
  setupAccessPointV2(preferences, server, ssid_ap, password_ap);
  server.begin();
}

void loop() {

}
