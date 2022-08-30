#ifndef __WIFI_UTIL_H__
#define __WIFI_UTIL_H__


#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#include <WiFiClient.h>
#include <WiFiManager.h>

// WiFi 設定
class WifiUtil {
public:
    WifiUtil();
    ~WifiUtil();
    void setupWiFi(const char* name);
    void resetSettings();
    static void configModeCallback(WiFiManager *myWiFiManager);

    void setupWiFiAP(const char* ssid, const char* password);
    
    void setupMDNS(const char* name, uint16_t httpPort,uint16_t oscPort);
private:
    WiFiManager _wifiManager;
};

#endif //__WIFI_UTIL_H__