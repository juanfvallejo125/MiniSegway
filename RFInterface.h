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

    double alphaVelocitySetpoint = 1;
    double alphaOrientationSetpoint = 1;

    void pollrfReceiver();
};



#endif /* RFINTERFACE_H_ */
