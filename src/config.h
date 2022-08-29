#ifndef PETSR2_CONFIG_H
#define PETSR2_CONFIG_H

#include <Arduino.h>

typedef struct {
  char type;
  int value;
} PETSR2_STRUCT_COMAND;

// #define PETSR2_ESP_NOW_CHANNEL 0
// static uint8_t PETSR2_ESP_NOW_CONTROLLER_MAC_ADDRESS[]  = {0x7C, 0x9E, 0xBD, 0xF2, 0x11, 0x74};

#define PETSR2_MOVE_DEFAULT 0
#define PETSR2_MOVE_FORWARD 1
#define PETSR2_MOVE_BACKWARD 2
#define PETSR2_MOVE_STOP 3
#define PETSR2_MOVE_RIGHT_ROTATION 4
#define PETSR2_MOVE_LEFT_ROTATION 5

#define PETSR2_MOVE_KEY_DEFAULT ' '
#define PETSR2_MOVE_KEY_FORWARD 'w'
#define PETSR2_MOVE_KEY_BACKWARD 'x'
#define PETSR2_MOVE_KEY_STOP 's'
#define PETSR2_MOVE_KEY_RIGHT_ROTATION 'd'
#define PETSR2_MOVE_KEY_LEFT_ROTATION 'a'

// ロータリーエンコーダー
#define ENCODER_R_A 12
#define ENCODER_R_B 13
#define ENCODER_L_A 27
#define ENCODER_L_B 14

#define BUTTON_0 0

// モーター
#define MOTOR_DR1 4
#define MOTOR_PWM1 32
#define MOTOR_EN1 17

#define MOTOR_DR2 5
#define MOTOR_PWM2 33
#define MOTOR_EN2 19

#endif