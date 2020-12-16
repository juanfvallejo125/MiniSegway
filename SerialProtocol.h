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


class UART;
class PID;
class OuterPID;

class SerialProtocol
{
public:
    std::string line = "";
    char charIn = NULL;
    UART* uart = NULL;
    PID* pid = NULL;
    OuterPID* outerPID = NULL;
    PID* innerPID = NULL;
    std::string acknowledgeMessage = "";
    std::vector <uint8_t> commandVector;
    std::vector <double> valueVector;
    SerialProtocol(UART* uart, OuterPID* outerPID, PID* innerPID);
    void executeProtocol();
    void parseLine();
    void processCommand();
    void lineToUpper(std::string& inputLine);
};

#endif /* SERIALPROTOCOL_H_ */
