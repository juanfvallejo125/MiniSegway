/*
 * RFInterface.cpp
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#include "RFInterface.h"

void RFInterface::pollrfReceiver(){
    bool A = (bool)GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN0)==GPIO_INPUT_PIN_HIGH;
    bool C = (bool)GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN3)==GPIO_INPUT_PIN_HIGH;
    bool B = (bool)GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN2)==GPIO_INPUT_PIN_HIGH;
    bool D = (bool)GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH;

    if(mode == 0){
        //setPWM(PWM_increase_A, PWM_increase_B); //Motor testing
        if(A){// Button A
            //PWM_increase_A += 2;
        }if(C){// Button C
            //Deadzone testing
            //motorPWM += 1;
            //printf("Motor PWM %f\n", motorPWM);
            //setPWM(motorPWM, motorPWM);
        }if(B){// Button B
            //PWM_increase_B += 2;
            mode = 1;
            tiltController.Ki=Ki_tilt;
            tiltController.acumError = 0;
            velocityController.Ki=Ki_velocity;
            velocityController.acumError = 0;
            turningController.Ki = Ki_turning;
            turningController.acumError = 0;
        }if(D){// Button D
            //PWM_increase_B -= 2;
        }if(!(A||C)){// Nor A or C
            //PWM_increase_A = 0;
        }if(!(B||D)){// Nor B or D
            //PWM_increase_B = 0;
        }
    }

    if(mode == 1){
        //setPWM(PWM_increase_A, PWM_increase_B); //Motor testing
        if(A){// Button A
            //move forward
//            if(tiltController.setpoint<3){
//                tiltController.setpoint += 1;
//            }else{
//                tiltController.setpoint = 3;
//            }
            LPF(250, filteredVelocitySetpoint, alphaVelocitySetpoint);
            velocityController.setpoint = filteredVelocitySetpoint[0];
            //pwmOffsetA_throt = 40;
            //pwmOffsetB_throt = 40;
        }if(C){// Button C
            //Move backward
//            if(tiltController.setpoint>-3){
//                tiltController.setpoint += -1;
//            }else{
//                tiltController.setpoint = -3;
//            }
            LPF(-250, filteredVelocitySetpoint, alphaVelocitySetpoint);
            velocityController.setpoint = filteredVelocitySetpoint[0];
            //pwmOffsetA_throt = -40;
            //pwmOffsetB_throt = -40;
        }if(B){// Button B
            //Right turn
            LPF(-1.5, filteredOrientationSetpoint, alphaOrientationSetpoint);
            turningController.setpoint = filteredOrientationSetpoint[0];
        }if(D){// Button D
            //Left turn
            LPF(1.5, filteredOrientationSetpoint, alphaOrientationSetpoint);
            turningController.setpoint = filteredOrientationSetpoint[0];
        }if(!(A||C)){// Nor A or C
//            if(tiltController.setpoint>0){
//                tiltController.setpoint -= 0.5;
//            }else if(tiltController.setpoint<0){
//                tiltController.setpoint += 0.5;
//            }
            velocityController.setpoint = 0;
        }if(!(B||D)){// Nor B or D
            turningController.setpoint = 0;
        }
   }
}

