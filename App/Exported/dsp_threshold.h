/*
 * dsp_threshold.h
 *
 *  Created on: 18 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_DSP_THRESHOLD_H_
#define EXPORTED_DSP_THRESHOLD_H_

typedef enum {
	DSP_TH_OK,
	DSP_TH_OUT_OF_RANGE,
	DSP_TH_CALCULATION_ERROR,
}
DSP_TH_status;

DSP_TH_status update_dsp_th(DSP_TH_t *threshold, float32_t new_val);
DSP_TH_status calc_dsp_threshold(DSP_TH_t *threshold);
DSP_TH_status set_outdated_dsp_th(uint8_t sens);

#endif /* EXPORTED_DSP_THRESHOLD_H_ */
