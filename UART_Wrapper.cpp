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
    // Initialize UART Module 0
    UART_initModule(UARTModule, &UART_init);
    UART_enableModule(UARTModule);
    UART_enableInterrupt(UARTModule, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    }

void UART::printOdometry(){
    while(finishedTransmission==0);
    messageSize = sprintf(buffer, "Speed: %.4f, Turning Rate: %.4f, Theta: %.4f, X: %.4f, Y: %.4f\r\n", odom->speed, odom->turningRate, odom->theta, odom->x, odom->y);
    finishedTransmission = 0;
    Interrupt_enableInterrupt(INT_EUSCIA0);// Automatically send everything
}

void UART::printEncoders(){
    while(finishedTransmission == 0);
    messageSize = sprintf(buffer, "Right encoder: %i, velocity: %.4f, Left encoder: %i, velocity: %0.4f\r\n", odom->rightMotor->enc_count, odom->rightMotor->enc_velocity, odom->leftMotor->enc_count, odom->leftMotor->enc_velocity);
    finishedTransmission = 0;
    Interrupt_enableInterrupt(INT_EUSCIA0);// Automatically send everything

}

void UART::printIMU(){
    while(finishedTransmission==0);
    messageSize = sprintf(buffer, "Angle: %.4f, Angular Rate: %.4f, Angle Accel: %.4f\r\n", imu->angle, imu->angleRate, imu->angleAccel);
    finishedTransmission = 0;
    Interrupt_enableInterrupt(INT_EUSCIA0);// Automatically send everything
}

void UART::printPWM(){
    while(finishedTransmission==0);
    messageSize = sprintf(buffer, "PWM Right: %i, PWM Left: %i\r\n", odom->rightMotor->currentPWM, odom->leftMotor->currentPWM);
    finishedTransmission = 0;
    Interrupt_enableInterrupt(INT_EUSCIA0);// Automatically send everything
}

void UART::printPID(PID& pid){
    while(finishedTransmission==0);
    messageSize = sprintf(buffer, "Setpoint: %.2f, PID Output: %.2f, Out D: %.2f, Out I: %.2f, Out P: %.2f\r\n", pid.setpoint, pid.output, pid.out_d, pid.out_i, pid.out_p);
    finishedTransmission = 0;
    Interrupt_enableInterrupt(INT_EUSCIA0);// Automatically send everything
}
