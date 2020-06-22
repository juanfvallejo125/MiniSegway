/*
 * MotorController.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Valle
 */

#include "MotorController.h"

MotorController::MotorController(Motor* rightMotor, Motor* leftMotor): rightMotor(rightMotor), leftMotor(leftMotor){}

void MotorController::commandMotors(double* const req_PWMs){
    rightMotor->setPWM((int16_t) req_PWMs[0]);
    leftMotor->setPWM((int16_t) req_PWMs[1]);
}




