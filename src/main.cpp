#include <Arduino.h>
#include <LittleFS.h>
#include <DebugLog.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>
#include "Sensors/AHT10/AHT10.h"
#include "Wifi/CheckWifiConnection.h"
#include "Wifi/SetUpAccessPoint.h"
#include "Miscellaneous/TypeModification.h"
#include "Miscellaneous/MyPreferences.h"
#include <IPAddress.h>

#define s_TO_us_FACTOR 1000000  //convert seconds to microseconds
#define WAKEUP_PIN  GPIO_NUM_33 // Pin used for wake-up esp32 from DeepSleep
#define RESET_PIN  GPIO_NUM_34 // Pin used for wake-up esp32 from DeepSleep

AsyncWebServer server(80);   // Create asynchrone web server

const char* ssid_ap = "ESP32";  // Network name in AccessPoint mod
//if password app lower than 8 digit, Access point won't be secured by password.
const char* password_ap = "123456789"; // Password name in AccessPoint mod.

const char* udpAddress = "10.0.0.5"; // Target Ip adress // TO REMOVE LATER -> Will be selected within UI
WiFiUDP udp;

unsigned long time_save; //Variable used to store current time

// Sleep frequency data:
uint64_t time_to_sleep; //Sleep time
const uint64_t default_time_sleep = 60ULL; //Default sleep time
const char* key_udp_msg_frequency = "udpMsgFreq"; //Preference key name

const char* key_udp_port = "udpPort"; //Preference key name
const uint16_t default_udp_port = 5000; //Default upd port
uint16_t udpPort ; // UDP Port number

const int LED_PIN = 2;  // PCB led

/**
 * @brief Check if a string can be converted to Int
 * 
 * @param str 
 * @return true 
 * @return false 
 */
 bool isInteger(const String& str) {
  char* endptr;
  strtol(str.c_str(), &endptr, 10);
  return *endptr == '\0';
}

/**
 * @brief Check if a string can be converted to Float
 * 
 * @param str 
 * @return true 
 * @return false 
 */
bool isFloat(const String& str) {
  char* endptr;
  strtod(str.c_str(), &endptr);
  return *endptr == '\0';
}

/**
 * @brief Check if a string has IP adress patter
 * 
 * @param ip 
 * @return true 
 * @return false 
 * @note true: 192.168.1.1, 255.255.255.255
 * @note false: 300.168.1.1, abc.def.ghi.jkl, 192.168.1
 * 
 */
bool isValidIPAddress(const String& ip) {
  IPAddress parsedIP;
  return parsedIP.fromString(ip);
}

/**
 * @brief Print wakeUp reason.
 * 
 * @return esp_sleep_wakeup_cause_t 
 */
esp_sleep_wakeup_cause_t getWakeupReason(){

  esp_sleep_wakeup_cause_t wakeupReason;
  wakeupReason = esp_sleep_get_wakeup_cause();

  switch(wakeupReason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : LOG_INFO("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : LOG_INFO("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : LOG_INFO("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : LOG_INFO("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : LOG_INFO("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeupReason); break;
  }
  return wakeupReason;
}

/**
 * @brief Will switch on and off the led
 * @note Defined the blinking during 1s.
 */
void ledBlinking(){
  digitalWrite(LED_PIN, LOW);   // Switch off led
  delay(100);
  digitalWrite(LED_PIN, HIGH);  // Switch on led
  delay(1000);
  digitalWrite(LED_PIN, LOW);   // Switch off led
  delay(500);
}

/**
 * @brief Generate UDP message and send it
 * 
 */
void udpMessage(){
  String message = "T: " + String(temp.temperature) + " C, H: " + String(humidity.relative_humidity) + " %";
  udp.beginPacket(udpAddress, udpPort);
  udp.print(message);
  udp.endPacket();
  LOG_INFO("Message sent: " + message);
}

/**
 * @brief Call functions to blink led, get T° and H values, send UDP message and blink led again
 * 
 */
void onWakeUp(){
  ledBlinking();
  readAHT10Values();
  udpMessage();
  ledBlinking();
}

/**
 * @brief Check if the esp32 wake up by using a WakeUp button
 * 
 * @return true 
 * @return false 
 */
bool wasWakeUpByButton() {
  esp_sleep_wakeup_cause_t wakeupReason = getWakeupReason();
  if (wakeupReason==ESP_SLEEP_WAKEUP_EXT0){
    LOG_INFO("Wake up using button");
    return true;
  }else{
    LOG_INFO("Wake up wihtout button");
    return false;
  }
}

/**
 * @brief Call deepsleep initialisation and start
 * 
 */
void deepSleep(){
  LOG_TRACE("Into Deepsleep function");
  esp_sleep_enable_timer_wakeup(time_to_sleep * s_TO_us_FACTOR);
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);
  LOG_INFO("ESP32 Will sleep in 1 seconds...");
  delay(1000);
  esp_deep_sleep_start();
}

void setup() {

  Serial.begin(115200);
  delay(2000); //To let system wake up

  pinMode(LED_PIN, OUTPUT);
  pinMode(WAKEUP_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  getWakeupReason();

  preferences.begin("wifi", false); //Start Notebook wifi where some data (inputSSID, inputPassword) will be store.
  int wifi_state = wifiCheckup(preferences); //Check if wifi connection is possible

  /**
   * @brief If wifi connexion is working
   * 
   */
  if(wifi_state == 200 ){ //Connection wifi is working well -> Start DeepSleep cycle
    LOG_TRACE("WifiState equal to 200");

    if(preferenceIntKeyExist(preferences, key_udp_port)){
      LOG_INFO("udpPort found and is not default values");
      udpPort = convertInt32ToUInt16(preferences.getInt(key_udp_port)); //Value converted from int_32 (preference answer format) to uint16 (udp port format)
      LOG_INFO("udpPort value after convertion: %u",udpPort);
    }else{
      udpPort = default_udp_port;
    }

    if(preferenceUint64_KeyExist(preferences, key_udp_msg_frequency)){ 
      LOG_INFO("udpMsgFreq found and is not default values");
      time_to_sleep = preferences.getULong64(key_udp_msg_frequency, default_time_sleep);
    } else{
      LOG_INFO("udpMsgFreq does not exist or his default is 0 - use default time sleep");
      time_to_sleep = default_time_sleep ;
    }

    initAHT10(); //Init T° and H sensors to do measure.
    if (wasWakeUpByButton()) {
      time_save=millis(); //get current time indicator

      /**
       * @brief When wakeup button pressed, give a 15s period to click again on it to send manualy new UDP message
       * @note Usefull to get instant sensors values in case of test for example
       */
      while((millis()-time_save)<15000){
        if (digitalRead(WAKEUP_PIN) == LOW){  //If there is an other click on the button during this while period
          LOG_INFO("Button pressed during while loop");
          delay(500);
          time_save=millis(); // Restart the while tempo
          onWakeUp(); //Send UDP message
        }
        if (digitalRead(RESET_PIN) == LOW){  //If there a click on the reset button during this while period
          delay(500);
          LOG_INFO("Reset Button pressed during while loop");
          resetPreference(preferences);
        }
      }
    }
    onWakeUp();
    deepSleep();
}

  // Init LittleFS
  if (!LittleFS.begin()) {
    LOG_ERROR("Erreur LittleFS...");
    return;
  }
    LOG_INFO("LittleFS monté avec succès.");

  // If no existing wifi credentials or wifi cannot connect
  setupAccessPointV2(preferences, server, ssid_ap, password_ap);
  server.begin();
}

void loop() {

}
