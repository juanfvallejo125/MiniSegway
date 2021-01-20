/*
 * SerialProtocol.cpp
 *
 *  Created on: Dec 7, 2020
 *      Author: Juan Francisco Valle
 */

#include "SerialProtocol.h"


SerialProtocol::SerialProtocol(UART* uart, OuterPID* outerPID, PID* innerPID, PID* turningPID, IMU* imu) : uart(uart), outerPID(outerPID), innerPID(innerPID), turningPID(turningPID), imu(imu){
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
    std::string argument = "";
    std::string variable = "";
    std::string valueStr = "";
    std::istringstream stream(line);
    double value = 0;
    bool validVariable = false;
    while(stream >> argument){ // While there's still arguments in the line

        lineToUpper(argument); // Convert to uppercase

        std::istringstream argumentStream(argument);

        if(argument == "OUTER"){
//            uart->printLine(std::string("Pushed Back OUTER"));
            commandVector.push_back(0);
            valueVector.push_back(0);
        }
        else if(argument == "INNER"){
//            uart->printLine(std::string("Pushed Back INNER"));
            commandVector.push_back(1);
            valueVector.push_back(0);
        }
        else if(argument == "TURNING"){
//            uart->printLine(std::string("Pushed Back INNER"));
            commandVector.push_back(13);
            valueVector.push_back(0);
        }
        else if(argument == "SHOW-PID-CONFIG"){
            commandVector.push_back(5);
            valueVector.push_back(0);
        }
        else if(argument == "ANGLE-MODE"){
            commandVector.push_back(6);
            valueVector.push_back(0);
        }
        else if(argument == "VEL-MODE"){
            commandVector.push_back(7);
            valueVector.push_back(0);
        }
        else if(argument == "SHOW-ACCEL-ANGLE"){
            commandVector.push_back(8);
            valueVector.push_back(0);
        }
        else if(argument == "SHOW-SETPOINT"){
            commandVector.push_back(9);
            valueVector.push_back(0);
        }
        else if(argument == "SEND-DATA"){
            commandVector.push_back(11);
            valueVector.push_back(0);
        }
        else if(argument == "STOP-DATA"){
            commandVector.push_back(12);
            valueVector.push_back(0);
        }
        else if(argument.find('=') != std::string::npos){// Check for an assignment command
            std::getline(argumentStream, variable, '=');
            validVariable = false;
            if(variable == "P"){
//                uart->printLine(std::string("Pushed Back P"));
                commandVector.push_back(2);
                validVariable = true;
            }
            else if(variable == "I"){
//                uart->printLine(std::string("Pushed Back I"));
                commandVector.push_back(3);
                validVariable = true;
            }
            else if(variable == "D"){
//                uart->printLine(std::string("Pushed Back D"));
                commandVector.push_back(4);
                validVariable = true;
            }
            else if(variable == "VEL-ALPHA"){
                commandVector.push_back(10);
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
            // Other unknown command
            commandVector.push_back(99); // Unknown command
            valueVector.push_back(0);
        }
    }

}

void SerialProtocol::processCommand(){
    std::ostringstream os;
    os << std::setprecision(4);
    while(commandVector.size()>0){
        uint8_t command = commandVector.front();
        double value = valueVector.front();
        commandVector.erase(commandVector.begin());
        valueVector.erase(valueVector.begin());
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
                pid->Kp = value;
                os << "Kp: " << pid->Kp << " ";
                break;
            case 3:
                pid->Ki = value;
                os << "Ki: " << pid->Ki << " ";
                break;
            case 4:
                pid->Kd = value;
                os << "Kd: " << pid->Kd << " ";
                break;
            case 5:
                os << pid->getPIDconfig() << " ";
                break;
            case 6:
                selectedMode = angleMode;
                os << "Angle Mode Selected ";
                break;
            case 7:
                selectedMode = velocityMode;
                os << "Velocity Mode Selected ";
                break;
            case 8:
                os << "Accel Angle: " << imu->angleAccel << " ";
                break;
            case 9:
                os << "Setpoint: " << pid->setpoint << " ";
                break;
            case 10:
                outerPID->alpha = value;
                os << "Alpha Vel: " << outerPID->alpha << " ";
                break;
            case 11:
                sendData = true;
                os << "Sending data... " ;
                break;
            case 12:
                sendData = false;
                os << "Stopped sending data " ;
                break;
            case 13:
                pid = turningPID;
                acknowledgeMessage.append("Turning PID Mode ");
                break;
            case 99:
                os << "Unknown command ";
                break;
        }
    }
    acknowledgeMessage.append(os.str());
    if(!acknowledgeMessage.empty()){
        uart->printLine(acknowledgeMessage);
    }
    acknowledgeMessage.clear();
}

void SerialProtocol::lineToUpper(std::string& inputLine){
    for(uint8_t i = 0; i < inputLine.length(); i++){
        inputLine[i] = toupper(inputLine[i]);
    }
}

