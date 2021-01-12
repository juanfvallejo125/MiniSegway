/*
 * Motor.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <driverlib.h>

class Motor{
private:

    volatile uint16_t * pwmRegister;

public:
    int16_t maxPWM;
    int16_t currentPWM;
    uint16_t forwardPort;
    uint32_t forwardPin;
    uint16_t backwardPort;
    uint32_t backwardPin;

    uint16_t enc_portA;
    uint32_t enc_pinA;
    uint16_t enc_portB;
    uint32_t enc_pinB;

    volatile long enc_count;
    volatile long previous_enc_count;
    volatile double enc_velocity;

    volatile long prev_tick;

    //Constructor
    Motor(uint8_t maxPWM, uint16_t forwardPort, uint32_t forwardPin, uint16_t backwardPort, uint32_t backwardPin, volatile uint16_t * pwmRegister,
          uint16_t enc_portA, uint32_t enc_pinA,  uint16_t enc_portB, uint32_t enc_pinB);

    // Getters and setters
    void setPWM(int16_t pwm);
    uint8_t getPWM();

    void forward();
    void backward();
    void calculateEncVelocity();
};


#endif /* MOTOR_H_ */
