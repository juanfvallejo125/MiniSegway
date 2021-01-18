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
//    std::string message;
//    std::ostringstream stream;
//    stream << std::setprecision(4) << "Speed: " << odom->speed << ", Turning Rate: " << odom->turningRate << ", Theta: " << odom->theta << ", Right_Angle_Rate: " << odom->rightAngRate[0] << ", Left_Angle_Rate: " << odom->leftAngRate[0] << ", X: " << odom->x << ", Y: " << odom->y;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "Speed: %.4f, Turning Rate: %.4f, Theta: %.4f, Right_Angle_Rate: %.4f, Left_Angle_Rate: %.4f, X: %.4f, Y: %.4f", odom->speed, odom->turningRate, odom->theta, odom->rightLinSpeed[0], odom->leftLinSpeed[0], odom->x, odom->y);
    printLine(buffer, messageSize);
}
//
void UART::printEncoders(){
//    std::string message;
//    std::ostringstream stream;
//    stream << std::setprecision(4) << "Right encoder: " << odom->rightMotor->enc_count << ", velocity: " << odom->rightMotor->enc_velocity << " , Left encoder: " << odom->leftMotor->enc_count << ", velocity: " << odom->leftMotor->enc_velocity;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "Right encoder: %i, velocity: %.4f, Left encoder: %i, velocity: %0.4f", odom->rightMotor->enc_count, odom->rightMotor->enc_velocity, odom->leftMotor->enc_count, odom->leftMotor->enc_velocity);
    printLine(buffer, messageSize);
}
//
void UART::printIMU(){
//    std::string message;
//    std::ostringstream stream;
//    stream << std::setprecision(4) << "Angle: " << imu->angle << ", Angular Rate: " << imu->angleRate << " , Angle Accel: " << imu->angleAccel;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "Angle: %.4f, Angular Rate: %.4f, Angle Accel: %.4f", imu->angle, imu->angleRate, imu->angleAccel);
    printLine(buffer, messageSize);
}
//
void UART::printPWM(){
//    std::string message;
//    std::ostringstream stream;
//    stream << std::setprecision(4) << "PWM Right: " << odom->rightMotor->currentPWM << ", PWM Left " << odom->rightMotor->currentPWM;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "PWM Right: %i, PWM Left: %i", odom->rightMotor->currentPWM, odom->leftMotor->currentPWM);
    printLine(buffer, messageSize);
}

void UART::printPID(PID& pid){
//    std::string message;
//    std::ostringstream stream;
//    stream << std::setprecision(4) << "Kp: " << pid.Kp << "Ki: " << pid.Ki << "Kd: " << pid.Kd << "Setpoint: " << pid.setpoint << ", PID Output: " << pid.output << " , Out P: " << pid.out_p << ", Out D: " << pid.out_d << ", Out I: " << pid.out_i;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "Kp: %.4f, Ki: %.4f, Kd: %.4f, Setpoint: %.2f, PID Output: %.2f, Out D: %.2f, Out I: %.2f, Out P: %.2f", pid.Kp, pid.Ki, pid.Kd, pid.setpoint, pid.output, pid.out_d, pid.out_i, pid.out_p);
    printLine(buffer, messageSize);
}

//void UART::printPIDconfig(PID& pid){
//    std::ostringstream stream;
//    stream << "P: " << pid.Kp << ", I: " << pid.Ki << " , D: " << pid.Kd;
//    printLine(stream.str());
//}

void UART::printRF(){
//    std::string message;
//    std::ostringstream stream;
//    stream << "CH1: " << commandInterface.pulseLength[0] << ", CH2: " << commandInterface.pulseLength[1] << ", CH3: " << commandInterface.pulseLength[2] << ", mode: " << commandInterface.mode;
    char buffer[200] = {0};
    int messageSize = sprintf(buffer, "CH1: %i, CH2: %i, CH3: %i, mode: %i", commandInterface.pulseLength[0], commandInterface.pulseLength[1], commandInterface.pulseLength[2], commandInterface.mode);
    printLine(buffer, messageSize);
}

void UART::encodersToStream(){
    ostream << std::setprecision(4) << "Right encoder: " << odom->rightMotor->enc_count << ", velocity: " << odom->rightMotor->enc_velocity << " , Left encoder: " << odom->leftMotor->enc_count << ", velocity: " << odom->leftMotor->enc_velocity;
    ostream << '\n';
}

void UART::odometrytoStream(){
    ostream << std::setprecision(4) << "Speed: " << odom->speed << ", Turning Rate: " << odom->turningRate << ", Theta: " << odom->theta << ", Right_Angle_Rate: " << odom->rightLinSpeed[0] << ", Left_Angle_Rate: " << odom->leftLinSpeed[0] << ", X: " << odom->x << ", Y: " << odom->y;
    ostream << '\n';
}

