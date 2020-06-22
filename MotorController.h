/*
 * MotorController.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include "Motor.h"

class MotorController{
private:
    Motor* rightMotor;
    Motor* leftMotor;

public:

    MotorController(Motor* rightMotor, Motor* leftMotor);

    void commandMotors(double* const req_PWMs);

};





#endif /* MOTORCONTROLLER_H_ */
