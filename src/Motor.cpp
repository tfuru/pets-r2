#include "config.h"
#include "Motor.h"

Motor::Motor() {

}

Motor::~Motor() {

}

// PWM 
// https://blog.goo.ne.jp/jh7ubc/e/4740cd8da9def6fafad48f0745359371
bool Motor::begin() {
    pinMode(MOTOR_DR1, OUTPUT);
    pinMode(MOTOR_EN1, OUTPUT);
    digitalWrite(MOTOR_EN1, LOW);

    pinMode(MOTOR_DR2, OUTPUT);
    pinMode(MOTOR_EN2, OUTPUT);
    digitalWrite(MOTOR_EN2, LOW);

    /*
    int chan = 0; // チャンネル
    int freq = 1000; // 周波数
    int bit_num = 8; // 256
    ledcSetup(chan, freq, bit_num);
    ledcAttachPin(MOTOR_PWM1, chan);
    ledcAttachPin(MOTOR_PWM2, chan);

    ledcWrite(MOTOR_PWM1, 128);
    ledcWrite(MOTOR_PWM2, 128);
    */
    pinMode(MOTOR_PWM1, OUTPUT);
    digitalWrite(MOTOR_PWM1, HIGH);
    pinMode(MOTOR_PWM2, OUTPUT);
    digitalWrite(MOTOR_PWM2, HIGH);

    // stopMove();

    // forward();
    // backward();
    // rightRotation();
    // leftRotation();

    return true;
}

void Motor::forward() {
    Serial.println("Motor::forward");

    digitalWrite(MOTOR_DR1, HIGH);
    digitalWrite(MOTOR_EN1, HIGH);
    digitalWrite(MOTOR_DR2, HIGH);
    digitalWrite(MOTOR_EN2, HIGH);
}

void Motor::backward() {
    Serial.println("Motor::backward");

    digitalWrite(MOTOR_DR1, LOW);
    digitalWrite(MOTOR_EN1, HIGH);
    digitalWrite(MOTOR_DR2, LOW);
    digitalWrite(MOTOR_EN2, HIGH);
}

void Motor::stopMove() {
    // Serial.println("Motor::stopMove");
    digitalWrite(MOTOR_EN1, LOW);
    digitalWrite(MOTOR_EN2, LOW);
}

void Motor::rightRotation() {
    Serial.println("Motor::rightRotation");

    digitalWrite(MOTOR_DR1, HIGH);
    digitalWrite(MOTOR_EN1, HIGH);

    digitalWrite(MOTOR_EN2, LOW);
}

void Motor::leftRotation() {
    Serial.println("Motor::leftRotation");

    digitalWrite(MOTOR_DR2, HIGH);
    digitalWrite(MOTOR_EN2, HIGH);

    digitalWrite(MOTOR_EN1, LOW);
}