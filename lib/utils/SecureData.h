#ifndef LIB_UTILS_SECUREDATA_H_
#define LIB_UTILS_SECUREDATA_H_

#include <Arduino.h>
#include <DebugLog.h>
#include <ESPAsyncWebServer.h>
#include "default/default.h"

bool secureIpAddress(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName);
bool secureStringData(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName);
bool secureIntData(AsyncWebServerRequest *request);

#endif  // LIB_UTILS_SECUREDATA_H_
