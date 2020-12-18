/*
 * SerialProtocol.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef SERIALPROTOCOL_H_
#define SERIALPROTOCOL_H_

#include "MiniSegway.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <iomanip>


class UART;
class PID;
class OuterPID;
class IMU;

class SerialProtocol
{
public:
    std::string line = "";
    char charIn = NULL;
    UART* uart = NULL;
    PID* pid = NULL;
    OuterPID* outerPID = NULL;
    PID* innerPID = NULL;
    IMU* imu = NULL;
    std::string acknowledgeMessage = "";
    std::vector <uint8_t> commandVector;
    std::vector <double> valueVector;
    SerialProtocol(UART* uart, OuterPID* outerPID, PID* innerPID, IMU* imu);
    void executeProtocol();
    void parseLine();
    void processCommand();
    void lineToUpper(std::string& inputLine);
};

#endif /* SERIALPROTOCOL_H_ */
