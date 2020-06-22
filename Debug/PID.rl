L 1 "../PID.cpp"
N/*
N * PID.cpp
N *
N *  Created on: Jun 21, 2020
N *      Author: Juan Francisco Valle
N */
N
N#include "PID.h"
L 1 "..\PID.h" 1
N/*
N * PID.h
N *
N *  Created on: Jun 21, 2020
N *      Author: Juan Francisco Vallejo
N */
N
N#ifndef PID_H_
N#define PID_H_
N
Nclass PID{
Npublic:
N    double Kp = 0;
N    double Ki = 0;
N    double Kd = 0;
N
N    double setpoint;
N    double measuredState;
N    double error;
N    double prevError = 0;
N    double acumError = 0;
N    double errorRate;
N    double dt;
N    double antiWindup;
N
N    double out_d;
N    double out_p;
N    double out_i;
N    double output;
N
N    PID(double Kp, double Ki, double Kd, double dt, double antiWindup = 225);
N
N    double updatePID(double setpoint, double input);
N
N    double updatePID(double setpoint, double input, double inputRate);
N
N
N};
N
N
N
N
N#endif /* PID_H_ */
L 9 "../PID.cpp" 2
N
NPID::PID(double Kp, double Ki, double Kd, double dt, double antiWindup):
N         Kp(Kp), Kd(Kd), Ki(Ki), antiWindup(antiWindup), dt(dt){}
N
Ndouble PID::updatePID(double setpoint, double input){
N    this->setpoint = setpoint;
N    this->measuredState = input;
N    error = this->setpoint - input;
N    out_p = Kp*error;
N    errorRate = (error - prevError)/dt;
N    out_d = Kp*Kd*errorRate;
N    if((acumError + error)*Kp*Ki<antiWindup)acumError+=(error);
N    out_i = Kp*Ki*acumError;
N    output = out_p + out_d + out_i;
N    prevError = error;
N    return output;
N}
N
Ndouble PID::updatePID(double setpoint, double input, double inputRate){
N    this->setpoint = setpoint;
N    this->measuredState = input;
N    error = this->setpoint - input;
N    out_p = Kp*error;
N    errorRate = -inputRate;
N    out_d = Kp*Kd*errorRate;
N
N    if((acumError + error)*Kp*Ki<antiWindup)acumError+=(error);
N    out_i = Kp*Ki*acumError;
N    output = out_p + out_d + out_i;
N    prevError = error;
N    return output;
N}
N
N
N
R "../PID.cpp" 10 6 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 11 10 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "" 0 0 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 13 13 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 14 20 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 15 25 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 16 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 17 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 18 15 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 19 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 20 33 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 21 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 22 12 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 23 15 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 27 13 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 28 20 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 29 25 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 30 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 31 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 32 15 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 33 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 35 33 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 36 11 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 37 12 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
R "../PID.cpp" 38 15 (ULP 5.2) Detected floating point operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
