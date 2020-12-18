/*
 * IMU.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Vallejo
 */

#ifndef IMU_H_
#define IMU_H_

#include "LSM6DSOX.h"
#include <cmath>
#include "driverlib.h"
#include "msp.h"
#include "SPI.h"
#include "MiniSegway.h"

class IMU{
public:
    double acceleration[3] = {};
    double angularRate[3] = {};
    double accelerationZero[3] = {};
    double angularRateZero[3] = {};
    double angle = 0;
    double angleRate = 0;
    double angleAccel = 0;
    double alpha = 0.98;
    double dt = 0.01;
    double accelOffset = 3;
    uint8_t TXDataBuffer[10];
    uint8_t RXDataBuffer[10];

    IMU();
    void configModule();
    void calibrate();
    void readAccelerometer();
    void readGyroscope();
    double getFilteredAngle();
};

#endif /* IMU_H_ */
