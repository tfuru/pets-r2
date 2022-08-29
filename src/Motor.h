#ifndef __MOTOR_H__
#define __MOTOR_H__

class Motor {
public:
    Motor();
    ~Motor();

    bool begin();
    void forward();
    void backward();
    void stopMove();
    void rightRotation();
    void leftRotation();    
};

#endif //__MOTOR_H__