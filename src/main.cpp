#include <Arduino.h>
#include "config.h"
#include "Controller.h"

Controller controller;
PETSR2_STRUCT_COMAND data;

#include "WifiUtil.h"
WifiUtil wifiUtil;

// AP モード SSID
const char* WIFI_SSID = "PETS-R2";

// ロータリーエンコーダー
#include <RotaryEncoder.h>

RotaryEncoder *encoderL = nullptr;
RotaryEncoder *encoderR = nullptr;

static int posL = 0;
static int posR = 0;

static int vL = 0;
static int vR = 0;

unsigned long TIMEOUT_SEC = 3L;
unsigned long oldTimeL = 0, oldTimeR = 0;

IRAM_ATTR void checkPositionL() {
  // just call tick() to check the state.
  encoderL->tick();
}

IRAM_ATTR void checkPositionR() {
  // just call tick() to check the state.
  encoderR->tick();
}

void setup() {
  Serial.begin(76800);
  while (!Serial)
    ;

  // WiFi 接続設定
  // wifiUtil.setupWiFi(WIFI_SSID);
  // Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());
  // delay(1000);

  // IO0 ボタン 割り込み
  pinMode(BUTTON_0, INPUT_PULLUP);

  // モーター操作 初期化 0,2,4 
  controller.begin();

  // ロータリーエンコーダー 初期化
  encoderL = new RotaryEncoder(ENCODER_L_A, ENCODER_L_B, RotaryEncoder::LatchMode::TWO03);
  encoderR = new RotaryEncoder(ENCODER_R_A, ENCODER_R_B, RotaryEncoder::LatchMode::TWO03);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(ENCODER_L_A), checkPositionL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_L_B), checkPositionL, CHANGE);

  attachInterrupt(digitalPinToInterrupt(ENCODER_R_A), checkPositionR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_R_B), checkPositionR, CHANGE);  
}

// 移動の実行
void motor(int l, int r) {
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
  
  /*
  if (data.value == c) {
    return;
  }
  */
  data.value = c;

  switch (data.value) {
    case PETSR2_MOVE_FORWARD:
      Serial.println("PETSR2_MOVE_FORWARD");
      controller.forward();
      break;
    case PETSR2_MOVE_BACKWARD:
      Serial.println("PETSR2_MOVE_BACKWARD");
      controller.backward();
      break;
    case PETSR2_MOVE_STOP:
      // Serial.println("PETSR2_MOVE_STOP");
      controller.stopMove();
      break;
    case PETSR2_MOVE_RIGHT_ROTATION:
      Serial.println("PETSR2_MOVE_RIGHT_ROTATION");
      controller.rightRotation();
      break;
    case PETSR2_MOVE_LEFT_ROTATION:
      Serial.println("PETSR2_MOVE_LEFT_ROTATION");
      controller.leftRotation();
      break;        
    default:
      // controller.stopMove();
      break;
  }
}

void loop() {
  // Serial.println("loop");
  int value = digitalRead(BUTTON_0);
  if (value == LOW) {
    Serial.println("button0State LOW");
    controller.stopMove();
    delay(120);
    return;
  }

  encoderL->tick();
  int newPosL = encoderL->getPosition();
  if (posL != newPosL) { 
    // v: -1 前進 v: 1 後進
    vL = (int)(encoderL->getDirection());
    posL = newPosL;
  } else {
    // 変化がなかった
    unsigned long time = millis();
    if (time - oldTimeL > TIMEOUT_SEC * 1000) {
      vL = 0;
      oldTimeL = time;
    }
  }

  encoderR->tick(); 
  int newPosR = encoderR->getPosition();
  if (posR != newPosR) {
    // v: -1 前進 v: 1 後進
    vR = (int)(encoderR->getDirection());
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

  motor(vL, vR);
}
