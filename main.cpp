#include "MiniSegway.h"

long last_ms = 0;
long ms = 0;

ControlMode selectedMode = velocityMode;

// Debug
bool debugMode = false;

// Send all data through serial
bool sendData = false;
bool ten_hz = false;

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
IMU imu = IMU(dt);

// Controllers
PID tiltController = PID(Kp_tilt, Ki_tilt, Kd_tilt, dt, windup_tilt);
PID turningController = PID(Kp_turning, Ki_turning, Kd_turning, dt, windup_turning);
OuterPID velocityController = OuterPID(Kp_velocity, Ki_velocity, Kd_velocity, dt, &tiltController, alpha_velocity);


// UART Initialization
UART UARTHandler = UART(UART_init, EUSCI_A2_BASE, &odom, &imu);

//User interface
RFInterface commandInterface;

//Serial protocol
SerialProtocol protocol = SerialProtocol(&UARTHandler, &velocityController, &tiltController, &imu);

void main(void)
   {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	// Set to Vcore1
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    // Set to use DCDC
    PCM_setPowerMode(PCM_DCDC_MODE);
	FlashCtl_setWaitState( FLASH_BANK0, 2);
    FlashCtl_setWaitState( FLASH_BANK1, 2);
	//Initial Configs
    FPU_enableModule();
	setupClocks();
	setupPins();
    configPWM();
    setupSystick();
    setupEncoderInterrupts();
    configTimerCapture();
    configSPI();
    imu.configModule();
    UARTHandler.UARTSetup();
    Interrupt_enableMaster();

    //Skip this section if we are in debug mode
    if(debugMode != true){
    //Wait for user to press channel 3 to start calibration
    while(commandInterface.mode == 0) commandInterface.pollrfReceiver();
    imu.calibrate();
    commandInterface.calibrate();
    while(commandInterface.mode == 1){
        commandInterface.pollrfReceiver();
    }
    }
    else{
        imu.calibrate();
        commandInterface.calibrate();
    }

    double motorCommands[2] = {};
    double rightMotorFilteredPWM[2] = {};
    double leftMotorFilteredPWM[2] = {};

    // Timed Loop
    while(true){
        while(ms-last_ms < LOOP_TIME);
        last_ms = ms;

        // Poll RF Receiver
        commandInterface.pollrfReceiver();

        //Poll Serial Commands
        protocol.executeProtocol();

        //Read sensors
        rightMotor.calculateEncVelocity();
        leftMotor.calculateEncVelocity();
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
        double zeroCommand[] = {0,0};
        if(commandInterface.mode==1){
            //Only command motors if the user has pressed B
            if(abs(imu.angle)<40){
                motorController.commandMotors(motorCommands);
            }else{
                motorController.commandMotors(zeroCommand);
            }

        }
        else{
            motorController.commandMotors(zeroCommand);
        }
        //Blue LED on if we are past setpoint
        if(imu.angle>=tiltController.setpoint){
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
            }
        else{
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            }
        if(sendData && ten_hz){
            UARTHandler.dataLogTransfer(tiltController, velocityController, turningController);
            ten_hz = false;
        }
        // Debugging Serial Transmissions
//        UARTHandler.printEncoders();
//        UARTHandler.printOdometry();
//        UARTHandler.printIMU();
//        UARTHandler.printPID(velocityController);
//        UARTHandler.printRF();
        if(debugMode){
//            if(UARTHandler.serialGetBufferAvailable()>0){
//                UARTHandler.echoRead();
//            }
        }
    };
}


