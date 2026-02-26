/*
 * dsp_threshold.c
 *
 *  Created on: 18 feb 2026
 *      Author: emili
 */
#include <app_DSP_algorithm.h>
#include "dsp_threshold.h"
#include "app_hw_definition.h"

DSP_TH_status update_dsp_th(DSP_TH_t *threshold, float32_t new_val) {
	if (threshold->is_updated)
		return DSP_TH_OK;
	if (new_val > threshold->max)
		threshold->max = new_val;
	threshold->sum += new_val;
	threshold->count++;

	if (threshold->count == threshold->buffer_len) {
		if (calc_dsp_threshold(threshold) != DSP_TH_OK)
			return DSP_TH_CALCULATION_ERROR;
		threshold->count = 0;
		threshold->is_updated = true;
	}
	return DSP_TH_OK;
}

DSP_TH_status calc_dsp_threshold(DSP_TH_t *threshold)
{
	if (threshold->buffer_len == 0)
		return DSP_TH_CALCULATION_ERROR;
	float32_t mean = threshold->sum / threshold->buffer_len;
	threshold->value_1 = (threshold->gain_1*(threshold->max - mean)) + mean;
	threshold->value_2 = (threshold->gain_2*(threshold->max - mean)) + mean;
	return DSP_TH_OK;
}



