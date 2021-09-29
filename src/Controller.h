#ifndef PETSR2_CONTROLLER_H
#define PETSR2_CONTROLLER_H

#include <Arduino.h>

// 左右レバー 前後
#define PETSR2_LEVER_R 16
#define PETSR2_LEVER_L 4

#define PETSR2_POWER 5

class Controller {
public:
    Controller();
    ~Controller();

    bool begin();

    void forward();
    void backward();
    void stopMove();
    void rightRotation();
    void leftRotation();
};

#endif