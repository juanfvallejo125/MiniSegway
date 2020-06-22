/*
 * LPF.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef LPF_H_
#define LPF_H_
#include "MiniSegway.h"

void LPF(double rawData, double* filteredData, double alpha);


#endif /* LPF_H_ */
