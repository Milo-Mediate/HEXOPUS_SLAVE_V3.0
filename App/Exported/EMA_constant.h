/*
 * EMA_constant.h
 *
 *  Created on: 18 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_EMA_CONSTANT_H_
#define EXPORTED_EMA_CONSTANT_H_
#include "arm_math.h"
/**
 * @brief Compute the alpha coefficient of an EMA filter given tau.
 * @param tau Time constant (in seconds)
 * @param Ts  Sampling period (in seconds)
 * @return Alpha value (between 0 and 1)
 */
extern float32_t EMA_GetAlpha(float32_t tau, float32_t ts);
#endif /* EXPORTED_EMA_CONSTANT_H_ */
