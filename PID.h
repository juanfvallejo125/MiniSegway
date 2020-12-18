/*
 * PID.h
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Vallejo
 */

#ifndef PID_H_
#define PID_H_

#include <string>
#include <sstream>
#include <iomanip>

class PID{
public:
    double Kp = 0;
    double Ki = 0;
    double Kd = 0;

    double setpoint;
    double measuredState;
    double error;
    double prevError = 0;
    double acumError = 0;
    double errorRate;
    double dt;
    double antiWindup;

    double out_d;
    double out_p;
    double out_i;
    double output;

    PID(double Kp, double Ki, double Kd, double dt, double antiWindup = 225);

    double updatePID(double setpoint, double input);

    double updatePID(double setpoint, double input, double inputRate);

    std::string getPIDconfig();




};




#endif /* PID_H_ */
