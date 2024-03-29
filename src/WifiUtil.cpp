#include <Arduino.h>
#include "WifiUtil.h"

WifiUtil::WifiUtil(): _wifiManager(){

}

WifiUtil::~WifiUtil(){

}

void WifiUtil::setupWiFi(const char* name) {
  _wifiManager.setAPCallback(WifiUtil::configModeCallback);
  _wifiManager.setBreakAfterConfig(true);
  _wifiManager.autoConnect(name);
}

void WifiUtil::resetSettings() {
    _wifiManager.resetSettings();
}

void WifiUtil::configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void WifiUtil::setupWiFiAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  delay(100);
  
  const IPAddress ip(192,168,1,1);
  const IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(ip, ip, subnet);
}

void WifiUtil::setupMDNS(const char* name, uint16_t httpPort, uint16_t oscPort) {
  // TODO config.json device_id をホスト名に設定する
  // dns-sd -B connecteddoll.local
  if (!MDNS.begin(name)) {
      Serial.println("Error setting up MDNS responder!");
      while(1){
          delay(1000);
      }
  }
  MDNS.addService("http", "tcp", httpPort);
  MDNS.addService("osc", "udp", oscPort);
}