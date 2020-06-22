/*
 * Odometry.cpp
 *
 *  Created on: Jun 16, 2020
 *      Author: Juan Francisco Vallejo
 */

#include "Odometry.h"

Odometry::Odometry(double wheelBase, double wheelRadius, double ticksPerRev, Motor* rightMotor, Motor* leftMotor, double dt, double alpha)
                    :wheelBase(wheelBase), wheelRadius(wheelRadius), ticksPerRev(ticksPerRev), rightMotor(rightMotor),
                     leftMotor(leftMotor), dt(dt), alpha(alpha){}

void Odometry::updateOdometry(){
    double delta_s_right = ((rightMotor->enc_count-prevRightCount)/ticksPerRev)*2*M_PI*wheelRadius;
    double delta_s_left = ((leftMotor->enc_count-prevLeftCount)/ticksPerRev)*2*M_PI*wheelRadius;
    prevRightCount = rightMotor->enc_count;
    prevLeftCount = leftMotor->enc_count;
    //Compute deltas
    double delta_theta = (delta_s_right-delta_s_left)/wheelBase;
    double delta_s = (delta_s_right+delta_s_left)/2;
    //Update positions
    x += delta_s*std::cos(theta+delta_theta/2);
    y += delta_s*std::sin(theta+delta_theta/2);
    theta += delta_theta;
    //Filter and verify angular velocities
    if((ms-rightMotor->prev_tick)>100) rightMotor->enc_velocity=0;
    if((ms-leftMotor->prev_tick)>100) leftMotor->enc_velocity=0;
    LPF(rightMotor->enc_velocity*2*M_PI*wheelRadius/ticksPerRev, rightAngRate, alpha);
    LPF(leftMotor->enc_velocity*2*M_PI*wheelRadius/ticksPerRev, leftAngRate, alpha);
    //Calculate rates
    turningRate = (rightAngRate[0]-leftAngRate[0])/wheelBase;
    speed = (rightAngRate[0]+leftAngRate[0])/2;
}

