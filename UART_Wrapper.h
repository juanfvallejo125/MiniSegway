/*
 * UART_Wrapper.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef UART_WRAPPER_H_
#define UART_WRAPPER_H_

#include <stdint.h>
#include <driverlib.h>
#include "Odometry.h"
#include "IMU.h"
#include "MiniSegway.h"

class Odometry;
class IMU;
class PID;

class UART{
private:
    eUSCI_UART_Config UART_init;
    Odometry* odom;
    IMU* imu;

public:
    char buffer[200] = {};// Set to 200, should be large enough
    int messageSize = 0;
    volatile int bufferIndex = 0;
    uint32_t UARTModule;
    bool finishedTransmission = 1;

    UART(eUSCI_UART_Config init, uint32_t UARTModule, Odometry* odom, IMU* imu);

    void printEncoders();

    void UARTSetup();

    void printOdometry();

    void printIMU();

    void printPWM();

    void printPID(PID& pid);

};

#endif /* UART_WRAPPER_H_ */
