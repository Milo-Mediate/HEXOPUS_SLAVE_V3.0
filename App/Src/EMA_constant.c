/*
 * EMA_constant.c
 *
 *  Created on: 18 feb 2026
 *      Author: emili
 */

#include "EMA_constant.h"
/**
 * @brief Compute the alpha coefficient of an EMA filter given tau.
 * @param tau Time constant (in seconds)
 * @param Ts  Sampling period (in seconds)
 * @return Alpha value (between 0 and 1)
 */
float32_t EMA_GetAlpha(float32_t tau, float32_t ts)
{
    if (tau <= 0.0f) return 1.0f; // Instant response (no filtering)

    float32_t alpha = 1.0f - expf(-ts / tau);
    return alpha;
}