void UART::IMUToStream(){
    ostream << std::setprecision(4) << "Angle: " << imu->angle << ", Angular Rate: " << imu->angleRate << " , Angle Accel: " << imu->angleAccel;
    ostream << '\n';
}

void UART::PWMToStream(){
    ostream << std::setprecision(4) << "PWM Right: " << odom->rightMotor->currentPWM << ", PWM Left " << odom->rightMotor->currentPWM;
    ostream << '\n';
}

void UART::PIDToStream(PID& pid){
    ostream << std::setprecision(4) << "Kp: " << pid.Kp << "Ki: " << pid.Ki << "Kd: " << pid.Kd << "Setpoint: " << pid.setpoint << ", PID Output: " << pid.output << " , Out P: " << pid.out_p << ", Out D: " << pid.out_d << ", Out I: " << pid.out_i;
    ostream << '\n';
}

void UART::RFToStream(){
    ostream << "CH1: " << commandInterface.pulseLength[0] << ", CH2: " << commandInterface.pulseLength[1] << ", CH3: " << commandInterface.pulseLength[2] << ", mode: " << commandInterface.mode;
    ostream << '\n';
}

void UART::dataLogTransfer(PID& innerPID, PID& outerPID, PID& turningPID){
//    //printRF();// User Input
//    printPID(innerPID);// Inner PID
//    printPWM();// Variable controlled by inner PID
//    printIMU();// Variable controlled by inner PID
//    printPID(outerPID);// Outer PID
//    printEncoders();// Variable controlled by outer PID
//    printOdometry();// Variable controlled by outer PID
//    printLine("", 0);// Extra newline character to determine that we finished one sequence
    char buffer[2000] = {0};
    int messageSize = sprintf(buffer, "$Kp %.4f Ki %.4f Kd %.4f Inner_Setpoint %.2f Inner_PID_Output %.2f Inner_Out_D %.2f Inner_Out_I %.2f Inner_Out_P %.2f\n"
                                      "$PWM_Right %i PWM_Left %i\n"
                                      "$Angle %.4f Angular_Rate %.4f Angle_Accel %.4f\n"
                                      "$Outer_Kp %.4f Outer_Ki %.4f Outer_Kd %.4f Outer_Setpoint %.2f Outer_PID_Output %.2f Outer_Out_D %.2f Outer_Out_I %.2f Outer_Out_P %.2f\n"
                                      "$Right_encoder %i Right_velocity %.4f Left_encoder %i Left_velocity %0.4f\n"
                                      "$Turn_Kp %.2f Turn_Ki %.2f Turn_Kd %.2f Turn_Setpoint %.1f Turn_PID_Output %.1f\n"
                                      "$Speed %.4f Turning_Rate %.4f Theta %.4f Right_Angle_Rate %.4f Left_Angle_Rate %.4f X %.4f Y %.4f\n"
                                      "$Ms %u",
                                      innerPID.Kp, innerPID.Ki, innerPID.Kd, innerPID.setpoint, innerPID.output, innerPID.out_d, innerPID.out_i, innerPID.out_p,
                                      odom->rightMotor->currentPWM, odom->leftMotor->currentPWM,
                                      imu->angle, imu->angleRate, imu->angleAccel,
                                      outerPID.Kp, outerPID.Ki, outerPID.Kd, outerPID.setpoint, outerPID.output, outerPID.out_d, outerPID.out_i, outerPID.out_p,
                                      odom->rightMotor->enc_count, odom->rightMotor->enc_velocity, odom->leftMotor->enc_count, odom->leftMotor->enc_velocity,
                                      turningPID.Kp, turningPID.Ki, turningPID.Kd, turningPID.setpoint, turningPID.output,
                                      odom->speed, odom->turningRate, odom->theta, odom->rightLinSpeed[0], odom->leftLinSpeed[0], odom->x, odom->y,
                                      ms);
    printLine(buffer, messageSize);
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

void UART::printLine(char* str, int length){
    serialWrite(str, length);
    serialWrite("\n", 1);
}

void UART::print(std::string message){
    serialWrite(message.c_str(), message.length());
}

void UART::echoRead(){
    char readChar = this->serialRead();
    const char* charPoint = &readChar;
    serialWrite(charPoint, 1);
}

void UART::clearStream(){
    ostream = std::ostringstream(); // Clear the stream
}

void UART::printStream(std::ostringstream stream){
    std::string message = stream.str();
    print(message);
}

void UART::printStream(){
    std::string message = ostream.str();
    print(message);
}
