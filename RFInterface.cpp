/*
 * RFInterface.cpp
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#include "RFInterface.h"

void RFInterface::calibrate(){
    int i;
    int ch1_acum = 0;
    int ch2_acum = 0;
    for(i=0; i<100; i++){
        ch1_acum += pulseLength[0];
        ch2_acum += pulseLength[1];
        __delay_cycles(DCO_Freq/1000); // Delay 1 ms
    }
    zeroValue[0] = ch1_acum/100;
    zeroValue[1] = ch1_acum/100;
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);//Light up LED to indicate we are out of calibration
}

void RFInterface::pollMode(){
    if(this->pulseLength[2] >= 1500){ // If the channel 3 is lit up, the system is in mode 1
        this->mode = 1;
    }
    if(this->pulseLength[2] < 1500){ // If the channel 3 is off, the system is in mode 0
        this->mode = 0;
    }
    if(mode == 0){
        tiltController.acumError = 0;
        velocityController.acumError = 0;
        turningController.acumError = 0;
    }
}

void RFInterface::pollrfReceiver(){

        //setPWM(PWM_increase_A, PWM_increase_B); //Motor testing
        if(selectedMode == angleMode){
            tiltController.setpoint = 0.003*(commandInterface.pulseLength[1]-commandInterface.zeroValue[1]);
            if(tiltController.setpoint>3){
                tiltController.setpoint = 3;
            }else if(tiltController.setpoint < -3){
                tiltController.setpoint = -3;
            }
        }
        else if(selectedMode == velocityMode){
            rawVelocitySetpoint = 0.7*(commandInterface.pulseLength[1]-commandInterface.zeroValue[1]);
            if(rawVelocitySetpoint >= 800) rawVelocitySetpoint = 800;
            else if(rawVelocitySetpoint <= -800) rawVelocitySetpoint = -800;

        }
        rawOrientationSetpoint = 0.007*(commandInterface.pulseLength[0] - commandInterface.zeroValue[0]);
        if(rawOrientationSetpoint <= -300) rawOrientationSetpoint = -3;
        else if(rawOrientationSetpoint >= 300) rawOrientationSetpoint = 3;

        LPF(rawVelocitySetpoint, filteredVelocitySetpoint, alphaVelocitySetpoint);
        velocityController.setpoint = filteredVelocitySetpoint[0];
        LPF(rawOrientationSetpoint, filteredOrientationSetpoint, alphaOrientationSetpoint);
        turningController.setpoint = filteredOrientationSetpoint[0];
}

