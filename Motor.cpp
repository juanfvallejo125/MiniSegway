/*
 * Motor.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Vallejo
 */

#include "Motor.h"

Motor::Motor(uint8_t maxPWM, uint16_t forwardPort, uint32_t forwardPin, uint16_t backwardPort, uint32_t backwardPin, volatile uint16_t * pwmRegister,
          uint16_t enc_portA, uint32_t enc_pinA,  uint16_t enc_portB, uint32_t enc_pinB)
          :enc_count(0), enc_velocity(0), prev_tick(0), maxPWM(maxPWM), forwardPort(forwardPort), forwardPin(forwardPin),
          backwardPort(backwardPort), backwardPin(backwardPin), pwmRegister(pwmRegister), enc_portA(enc_portA),
          enc_pinA(enc_pinA), enc_portB(enc_portB), enc_pinB(enc_pinB) {}

void Motor::setPWM(int16_t pwm){
    //Assure pwm doesn't exceed the maximum value
    if(pwm>maxPWM){
        pwm = maxPWM;
    }else if(pwm < -maxPWM){
        pwm = -maxPWM;
    }
    if(pwm>0){
        forward();
    }else{
        backward();
    }
    currentPWM = pwm;
    *pwmRegister = abs(pwm);
}

void Motor::forward(){
    GPIO_setOutputHighOnPin(forwardPort, forwardPin);
    GPIO_setOutputLowOnPin(backwardPort, backwardPin);
}

void Motor::backward(){
    GPIO_setOutputHighOnPin(backwardPort, backwardPin);
    GPIO_setOutputLowOnPin(forwardPort, forwardPin);

}

void Motor::calculateEncVelocity(){
    enc_velocity = 100*(enc_count - previous_enc_count);
    previous_enc_count = enc_count;
}


