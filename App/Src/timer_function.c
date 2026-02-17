/*
 * timer_plot.c
 *
 *  Created on: Feb 10, 2026
 *      Author: emili
 */
#include "timer_function.h"
#include "tim.h"

HAL_StatusTypeDef timer_on(TIM_HandleTypeDef *tim)
{
//	__HAL_TIM_SET_COUNTER(tim, 0);
//	__HAL_TIM_CLEAR_FLAG(tim, TIM_FLAG_UPDATE);
	return HAL_TIM_Base_Start_IT(tim);
}

HAL_StatusTypeDef timer_off(TIM_HandleTypeDef *tim)
{
	HAL_StatusTypeDef result = HAL_TIM_Base_Stop_IT(tim);
//	__HAL_TIM_CLEAR_FLAG(tim, TIM_FLAG_UPDATE);
	return result;
}
