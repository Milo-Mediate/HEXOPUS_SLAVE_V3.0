/**
 * @file filter_goertzel.c
 * @brief Single-bin sliding Goertzel update for 10 kHz processing.
 * @details
 * Implements the sliding Goertzel recurrence:
 *   q0 = coeff_*q1 - q2 + x_new - x_old
 *   {q1, q2} <- {q0, q1}
 * and returns the instantaneous power estimate:
 *   P = q1^2 + q2^2 - q1*q2*coeff_
 *
 * ## Dependencies / Globals
 * - Uses global state from buffer_manager: coeff_, q1_[NUM_SENS], q2_[NUM_SENS].
 *
 * ## Concurrency
 * - If called from both ISR and thread context, protect updates to q1_/q2_.
 *
 * ## MISRA Notes
 * - Validates channel index `ch < NUM_SENS`.
 * - Returns 0.0f if `ch` is out of range.
 *
 * @author milo
 * @date   22/09/2025
 */

#include "filter_goertzel.h"
#include "arm_math.h"

#define SAMPLING_FREQ      40000.0f
#define TARGET_FREQ        10000.0f

static const float32_t omega = (2.0f * PI * TARGET_FREQ) / SAMPLING_FREQ;
static float32_t coeff;

void coeff_init() {
	coeff = arm_cos_f32(omega) * 2.0f;
}

/**
 * @brief Compute the alpha coefficient of an EMA filter given tau.
 * @param tau Time constant (in seconds)
 * @param Ts  Sampling period (in seconds)
 * @return Alpha value (between 0 and 1)
 */


G_Filter_error goertzel_10kHz(G_Filter_t *filter, float32_t new_adc_val) {
	/* Guard: check channel range */
	if (!filter) {
		return FILTER_ERROR;
	}

	/* Goertzel sliding update */
	float32_t q0 = (coeff * filter->q1) - filter->q2 + new_adc_val - filter->adc_val[filter->index];
	float32_t q2_new = filter->q1;
	float32_t q1_new = q0;

	/* Commit state */
	filter->q1 = q1_new;
	filter->q2 = q2_new;

	filter->adc_val[filter->index] = new_adc_val;
	filter->index += 1;
	/* Power estimate */
	filter->power = (filter->q1 * filter->q1) + (filter->q2 * filter->q2) - (filter->q1 * filter->q2 * coeff);
	return FILTER_OK;
}
