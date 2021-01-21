/*
 * RFInterface.h
 *
 *  Created on: Jun 21, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef RFINTERFACE_H_
#define RFINTERFACE_H_

#include "MiniSegway.h"

class RFInterface{
public:
    int mode = 0;

    double filteredVelocitySetpoint[2] = {};
    double filteredOrientationSetpoint[2] = {};

    double alphaVelocitySetpoint = 0.1;
    double alphaOrientationSetpoint = 1;
    double rawVelocitySetpoint = 0;
    double rawOrientationSetpoint = 0;

    // Use of 3 channels
    uint16_t pulseStart[3] = {0};
    uint16_t pulseEnd[3] = {0};
    uint16_t pulseLength[3] = {0};
    uint16_t zeroValue[3] = {0};

    void pollMode();
    void pollrfReceiver();
    void calibrate();
};



#endif /* RFINTERFACE_H_ */
