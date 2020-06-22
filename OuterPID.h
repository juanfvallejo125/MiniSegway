/*
 * OuterPID.h
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef OUTERPID_H_
#define OUTERPID_H_

#include "PID.h"
#include "LPF.h"

class OuterPID : public PID{
public:
    PID* innerPID;
    double alpha;
    double filteredOutput[2] = {};

    OuterPID(double Kp, double Ki, double Kd, double dt, PID* innerPID, double alpha,double antiWindup = 225);

    double updatePID(double setpoint, double input, double innerInput,double innerInputRate);

};



#endif /* OUTERPID_H_ */
