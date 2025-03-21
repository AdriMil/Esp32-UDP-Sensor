#ifndef LIB_MY_UTILS_MYUTILS_H_
#define LIB_MY_UTILS_MYUTILS_H_

#include "Miscellaneous/TypeModification.h"
#include "Miscellaneous/MyPreferences.h"
#include <DebugLog.h>

uint16_t isCustomUdpPort(Preferences& preferences, uint16_t udpPort,
                            const char *key_udp_port, const uint16_t default_udp_port);
uint64_t isCustomTimeToSleep(Preferences& preferences, uint64_t time_to_sleep,
                            const char *key_udp_msg_frequency, const uint64_t default_time_sleep);
String isCustomIpAdress(Preferences& preferences, String udp_target_ip,
                            const char *key_udp_target_ip, const String default_udp_target_ip);

#endif  // LIB_MY_UTILS_MYUTILS_H_
