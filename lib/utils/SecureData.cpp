#include "SecureData.h"

/**
 * @brief Check if input containt only digits
 * 
 * @param value 
 * @return true 
 * @return false 
 */
bool isValidNumber(String value) {
    for (char c : value) {
        if (!isdigit(c)) return false;
    }
    return !value.isEmpty();
}

/**
 * @brief Check if input has an IP address shape
 * 
 * @param ip 
 * @return true 
 * @return false 
 */
bool isValidIP(String ip) {
    int segments = 0;
    int num = 0;

    for (char c : ip) {
        if (c == '.') {
            if (num < 0 || num > 255) return false;
            num = 0;
            segments++;
        } else if (isdigit(c)) {
            num = num * 10 + (c - '0');
        } else {
            return false;
        }
    }
    return segments == 3 && num >= 0 && num <= 255;
}

/**
 * @brief Check if input has lengh between 1 and 35.
 * 
 * @param value 
 * @return true 
 * @return false 
 */
bool isValidString(String value) {
    if (value.length() < 1 || value.length() > 35) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief Redirect to 400 error page if issue on udp-target-ip parameter
 * 
 * @param request 
 * @param parameterName 
 * @return true 
 * @return false 
 */
bool secureIpAddress(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName) {
    LOG_TRACE("secureIpAddress enter function");
    if (parameterName && parameterName->name() == "udp-target-ip") {
        LOG_TRACE("secureIpAddress - parameter found");
        String udpTargetIp = parameterName->value();
        if (!isValidIP(udpTargetIp)) {
            LOG_TRACE("secureIpAddress parameter not valid");
            request->send(400, "text/plain", "Invalid UDP traget IP format");
            return false;
        }
    } else {
        LOG_TRACE("secureIpAddress - parameter not found");
        request->send(400, "text/plain", "Missing parameter UDP traget ip");
        return false;
    }
    LOG_TRACE("secureIpAddress - parameter valid");
    return true;
}

/**
 * @brief Redirect to 400 error page if issue on ssid or password parameters
 * 
 * @param request 
 * @param parameterName 
 * @return true 
 * @return false 
 */
bool secureStringData(AsyncWebServerRequest *request, const AsyncWebParameter* parameterName) {
    if (parameterName) {
        LOG_TRACE("secureStringData - parameter found");
        String stringToCheck = parameterName->value();
        if (!isValidString) {
            LOG_TRACE("secureStringData - parameter not valid");
            request->send(400, "text/plain", "Invalid String format (wifi ssid or password). Min: 1, Max: 35");
            return false;
        }
    } else {
        LOG_TRACE("secureStringData - parameter not found");
        request->send(400, "text/plain", "Missing parameter ssid or password");
        return false;
    }
    LOG_TRACE("secureStringData - parameter valid");
    return true;
}

/**
 * @brief Redirect to 400 error page if issue on values from paramLimits from default
 * @note See default.cpp for paramLimits definition
 * 
 * @param request 
 * @return true 
 * @return false 
 */
bool secureIntData(AsyncWebServerRequest *request) {
    for (const auto& entry : paramLimits) {
        const String& paramName = entry.first;
        const ParamLimits& limits = entry.second;

        const AsyncWebParameter* param = request->getParam(paramName, true);

        if (!param) {
            LOG_TRACE("secureIntData - parameter not found");
            request->send(400, "text/plain", "Missing parameter: " + paramName);
            return false;
        }

        String paramValue = param->value();
        if (!isValidNumber(paramValue) || paramValue.toInt() < limits.min || paramValue.toInt() > limits.max) {
            LOG_TRACE("secureIntData - parameter not valid");
            request->send(400, "text/plain", "Invalid value for " + paramName);
            return false;
        }
    }
    LOG_TRACE("secureIntData - all parameter valid");
    return true;
}
