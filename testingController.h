/*
 * testingController.h
 *
 *  Created on: Jan 18, 2021
 *      Author: Juan
 */

#ifndef TESTINGCONTROLLER_H_
#define TESTINGCONTROLLER_H_

#include "MiniSegway.h"
#include <array>
#include <vector>

class testingController
{
public:
    // Constructor
    testingController(std::array<std::vector<long>, 10>* time_sequences_in, std::array<std::vector<double>, 10>* vel_sequences_in, std::array<std::vector<double>, 10>* turn_sequences_in);

    // Methods
    void startTest(long start_ms, int test_num);

    double getVelCommand(long t);
    double getTurnCommand(long t);

    // Fields
    long start_ms = 0;
    int test_num = 0;
    bool test_activated = false;

    // Array of vectors. Store up to 10 tests, each test can have different lengths
    std::array<std::vector<long>, 10>* time_sequences;
    std::array<std::vector<double>, 10>* vel_sequences;
    std::array<std::vector<double>, 10>* turn_sequences;

};

#endif /* TESTINGCONTROLLER_H_ */
