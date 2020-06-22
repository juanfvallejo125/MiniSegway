/*
 * LPF.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Vallejo
 */

#include "LPF.h"

void LPF(double rawData, double* filteredData, double alpha){
    filteredData[0] = rawData*alpha + (1-alpha)*filteredData[1];
    filteredData[1] = filteredData[0];
}

