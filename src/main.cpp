#include <Arduino.h>
#include <LittleFS.h>
#include <DebugLog.h>
#include <ESPAsyncWebServer.h>
#include <IPAddress.h>
#include "Sensors/AHT10/AHT10.h"
#include "Wifi/CheckWifiConnection.h"
#include "Miscellaneous/TypeModification.h"
#include "Miscellaneous/MyPreferences.h"
#include "Miscellaneous/MyDeepSleep.h"
#include "UDP/MyUdp.h"
#include "default.h"
#include "SetUpAccessPoint.h"

#define WAKEUP_PIN  GPIO_NUM_33  // Pin used for wake-up esp32 from DeepSleep
#define RESET_PIN  GPIO_NUM_34  // Pin used for wake-up esp32 from DeepSleep

AsyncWebServer server(80);  // Create asynchrone web server

unsigned long time_save;  // Variable used to store current time
const int LED_PIN = 2;  // PCB led

/**
 * @brief Will switch on and off the led
 * @note Defined the blinking during 1s.
 */
void ledBlinking() {
  digitalWrite(LED_PIN, LOW);   // Switch off led
  delay(100);
  digitalWrite(LED_PIN, HIGH);  // Switch on led
  delay(1000);
  digitalWrite(LED_PIN, LOW);   // Switch off led
  delay(500);
}

/**
 * @brief Call functions to blink led, get T° and H values, send UDP message and blink led again
 * 
 */
void onWakeUp() {
  ledBlinking();
  readAHT10Values();
  String message = "T: " + String(temp.temperature) + " C, H: " + String(humidity.relative_humidity) + " %";
  udpMessage(message, udpAddress, udpPort);
  ledBlinking();
}

/**
 * @brief Init udp settings if first start
 * 
 * @param preferences 
 */
void initPreferences(Preferences& preferences) {
  preferences.begin("wifi", false);

  if (!preferenceIntKeyExist(preferences, key_udp_port)) {
    udpPort = default_udp_port;
    preferences.putInt(key_udp_port, udpPort);
  }

  if (!preferenceUint64_KeyExist(preferences, key_udp_msg_frequency)) {
    time_to_sleep = default_time_sleep;
    preferences.putULong64(key_udp_msg_frequency, time_to_sleep);
  }

  if (!preferenceStringKeyExist(preferences, key_udp_target_ip)) {
    udp_target_ip = default_udp_target_ip;
    preferences.putString(key_udp_target_ip, udp_target_ip);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);  // To let system wake up

  pinMode(LED_PIN, OUTPUT);
  pinMode(WAKEUP_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);

  initPreferences(preferences);

  // Check if case error wifi + manual reset buttun
  esp_sleep_wakeup_cause_t wakeupReason = getWakeupReason();
  int errorWifiCode = getErrorWifi(preferences);

  if ((errorWifiCode > 0) && (wakeupReason == ESP_SLEEP_WAKEUP_EXT0)) {
    LOG_INFO("error wifi + manual reset buttun");
    resetErrorWifi(preferences);
  }

  int wifi_state = wifiCheckup(preferences);  // Check if wifi connection is possible

  /**
   * @brief If wifi connexion is working
   * 
   */
  if (wifi_state == 200) {  // Connection wifi is working well -> Start DeepSleep cycle
    LOG_TRACE("WifiState equal to 200");

    if (preferenceIntKeyExist(preferences, key_udp_port)) {
      LOG_INFO("udpPort found and is not default values");
      udpPort = convertInt32ToUInt16(preferences.getInt(key_udp_port));  // Value converted from int_32 (preference answer format) to uint16 (udp port format)
      LOG_INFO("udpPort value after convertion: %d", udpPort);
    } else {
      udpPort = default_udp_port;
    }

    if (preferenceUint64_KeyExist(preferences, key_udp_msg_frequency)) {
      LOG_INFO("udpMsgFreq found and is not default values");
      time_to_sleep = preferences.getULong64(key_udp_msg_frequency, default_time_sleep);
    } else {
      LOG_INFO("udpMsgFreq does not exist or his default is 0 - use default time sleep");
      time_to_sleep = default_time_sleep;
    }

    initAHT10();  // Init T° and H sensors to do measure.
    if (wasWakeUpByButton()) {
      time_save = millis();  // Get current time indicator

      /**
       * @brief When wakeup button pressed, give a 15s period to click again on it to send manualy new UDP message
       * @note Usefull to get instant sensors values in case of test for example
       */
      while ((millis()-time_save) < 15000) {
        if (digitalRead(WAKEUP_PIN) == LOW) {  // If there is an other click on the button during this while period
          LOG_INFO("Button pressed during while loop");
          delay(500);
          time_save = millis();  // Restart the while tempo
          onWakeUp();  // Send UDP message
        }
        if (digitalRead(RESET_PIN) == LOW) {  // If there a click on the reset button during this while period
          delay(500);
          LOG_INFO("Reset Button pressed during while loop");
          resetPreference(preferences);
        }
      }
    }
    onWakeUp();
    deepSleep(time_to_sleep, WAKEUP_PIN);
}
  errorWifiCode = getErrorWifi(preferences);
  calculedTime = setNextWackUp(errorWifiCode);

  if (errorWifiCode == 1) {
      // Init LittleFS
      if (!LittleFS.begin()) {
        LOG_ERROR("Erreur LittleFS...");
        return;
      }
        LOG_INFO("LittleFS monté avec succès.");

      // If no existing wifi credentials or wifi cannot connect
      setupAccessPoint(preferences, server, ssid_ap, password_ap);
      server.begin();
  }
  if (errorWifiCode > 1) {
    deepSleep(calculedTime, RESET_PIN);
  }
}

void loop() {
  if (accessPointOn) {
    LOG_INFO("Access point activated, will check if AP not using since 10 minutes");
    delay(10000);
    if (timeDifference(routeAccessTime) > 600000) {
      LOG_INFO("Access point inactive since 10 mins, go to sleep");
      deepSleep(calculedTime, RESET_PIN);
    }
  }
}
