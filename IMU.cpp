/*
 * IMU.cpp
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Vallejo
 *
 */

#include "IMU.h"

IMU::IMU(double dt){
    this->dt = dt;
}

void SPI_Write(uint8_t byteCount, uint8_t* buffer);
void SPI_Read(uint8_t byteCount, uint8_t* rx_buffer, uint8_t* tx_buffer);

void IMU::configModule(){
    //set the gyroscope control register to work at 104 Hz, 250 dps and in bypass mode
    TXDataBuffer[0] = LSM6DS3_CTRL2_G;
    TXDataBuffer[1] = 0b01000000;//0x4C;
    // Set the Accelerometer control register to work at 104 Hz, 2G,and LPF2 output selected
    // low pass filter(check figure9 of LSM6DS3's datasheet)
    TXDataBuffer[2] = LSM6DS3_CTRL1_XL;
    TXDataBuffer[3] = 0b01000010;//0x4A;//2g resolution, 104 Hz ODR, LPF2 enabled
    // set gyroscope power mode to high performance and bandwidth to 16 mHz
    TXDataBuffer[4] = LSM6DS3_CTRL7_G;
    TXDataBuffer[5] = 0b00000000;//0x00;
    // Set LPF cutoff to ODR/20 (5.2Hz), LPF2 data sent to 6D interrupt
    TXDataBuffer[6] = LSM6DS3_CTRL8_XL;
    TXDataBuffer[7] = 0b01001001;//0x09;
    SPI_Write(8, TXDataBuffer);
}

void IMU::readAccelerometer(){
    // Set up TX buffer
    TXDataBuffer[0] = LSM6DS3_OUTX_L_XL|(1<<7);
    TXDataBuffer[1] = LSM6DS3_OUTX_H_XL|(1<<7);
    TXDataBuffer[2] = LSM6DS3_OUTY_L_XL|(1<<7);
    TXDataBuffer[3] = LSM6DS3_OUTY_H_XL|(1<<7);
    TXDataBuffer[4] = LSM6DS3_OUTZ_L_XL|(1<<7);
    TXDataBuffer[5] = LSM6DS3_OUTZ_H_XL|(1<<7);
    // SPI read 6 bytes
    SPI_Read(6, RXDataBuffer, TXDataBuffer);
    // Acceleration conversions
    uint32_t i;
    uint32_t j = 0;
    for(i=0;i<3;i++){
        acceleration[i] = ((int16_t) (RXDataBuffer[j+1] << 8 | RXDataBuffer[j]))* 2/32768.0;//4 / 32768.0;
        j++;j++;
    }
}

void IMU::readGyroscope(){
    // Set up TX buffer
    TXDataBuffer[0] = LSM6DS3_OUTX_L_G|(1<<7);
    TXDataBuffer[1] = LSM6DS3_OUTX_H_G|(1<<7);
    TXDataBuffer[2] = LSM6DS3_OUTY_L_G|(1<<7);
    TXDataBuffer[3] = LSM6DS3_OUTY_H_G|(1<<7);
    TXDataBuffer[4] = LSM6DS3_OUTZ_L_G|(1<<7);
    TXDataBuffer[5] = LSM6DS3_OUTZ_H_G|(1<<7);
    // SPI read 6 bytes
    SPI_Read(6, RXDataBuffer, TXDataBuffer);
    // Gyroscope conversions
    uint32_t i;
    uint32_t j = 0;
    for(i=0;i<3;i++){
        angularRate[i] = ((int16_t) (RXDataBuffer[j+1] << 8 | RXDataBuffer[j]))* 250/32768.0;//2000 / 32768.0;
        j++;j++;
    }
}

void IMU::calibrate(){
    int i;
    int j;
    double gyro_acum[3]={0,0,0};
    double accel_acum[3]={0,0,0};
    for(i=0; i<100; i++){
        readAccelerometer();
        readGyroscope();
        for(j=0; j<3; j++){
            gyro_acum[j] += angularRate[j];
            accel_acum[j] += acceleration[j];
        }
        __delay_cycles(DCO_Freq/1000); // Delay 1 ms
    }
    for(j=0; j<3; j++){
        angularRateZero[j] = gyro_acum[j]/100;
        accelerationZero[j] = accel_acum[j]/100;
    }
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);//Light up LED to indicate we are out of calibration
}

double IMU::getFilteredAngle(){
    readGyroscope();
    readAccelerometer();
    angleAccel = std::atan(acceleration[2]/acceleration[0])*180/M_PI;
    angleRate = (angularRate[1] - angularRateZero[1]);
    angle = alpha*(dt*(angleRate)+angle)+ (1-alpha)*(angleAccel-accelOffset);
    return angle;
}




