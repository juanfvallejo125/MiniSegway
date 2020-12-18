/*
 * UART_Wrapper.cpp
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Valle
 */

#include "UART_Wrapper.h"

UART::UART(eUSCI_UART_Config init, uint32_t Module, Odometry* odom, IMU* imu):UART_init(init), UARTModule(Module),
            odom(odom), imu(imu){}

void UART::UARTSetup(){
    // Configure P1.2 and P1.3 as UART TX/RX
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    // Initialize UART Module 0
    UART_initModule(UARTModule, &UART_init);
    UART_enableModule(UARTModule);
    UART_enableInterrupt(UARTModule, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    }

void UART::printOdometry(){
    std::string message;
    std::ostringstream stream;
    stream << std::setprecision(4) << "Speed: " << odom->speed << ", Turning Rate: " << odom->turningRate << " , Theta: " << odom->theta << ", X: " << odom->x << ", Y: " << odom->y;
//    messageSize = sprintf(buffer, "Speed: %.4f, Turning Rate: %.4f, Theta: %.4f, X: %.4f, Y: %.4f\r\n", Speed:, odom->turningRate, odom->theta, odom->x, odom->y);
    printLine(stream.str());
}
//
void UART::printEncoders(){
    std::string message;
    std::ostringstream stream;
    stream << std::setprecision(4) << "Right encoder: " << odom->rightMotor->enc_count << ", velocity: " << odom->rightMotor->enc_velocity << " , Left encoder: " << odom->leftMotor->enc_count << ", velocity: " << odom->leftMotor->enc_velocity;
//    messageSize = sprintf(buffer, "Right encoder: %i, velocity: %.4f, Left encoder: %i, velocity: %0.4f\r\n", odom->rightMotor->enc_count, odom->rightMotor->enc_velocity, odom->leftMotor->enc_count, odom->leftMotor->enc_velocity);
    printLine(stream.str());
}
//
void UART::printIMU(){
    std::string message;
    std::ostringstream stream;
    stream << std::setprecision(4) << "Angle: " << imu->angle << ", Angular Rate: " << imu->angleRate << " , Angle Accel: " << imu->angleAccel;
//    messageSize = sprintf(buffer, "Angle: %.4f, Angular Rate: %.4f, Angle Accel: %.4f\r\n", imu->angle, imu->angleRate, imu->angleAccel);
    printLine(stream.str());
}
//
void UART::printPWM(){
    std::string message;
    std::ostringstream stream;
    stream << std::setprecision(4) << "PWM Right: " << odom->rightMotor->currentPWM << ", PWM Left " << odom->rightMotor->currentPWM;
//    messageSize = sprintf(buffer, "PWM Right: %i, PWM Left: %i\r\n", odom->rightMotor->currentPWM, odom->leftMotor->currentPWM);
    printLine(stream.str());
}

void UART::printPID(PID& pid){
    std::string message;
    std::ostringstream stream;
    stream << std::setprecision(4) << "Setpoint: " << pid.setpoint << ", PID Output: " << pid.output << " , Out P: " << pid.out_p << ", Out D: " << pid.out_d << ", Out I: " << pid.out_i;
    //messageSize = sprintf(buffer, "Setpoint: %.2f, PID Output: %.2f, Out D: %.2f, Out I: %.2f, Out P: %.2f\r\n", pid.setpoint, pid.output, pid.out_d, pid.out_i, pid.out_p);
    printLine(stream.str());
}

//void UART::printPIDconfig(PID& pid){
//    std::ostringstream stream;
//    stream << "P: " << pid.Kp << ", I: " << pid.Ki << " , D: " << pid.Kd;
//    printLine(stream.str());
//}

void UART::printRF(){
    std::string message;
    std::ostringstream stream;
    stream << "CH1: " << commandInterface.pulseLength[0] << ", CH2: " << commandInterface.pulseLength[1] << ", CH3: " << commandInterface.pulseLength[2] << ", mode: " << commandInterface.mode;
    //messageSize = sprintf(buffer, "CH1: %i, CH2: %i, CH3: %i, mode: %i\r\n", commandInterface.pulseLength[0], commandInterface.pulseLength[1], commandInterface.pulseLength[2], commandInterface.mode);
    printLine(stream.str());
}

int UART::serialGetBufferAvailable(){
    if(bufferRXHead >= bufferRXTail){ return(bufferRXHead - bufferRXTail); }
    return(RX_BUFFER_SIZE - (bufferRXTail - bufferRXHead));
}

char UART::serialRead(){
    if(bufferRXTail == bufferRXHead){ return '~'; } // If head and tail are the same, there's no data
    char data = ringBufferRX[bufferRXTail];
    bufferRXTail++;
    if(bufferRXTail == RX_BUFFER_SIZE){ bufferRXTail = 0; }
    return data;
}

void UART::serialWrite(const char* buffer, size_t len){
    for(int i = 0; i < len; ++i){
        while((bufferTXHead+1) == bufferTXTail);// Hang on until the buffer has some free space
        ringBufferTX[bufferTXHead] = buffer[i];
        bufferTXHead++;
        if(bufferTXHead == TX_BUFFER_SIZE){ bufferTXHead = 0;}
        UART_enableInterrupt(UARTHandler.UARTModule, EUSCI_A_UART_TRANSMIT_INTERRUPT);// Automatically send everything
    }
}

void UART::serialWrite(char* buffer, size_t len){
    this->serialWrite((const char*) buffer, len);
}

void UART::printLine(std::string message){
    message.push_back('\n');
    serialWrite(message.c_str(), message.length());
}

void UART::echoRead(){
    char readChar = this->serialRead();
    const char* charPoint = &readChar;
    serialWrite(charPoint, 1);
}
