#include "default/default.h"

// WIFI INIT SETTINGS
const char* ssid_ap = "ESP32";  // Network name in AccessPoint mod
// if password app lower than 8 digit, Access point won't be secured by password.
const char* password_ap = "123456789";  // Password name in AccessPoint mod.

// Sleep frequency data:
const char* key_udp_msg_frequency = "udpMsgFreq";  // Preference key name
const uint64_t default_time_sleep = 1200ULL;  // Default sleep time
uint64_t time_to_sleep;  // Sleep time

const char* key_udp_port = "udpPort";  // Preference key name
const uint16_t default_udp_port = 5000;  // Default upd port
uint16_t udpPort;  // UDP Port number

const char* key_udp_target_ip = "udpTargetIp";  // Preference key name
const String default_udp_target_ip = "192.168.1.255";  // Default Ip address
String udp_target_ip;
