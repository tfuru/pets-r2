#include <Arduino.h>
#include <RotaryEncoder.h>
#include "config.h"

#include "WifiUtil.h"
#include "Motor.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

const int HTTP_PORT = 80;
AsyncWebServer webServer(HTTP_PORT);

Motor controller;
PETSR2_STRUCT_COMAND data;

WifiUtil wifiUtil;

// AP モード SSID
const char* WIFI_AP_SSID = "PETS-R2";
const char* WIFI_AP_PASSWD = "pets-r2";

// mDNS ホスト名
const char* MDNS_HOST = "pets-r2";
// OSC受信 設定
const int OSC_BIND_PORT = 54345;

// ロータリーエンコーダー 初期化
RotaryEncoder encoderL = RotaryEncoder(ENCODER_L_A, ENCODER_L_B, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encoderR = RotaryEncoder(ENCODER_R_A, ENCODER_R_B, RotaryEncoder::LatchMode::TWO03);

static int posL = 0;
static int posR = 0;

static int vL = 0;
static int vR = 0;

static unsigned long TIMEOUT_SEC = 3L;
unsigned long oldTimeL = 0, oldTimeR = 0;

// コマンドリスト 3x3 
volatile int CmdList[100];
volatile int CmdListIndex = 0;
volatile int oldCmdListIndex = -1;

// 割り込みタイマー
hw_timer_t * timer = NULL;
portMUX_TYPE timeMux = portMUX_INITIALIZER_UNLOCKED;

// 1000000us = 1000ms = 1s
uint64_t alarmValue = 1000000 * 0.5;

void motor(int cmd);
void IRAM_ATTR onTimer();
void timerSetup() {
  // 80MHz / 80 = 1MHz (1us)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  // 1000000us = 1000ms = 1s
  uint64_t alarmValue = 1000000;
  timerAlarmWrite(timer, alarmValue, true);
  timerAlarmEnable(timer);
}

void webServerSetup() {
  Serial.println(F("webServerSetup"));

  // mDNS 設定
  wifiUtil.setupMDNS(MDNS_HOST, HTTP_PORT, OSC_BIND_PORT);

  // API 実装 ---- 
  {
    // IPAddress 取得 API
    // curl -X POST -H "Content-Type: application/json" -d '{}' http://connecteddoll.local/api/ip    
    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/ip", [](AsyncWebServerRequest *request, JsonVariant &json) {
      IPAddress localIP = WiFi.localIP();
      String output = "{\"status\":\"OK\",\"ip\":\"" + localIP.toString() + "\"}";
      request->send(200, "application/json", output);
    });
    webServer.addHandler(handler);
  }
}

void IRAM_ATTR checkPositionL() {
  // just call tick() to check the state.
  // encoderL->tick();
  encoderL.tick();
}

void IRAM_ATTR  checkPositionR() {
  // just call tick() to check the state.
  // encoderR->tick();
  encoderR.tick();
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  // WiFi 接続設定
  wifiUtil.setupWiFiAP(WIFI_AP_SSID, WIFI_AP_PASSWD);
  Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());
  delay(1000);
  webServerSetup();

  // 割り込みタイマー 開始 
  timerSetup();

  // IO0 ボタン 割り込み
  pinMode(BUTTON_0, INPUT_PULLUP);

  // モーター操作 初期化 
  controller.begin();

  // register interrupt routine
  attachInterrupt(ENCODER_L_A, checkPositionL, CHANGE);
  attachInterrupt(ENCODER_L_B, checkPositionL, CHANGE);

  attachInterrupt(ENCODER_R_A, checkPositionR, CHANGE);
  attachInterrupt(ENCODER_R_B, checkPositionR, CHANGE);
}

// 移動の実行
int command(int l, int r) {
  int c = PETSR2_MOVE_DEFAULT;
  if (l == -1 && r == 1) {
    // 右に曲がる
    c = PETSR2_MOVE_RIGHT_ROTATION;
  }
  else if (l == 1 && r == -1) {
    // 左に曲がる
    c = PETSR2_MOVE_LEFT_ROTATION;
  }
  else if (l == -1 && r == -1) {
    // 前
    c = PETSR2_MOVE_FORWARD;
  }
  else if (l == 1 && r == 1) {
    // 後
    c = PETSR2_MOVE_BACKWARD;
  }
  else if (l == 0 && r == 0) {
    c = PETSR2_MOVE_STOP;
  }
  
  if (data.value == c) {
    return PETSR2_MOVE_DEFAULT;
  }  
  data.value = c;
  return c;
}

void motor(volatile int cmd) {
  Serial.print(F("motor ")); Serial.println(cmd);
  switch (cmd) {
    case PETSR2_MOVE_FORWARD:
      Serial.println(F("PETSR2_MOVE_FORWARD"));
      controller.forward();
      break;
    case PETSR2_MOVE_BACKWARD:
      Serial.println(F("PETSR2_MOVE_BACKWARD"));
      controller.backward();
      break;
    case PETSR2_MOVE_STOP:
      // Serial.println(F("PETSR2_MOVE_STOP"));
      controller.stopMove();
      break;
    case PETSR2_MOVE_RIGHT_ROTATION:
      Serial.println(F("PETSR2_MOVE_RIGHT_ROTATION"));
      controller.rightRotation();
      break;
    case PETSR2_MOVE_LEFT_ROTATION:
      Serial.println(F("PETSR2_MOVE_LEFT_ROTATION"));
      controller.leftRotation();
      break;        
    default:
      // controller.stopMove();
      break;
  }
}

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timeMux);
  if (CmdListIndex > 0) {
      motor(CmdList[CmdListIndex]);
      CmdListIndex--;
      if (CmdListIndex < 0) CmdListIndex = 0;
  }
  portEXIT_CRITICAL_ISR(&timeMux);
}

void loop() {
  
  int value = digitalRead(BUTTON_0);
  if (value == LOW) {
    portENTER_CRITICAL(&timeMux);
    CmdList[CmdListIndex] = command(-1, 1);
    CmdListIndex++;
    if (CmdListIndex > 100) CmdListIndex = 100;
    portEXIT_CRITICAL(&timeMux);
    delay(300);
  }
  if (CmdListIndex != oldCmdListIndex) {
    Serial.print(F("CmdListIndex ")); Serial.println(CmdListIndex);
    oldCmdListIndex = CmdListIndex;
  }

  encoderL.tick();
  int newPosL = encoderL.getPosition();
  if (posL != newPosL) { 
    // v: -1 前進 v: 1 後進
    vL = (int)(encoderL.getDirection());
    posL = newPosL;
  } else {
    // 変化がなかった
    unsigned long time = millis();
    if (time - oldTimeL > TIMEOUT_SEC * 1000) {
      vL = 0;
      oldTimeL = time;
    }
  }

  encoderR.tick(); 
  int newPosR = encoderR.getPosition();
  if (posR != newPosR) {
    // v: -1 前進 v: 1 後進
    vR = (int)(encoderR.getDirection());
    posR = newPosR;
  }
  else {
    // 変化がなかった
    unsigned long time = millis();
    if (time - oldTimeR > TIMEOUT_SEC * 1000) {
      vR = 0;
      oldTimeR = time;
    }
  }

  volatile int c = command(vL, vR);
  if (c != PETSR2_MOVE_DEFAULT) {
    portENTER_CRITICAL(&timeMux);
    CmdList[CmdListIndex] = c;
    CmdListIndex++;
    if (CmdListIndex > 100) CmdListIndex = 100;
    portEXIT_CRITICAL(&timeMux);
  }

}
