#ifndef DEFAULT_H
#define DEFAULT_H

#include <Arduino.h>

//WIFI INIT SETTINGS
extern const char* ssid_ap;
extern const char* password_ap;

extern const char* udpAddress; 

// Sleep frequency data:
extern const char* key_udp_msg_frequency; 
extern const uint64_t default_time_sleep; 
extern uint64_t time_to_sleep; 

extern const char* key_udp_port;
extern const uint16_t default_udp_port;
extern uint16_t udpPort;

extern const char* key_udp_ip;
extern const String default_udp_ip;
extern String udp_ip;

#endif