/*
 * MiniSegway.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Vallejo
 */

#ifndef MINISEGWAY_H_
#define MINISEGWAY_H_

#define RX_BUFFER_SIZE 2000
#define TX_BUFFER_SIZE 2000

enum ControlMode {velocityMode, angleMode};

// Global definitions
#define DCO_Freq 32E+6
#define MAX_PWM 225
const double dt = 0.01;

const double Kp_tilt = 25;//18//35//38 38 for low battery
const double Kd_tilt = 0.02;//0.05//0.02
const double Ki_tilt = 0;//0.005;//0.002;//0.1 // 0.02
const double windup_tilt = 225;
const double alpha_PWM = 1;//0.4;

const double Kp_turning = 25;//17;
const double Kd_turning = 0.03;
const double Ki_turning = 0;
const double windup_turning = 225;

const double Kp_velocity = 0.015;//0.015;
const double Kd_velocity = 0.00;//0.005;
const double Ki_velocity = 0.00;// 0.018//0.005;//0.015
const double windup_velocity = 225;
const double alpha_velocity = 0.2;// 0.2//0.3; // 0.4

const double odom_velocity_alpha = 0.1;// 0.2
const double wheelRadius = 50.25;
const double wheelBase = 167.5;
const double ticksPerRev = 240;

#include "driverlib.h"
#include <stdint.h>
#include "MotorController.h"
#include "LPF.h"
#include "msp.h"
#include <stdio.h>
#include "IMU.h"
#include "SPI.h"
#include "PID.h"
#include "OuterPID.h"
#include "RFInterface.h"
#include "UART_Wrapper.h"
#include "SerialProtocol.h"
#include "initConfigs.h"
#include "Odometry.h"
#include "testingController.h"

// Motor pins
#define CW_1_PORT GPIO_PORT_P3
#define CW_1_PIN GPIO_PIN7
#define CCW_1_PORT GPIO_PORT_P3
#define CCW_1_PIN GPIO_PIN6

#define CW_2_PORT GPIO_PORT_P3
#define CW_2_PIN GPIO_PIN5
#define CCW_2_PORT GPIO_PORT_P5
#define CCW_2_PIN GPIO_PIN2

// Encoder Pins
// Right Wheel
#define RIGHT_ENC_PORT GPIO_PORT_P4
#define RIGHT_ENC_PIN_A GPIO_PIN0
#define RIGHT_ENC_PIN_B GPIO_PIN2
// Left Wheel
#define LEFT_ENC_PORT GPIO_PORT_P5
#define LEFT_ENC_PIN_A GPIO_PIN4
#define LEFT_ENC_PIN_B GPIO_PIN7 // On Port 4, 5.5 is fried

class Motor;
class Odometry;
class UART;
class IMU;
class PID;
class OuterPID;
class RFInterface;
class SerialProtocol;
class testingController;

// Global variables
extern Motor rightMotor;
extern Motor leftMotor;
extern UART UARTHandler;
extern Odometry odom;
extern IMU imu;
extern PID tiltController;
extern PID turningController;
extern OuterPID velocityController;
extern RFInterface commandInterface;
extern SerialProtocol protocol;
extern testingController testController;
extern ControlMode selectedMode;

extern bool sendData;
extern long ms; // Counts number of ms since program started
extern long last_ms;
extern bool ten_hz;

//Debug

#endif /* MINISEGWAY_H_ */
