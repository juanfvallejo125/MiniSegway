/*
 * UART_Wrapper.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef UART_WRAPPER_H_
#define UART_WRAPPER_H_

#include "MiniSegway.h"
#include <stdint.h>
#include <driverlib.h>
#include "Odometry.h"
#include "IMU.h"
#include <string>
#include <cstring>
#include <iomanip>

class Odometry;
class IMU;
class PID;
class RFInterface;

class UART{
private:
    eUSCI_UART_Config UART_init;
    Odometry* odom;
    IMU* imu;

public:
    char ringBufferRX[RX_BUFFER_SIZE] = {};// Ring buffer for data reception
    volatile int bufferRXHead = 0;
    volatile int bufferRXTail = 0;
    char ringBufferTX[TX_BUFFER_SIZE] = {};// Buffer for data transmission
    volatile int bufferTXHead = 0;
    volatile int bufferTXTail = 0;
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

//    void printPIDconfig(PID& pid);

    void printRF();

    char serialRead();

    void serialWrite(const char* buffer, size_t len);

    void serialWrite(char* buffer, size_t len);

    int serialGetBufferAvailable();

    void echoRead();

    void printLine(std::string message);




};

#endif /* UART_WRAPPER_H_ */
