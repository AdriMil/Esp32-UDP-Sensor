
#include "SetUpAccessPoint.h"

unsigned long routeAccessTime = 0;
bool accessPointOn = false;

 /**
  * @brief Create an access point with my accessible routes
  * 
  * @param preferences 
  * @param server 
  * @param ssid_ap 
  * @param password_ap 
  */
void setupAccessPoint(Preferences& preferences, AsyncWebServer& server, const char* ssid_ap, const char* password_ap) {
  WiFi.softAP(ssid_ap, password_ap);
  accessPointOn = true;
  routeAccessTime = getTime();
  LOG_INFO("Access Point started. You can connect to network: " + String(ssid_ap));
  LOG_INFO("Access Point started. Web Interface  IP : ");
  LOG_INFO(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    routeAccessTime = getTime();
    request->send(LittleFS, "/index.html", "text/html");
    });

  server.on("/wifi.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    routeAccessTime = getTime();
    request->send(LittleFS, "/wifi.html", "text/html");
    });

  server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    routeAccessTime = getTime();
      request->send(LittleFS, "/settings.html", "text/html");
    });

  server.on("/restart.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      routeAccessTime = getTime();
      request->send(LittleFS, "/restart.html", "text/html");
    });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(LittleFS, "/style.css", "text/css");
    });

  server.on("/404.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    routeAccessTime = getTime();
    request->send(LittleFS, "/404.html", "text/html");
    });

  server.on("/dev/25/fe/clock.js", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(LittleFS, "/clock.js", "text/javascript");
    });

  server.on("/reset-memory", HTTP_POST, [&preferences](AsyncWebServerRequest *request) {
    routeAccessTime = getTime();
    resetErrorWifi(preferences);
    LOG_TRACE("In server.on /reset-memory");
    resetPreference(preferences);
    request->redirect("/");
  });

  // Send values using POST request
  server.on("/setwifi", HTTP_POST, [&preferences](AsyncWebServerRequest *request) {
    LOG_TRACE("In server.on /setwifi");
    routeAccessTime = getTime();

  if (request->hasParam("device-id", true) &&
      request->hasParam("ssid", true) &&
      request->hasParam("password", true) &&
      request->hasParam("udp-port", true) &&
      request->hasParam("msg-frequency", true) &&
      request->hasParam("udp-target-ip", true)) {
      LOG_TRACE("device-id, wifi ssid, wifi password, udp port and udp message frequency are existing");

      // Extract ssid and password parameters from http request and store it in pointers ssidParam and passwordParam
      const AsyncWebParameter* deviceIdParam = request->getParam("device-id", true);
      const AsyncWebParameter* ssidParam = request->getParam("ssid", true);
      const AsyncWebParameter* passwordParam = request->getParam("password", true);
      const AsyncWebParameter* udpPortParam = request->getParam("udp-port", true);
      const AsyncWebParameter* udpMsgFreqParam = request->getParam("msg-frequency", true);
      const AsyncWebParameter* udpTargetIpParam = request->getParam("udp-target-ip", true);
      
      LOG_TRACE("Device ID: " + deviceIdParam->value());
      LOG_TRACE("Wifi settings extracted from http request: " + ssidParam->value() + " and " + passwordParam->value());
      LOG_TRACE("UDP setting extracted from http request: " + udpPortParam->value() +
                " and " + udpMsgFreqParam->value() + " and " + udpTargetIpParam->value());

      if (deviceIdParam != nullptr && ssidParam != nullptr &&
          passwordParam != nullptr && udpPortParam != nullptr &&
          udpMsgFreqParam != nullptr && udpTargetIpParam != nullptr) {

          // Save data in Preferences wifi notebook by keys ssid ans password
          LOG_TRACE("parameters are saved in non volatile memory");
          preferences.putString("deviceId", deviceIdParam->value());
          preferences.putString("ssid", ssidParam->value());
          preferences.putString("password", passwordParam->value());
          preferences.putInt("udpPort", static_cast<int32_t>(udpPortParam->value().toInt()));
          preferences.putULong64("udpMsgFreq",  static_cast<uint64_t>(udpMsgFreqParam->value().toInt()));
          preferences.putString("udpTargetIp", udpTargetIpParam->value());

          // Redirect page
          LOG_TRACE("Before redirecting");
          resetErrorWifi(preferences);
          delay(1000);
          ESP.restart();
      } else {
          LOG_TRACE("Issue when try to get parameters");
          request->send(LittleFS, "/400.html", "text/html");
      }
  } else {
      LOG_TRACE("Else loop");
      request->send(LittleFS, "/400.html", "text/html");
  }
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/404.html", "text/html");
  });
}
