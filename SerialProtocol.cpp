/*
 * SerialProtocol.cpp
 *
 *  Created on: Dec 7, 2020
 *      Author: Juan Francisco Valle
 */

#include "SerialProtocol.h"



SerialProtocol::SerialProtocol(UART* uart, OuterPID* outerPID, PID* innerPID) : uart(uart), outerPID(outerPID), innerPID(innerPID){
    pid = outerPID; // Default the outer PID
}

void SerialProtocol::executeProtocol(){
    while(uart->serialGetBufferAvailable() > 0){
        charIn = uart->serialRead();
        if(charIn == '\n') { // This is an endline
            this->parseLine();
            this->processCommand();
            this->line.clear();
        }
        else{
            line.push_back(charIn);
        }
    }
}

void SerialProtocol::parseLine(){
    std::string errormsg = "";
    std::string argument = "";
    std::string variable = "";
    std::string valueStr = "";
    std::istringstream stream(line);
    double value = 0;
    bool validVariable = false;
    while(stream >> argument){ // While there's still arguments in the line

        lineToUpper(argument); // Convert to uppercase

        std::istringstream argumentStream(argument);

        if(argument.compare("OUTER") == 0){
            uart->printLine(std::string("Pushed Back OUTER"));
            commandVector.push_back(0);
            valueVector.push_back(1);
        }
        else if(argument.compare("INNER") == 0){
            uart->printLine(std::string("Pushed Back INNER"));
            commandVector.push_back(1);
            valueVector.push_back(1);
        }
        else if(std::getline(argumentStream, variable, '=')){
            validVariable = false;
            if(variable.compare("P") == 0){
                uart->printLine(std::string("Pushed Back P"));
                commandVector.push_back(2);
                validVariable = true;
            }
            else if(variable.compare("I") == 0){
                uart->printLine(std::string("Pushed Back I"));
                commandVector.push_back(3);
                validVariable = true;
            }
            else if(variable.compare("D") == 0){
                uart->printLine(std::string("Pushed Back D"));
                commandVector.push_back(4);
                validVariable = true;
            }
            if(validVariable){
                getline(argumentStream, valueStr, '=');
                std::istringstream valueStream(valueStr);
                valueStream >> value;
                valueVector.push_back(value);
            }
        }
        else{
            // Probably throw an exception to handle what would happen with wrong input
        }
    }

}

void SerialProtocol::processCommand(){
    std::ostringstream os;
    while(commandVector.size()>0){
        uint8_t command = commandVector.front();
        commandVector.erase(commandVector.begin());
        switch(command){
            case 0: // User typed in Outer must go into outer PID mode
                pid = outerPID;
                acknowledgeMessage.append("Outer PID mode ");
                break;
            case 1:
                pid = innerPID;
                acknowledgeMessage.append("Inner PID Mode ");
                break;
            case 2:
                pid->Kp = valueVector.at(2);
                os << "Kp: " << pid->Kp << " ";
                break;
            case 3:
                pid->Ki = valueVector.at(3);
                os << "Ki: " << pid->Ki << " ";
                break;
            case 4:
                pid->Kd = valueVector.at(4);
                os << "Kd: " << pid->Kd << " ";
                break;
        }
    }
    acknowledgeMessage.append(os.str());
    uart->printLine(acknowledgeMessage);
    acknowledgeMessage.clear();
}

void SerialProtocol::lineToUpper(std::string& inputLine){
    for(uint8_t i = 0; i < inputLine.length(); i++){
        inputLine[i] = toupper(inputLine[i]);
    }
}

