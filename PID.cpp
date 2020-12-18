/*
 * PID.cpp
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#include "PID.h"

PID::PID(double Kp, double Ki, double Kd, double dt, double antiWindup):
         Kp(Kp), Kd(Kd), Ki(Ki), antiWindup(antiWindup), dt(dt){}

double PID::updatePID(double setpoint, double input){
    this->setpoint = setpoint;
    this->measuredState = input;
    error = this->setpoint - input;
    out_p = Kp*error;
    errorRate = (error - prevError)/dt;
    out_d = Kp*Kd*errorRate;
    if((acumError + error)*Kp*Ki<antiWindup)acumError+=(error);
    out_i = Kp*Ki*acumError;
    output = out_p + out_d + out_i;
    prevError = error;
    return output;
}

double PID::updatePID(double setpoint, double input, double inputRate){
    this->setpoint = setpoint;
    this->measuredState = input;
    error = this->setpoint - input;
    out_p = Kp*error;
    errorRate = -inputRate;
    out_d = Kp*Kd*errorRate;

    if((acumError + error)*Kp*Ki<antiWindup)acumError+=(error);
    out_i = Kp*Ki*acumError;
    output = out_p + out_d + out_i;
    prevError = error;
    return output;
}

std::string PID::getPIDconfig(){
    std::ostringstream stream;
    stream << std::setprecision(4) << "P: " << this->Kp << ", I: " << this->Ki << " , D: " << this->Kd;
    return stream.str();
}



