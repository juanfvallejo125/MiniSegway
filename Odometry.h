/*
 * Odometry.h
 *
 *  Created on: Jun 16, 2020
 *      Author: Juan Francisco Vallejo
 */

#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include <stdint.h>
#include <driverlib.h>
#include "Motor.h"
#include <cmath>
#include "LPF.h"


class Odometry
{
private:
    double wheelBase;
    double wheelRadius;
    double ticksPerRev;
    double dt;
    double alpha;

public:
    double turningRate = 0;
    double speed = 0;
    double x = 0;
    double y = 0;
    double theta = 0;
    double rightAngRate[2] = {};
    double leftAngRate[2] = {};
    double prevRightCount = 0;
    double prevLeftCount = 0;
    Motor* rightMotor;
    Motor* leftMotor;


    Odometry(double wheelBase, double wheelRadius, double ticksPerRev, Motor* rightMotor, Motor* leftMotor, double dt, double alpha);


    void updateOdometry();
};

#endif /* ODOMETRY_H_ */
