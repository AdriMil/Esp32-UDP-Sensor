#include "CheckData.h"

/**
 * @brief Check if there is a custom value for UDP port
 * 
 * @param preferences 
 * @param udpPort 
 * @param key_udp_port 
 * @param default_udp_port 
 * @return uint16_t 
 */
uint16_t isCustomUdpPort(Preferences& preferences, uint16_t udpPort,
                        const char *key_udp_port, const uint16_t default_udp_port) {
    if (!preferenceIntKeyExist(preferences, key_udp_port)) {
        udpPort = default_udp_port;
        preferences.putInt(key_udp_port, udpPort);
        return udpPort;
      } else {
        LOG_INFO("udpPort found and is not default values");
        // Value converted from int_32 (preferences answer format) to uint16 (udp port format):
        udpPort = convertInt32ToUInt16(preferences.getInt(key_udp_port));
        LOG_INFO("udpPort value after convertion: %d", udpPort);
        return udpPort;
      }
  }

  /**
  * @brief Check if there is a custom value for wake up frequency
  * 
  * @param preferences 
  * @param time_to_sleep 
  * @param key_udp_msg_frequency 
  * @param default_time_sleep 
  * @return uint64_t 
  */
  uint64_t isCustomTimeToSleep(Preferences& preferences, uint64_t time_to_sleep,
                                const char *key_udp_msg_frequency, const uint64_t default_time_sleep) {
    if (!preferenceUint64_KeyExist(preferences, key_udp_msg_frequency)) {
        LOG_INFO("udpMsgFreq does not exist or his default is 0 - use default time sleep");
        time_to_sleep = default_time_sleep;
        preferences.putULong64(key_udp_msg_frequency, time_to_sleep);
        return time_to_sleep;
        } else {
        LOG_INFO("udpMsgFreq found and is not default values");
        time_to_sleep = preferences.getULong64(key_udp_msg_frequency, default_time_sleep);
        return time_to_sleep;
        }
  }

  /**
  * @brief Check if there is a custom value for udp target ip
  * 
  * @param preferences 
  * @param udp_target_ip 
  * @param key_udp_target_ip 
  * @param default_udp_target_ip 
  * @return String 
  */

  String isCustomIpAdress(Preferences& preferences, String udp_target_ip,
                            const char *key_udp_target_ip, const String default_udp_target_ip) {
    if (!preferenceStringKeyExist(preferences, key_udp_target_ip)) {
        LOG_INFO("devideId does not exist or his default value is 192.168.1.255");
        udp_target_ip = default_udp_target_ip;
        preferences.putString(key_udp_target_ip, udp_target_ip);
        return udp_target_ip;
    } else {
        LOG_INFO("devideId found and is not default values");
        udp_target_ip = preferences.getString(key_udp_target_ip);
        return udp_target_ip;
    }
  }
