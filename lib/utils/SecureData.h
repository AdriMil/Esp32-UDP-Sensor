#ifndef UTILS_SECUREDATA_H_
#define UTILS_SECUREDATA_H_

#include <Arduino.h>
#include <DebugLog.h>
#include <ESPAsyncWebServer.h>
#include "default.h"

bool secureIpAddress(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName);
bool secureStringData(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName);
bool secureIntData(AsyncWebServerRequest *request);

#endif  // UTILS_SECUREDATA_H_