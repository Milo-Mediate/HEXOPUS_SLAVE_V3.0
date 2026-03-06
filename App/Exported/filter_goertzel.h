
/*
 * filter_goertzel.h
 *
 *  Created on: Aug 1, 2025
 *      Author: emili
 */

#ifndef EXPORTED_FILTER_GOERTZEL_H_
#define EXPORTED_FILTER_GOERTZEL_H_
#define NUM_SAMPLES_FILTER 400

#include <stdio.h>
#include "arm_math.h"

#include <stdbool.h>

typedef enum {
	BELOW_TH_1 = 0,
	BETWEEN_THS,
	ABOVE_TH_2
}DSP_Status;

typedef struct {
	float32_t  q1;
	float32_t  q2;
	float32_t  power;
	float32_t  k_dsp;
	float32_t  pow_fil;
	float32_t  adc_val[NUM_SAMPLES_FILTER];
	uint32_t   counter_1;
	uint32_t   counter_2;
	uint32_t   counter_1_off;
	uint32_t   counter_2_off;
	uint32_t   max_counter;
	uint16_t   index;
	DSP_Status status;
}G_Filter_t;

typedef struct {
	uint16_t  buffer_len;
	uint16_t  count;
	float32_t sum;
	float32_t max;
	float32_t value_1;
	float32_t value_2;
	float32_t gain_1;
	float32_t gain_2;
	bool      is_updated;
	bool      enable;
} DSP_TH_t;


typedef enum {
	FILTER_OK = 0,
	FILTER_ERROR
}G_Filter_error;

//extern float32_t goertzel_10kHz(uint8_t ch, float32_t new_sample, float32_t old_sample);
extern G_Filter_error goertzel_10kHz(G_Filter_t *filter, float32_t new_adc_val);

#endif /* EXPORTED_FILTER_GOERTZEL_H_ */
