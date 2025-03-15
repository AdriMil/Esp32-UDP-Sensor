#ifndef SET_UP_ACCESS_POINT_H
#define SET_UP_ACCESS_POINT_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>  
#include <LittleFS.h>
#include <DebugLog.h>
#include "Miscellaneous/MyPreferences.h"
#include "Wifi/CheckWifiConnection.h"
#include "default.h"
#include "UDP/MyUdp.h"
#include "Miscellaneous/TimeManagement.h"

extern unsigned long routeAccessTime;
extern bool accessPointOn;

void setupAccessPoint(Preferences& preferences, AsyncWebServer& server, const char* ssid_ap, const char* password_ap);

#endif
