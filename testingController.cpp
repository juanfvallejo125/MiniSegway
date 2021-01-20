/*
 * testingController.cpp
 *
 *  Created on: Jan 18, 2021
 *      Author: Juan
 */

#include <testingController.h>

testingController::testingController(std::array<std::vector<long>, 10>* time_sequences_in, std::array<std::vector<double>, 10>* vel_sequences_in, std::array<std::vector<double>, 10>* turn_sequences_in){
    this->time_sequences = time_sequences_in;
    this->vel_sequences = vel_sequences_in;
    this->turn_sequences = turn_sequences_in;

}

void testingController::startTest(long start_ms, int test_num){
    this->start_ms = start_ms;
    this->test_activated = true;
    this->test_num = test_num;
}

double testingController::getVelCommand(long t){
    int interval = 0;
    bool interval_found = false;
    double velocity = 0;
    std::vector<long>& time_seq = time_sequences->at(test_num);
    std::vector<double>& vel_seq = vel_sequences->at(test_num);
    t = t - start_ms;
    for(int i = 0; i < time_seq.size()-1; i++){
        if(t >= time_seq.at(i) && t <= time_seq.at(i+1)){
            interval = i;
            interval_found = true;
            break;
        }
    }
    if(!interval_found){// Nothing found, send a zero command
        test_activated = false;
    }else{// Interval found, linear interpolation between points
        double m = (vel_seq[interval+1] - vel_seq[interval])/(time_seq[interval+1] - time_seq[interval]);
        velocity = m*(t - time_seq[interval]) + vel_seq[interval];
    }
    return velocity;
}

double testingController::getTurnCommand(long t){
    int interval = 0;
    bool interval_found = false;
    double turn_rate = 0;
    std::vector<long>& time_seq = time_sequences->at(test_num);
    std::vector<double>& turn_seq = turn_sequences->at(test_num);
    t = t - start_ms;
    for(int i = 0; i < time_seq.size()-1; i++){
        if(t >= time_seq.at(i) && t <= time_seq.at(i+1)){
            interval = i;
            interval_found = true;
            break;
        }
    }
    if(!interval_found){// Nothing found, send a zero command
        test_activated = false;
    }else{// Interval found, linear interpolation between points
        double m = (turn_seq[interval+1] - turn_seq[interval])/(time_seq[interval+1] - time_seq[interval]);
        turn_rate = m*(t - time_seq[interval]) + turn_seq[interval];
    }
    return turn_rate;
}






