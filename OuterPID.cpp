/*
 * OuterPID.cpp
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#include "OuterPID.h"

OuterPID::OuterPID(double Kp, double Ki, double Kd, double dt, PID* innerPID, double alpha,double antiWindup):
                    PID(Kp, Ki, Kd, dt, antiWindup), alpha(alpha), innerPID(innerPID){}

double OuterPID::updatePID(double setpoint, double input, double innerInput, double innerInputRate){
    this->PID::updatePID(setpoint, input);
    LPF(output, filteredOutput, alpha);
    innerPID->updatePID(filteredOutput[0], innerInput, innerInputRate);
    return innerPID->output;
}




