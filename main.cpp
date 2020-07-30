#include "MiniSegway.h"

long last_ms = 0;
long ms = 0;

//Motor initialization
Motor rightMotor = Motor(MAX_PWM, CW_1_PORT, CW_1_PIN, CCW_1_PORT, CCW_1_PIN,  &TA2CCR3,
           RIGHT_ENC_PORT, RIGHT_ENC_PIN_A, RIGHT_ENC_PORT, RIGHT_ENC_PIN_B);
Motor leftMotor = Motor(MAX_PWM, CW_2_PORT, CW_2_PIN, CCW_2_PORT, CCW_2_PIN, &TA2CCR4,
          LEFT_ENC_PORT, LEFT_ENC_PIN_A, GPIO_PORT_P4, LEFT_ENC_PIN_B);

MotorController motorController(&rightMotor, &leftMotor);

// UART configuration 115200bps with SMCLK = 4MHz
const eUSCI_UART_Config UART_init =
    {
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     2,
     2,
     0xBB,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
    };

//Odometry, IMU Initialization
Odometry odom = Odometry(wheelBase, wheelRadius, ticksPerRev, &rightMotor, &leftMotor, dt, odom_velocity_alpha);
IMU imu = IMU();

// Controllers
PID tiltController = PID(Kp_tilt, 0, Kd_tilt, dt, windup_tilt);
PID turningController = PID(Kp_turning, 0, Kd_turning, dt, windup_turning);
OuterPID velocityController = OuterPID(Kp_velocity, 0, Kd_velocity, dt, &tiltController, alpha_velocity);


// UART Initialization
UART UARTHandler = UART(UART_init, EUSCI_A0_BASE, &odom, &imu);

//User interface
RFInterface commandInterface;

void main(void)
 {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	//Initial Configs
	setupClocks();
	setupPins();
    configPWM();
    FPU_enableModule();
    setupSystick();
    setupEncoderInterrupts();
    configTimerCapture();
    configSPI();
    imu.configModule();

    UARTHandler.UARTSetup();
    Interrupt_enableMaster();

    //Wait for user to press upper right corner on the remote to start the calibration
    while(commandInterface.mode == 0) commandInterface.pollrfReceiver();
    imu.calibrate();
    commandInterface.calibrate();
    while(commandInterface.mode == 1){
        commandInterface.pollrfReceiver();
    }



    double motorCommands[2] = {};
    double rightMotorFilteredPWM[2] = {};
    double leftMotorFilteredPWM[2] = {};

    // Timed Loop
    while(true){
        while(ms-last_ms < 10);
        last_ms = ms;

        // Poll RF Receiver
        commandInterface.pollrfReceiver();

        //Read sensors
        odom.updateOdometry();
        imu.getFilteredAngle();

        //Controllers
        if(selectedMode == angleMode){
            tiltController.updatePID(tiltController.setpoint, imu.angle, imu.angleRate);
        }
        else if(selectedMode == velocityMode){
            velocityController.updatePID(velocityController.setpoint, odom.speed, imu.angle, imu.angleRate);
        }
        turningController.updatePID(turningController.setpoint, odom.turningRate);

        //PWM commands
        LPF(tiltController.output - turningController.output, rightMotorFilteredPWM, alpha_PWM);
        LPF(tiltController.output + turningController.output, leftMotorFilteredPWM, alpha_PWM);
        motorCommands[0] = rightMotorFilteredPWM[0];
        motorCommands[1] = leftMotorFilteredPWM[0];
        if(commandInterface.mode==1){
            //Only command motors if the user has pressed B
            if(abs(imu.angle)<40){
                motorController.commandMotors(motorCommands);
            }else{
                double zeroCommand[] = {0,0};
                motorController.commandMotors(zeroCommand);
            }

        }
        //Blue LED on if we are past setpoint
        if(imu.angle>=abs(tiltController.setpoint)){
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
            }
            else{
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            }
        // Debugging Serial Transmissions
        //UARTHandler.printEncoders();
        //UARTHandler.printOdometry();
        ///UARTHandler.printIMU();
        UARTHandler.printPID(velocityController);
        //UARTHandler.printRF();
    };
}


