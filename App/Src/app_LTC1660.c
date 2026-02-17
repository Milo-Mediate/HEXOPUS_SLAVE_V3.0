/*
 * app_LTC1660.c
 *
 *  Created on: Feb 5, 2026
 *      Author: emili
 */

#include "app_LTC1660.h"
#include "stm32h5xx_hal.h"


//#include "LTC1660.h"


#define MAX_DAC_INSTALLED      2U
#define DAC_CONFIGURED         2U

DAC_t dacs[MAX_DAC_INSTALLED];
dac_cal_status_t dac_errors[NUM_SENS] = { 0 };

uint16_t app_get_dac_channel(uint8_t dev, uint8_t channel)
{
	if (dev > DAC_CONFIGURED)
		return -1;
	return get_dac_channel(&dacs[dev], channel);
}

//uint16_t get_dac_2_channel(uint8_t channel)
//{
//	return get_dac_channel(&dacs[1], channel);
//}

dac_cal_status_t calibration_init()
{
	for (uint8_t i = 0U; i < DAC_CONFIGURED; i++)
	{
		if (app_calibration_dac(&dacs[i])!= DAC_CAL_OK)
			return DAC_CAL_ERR;
	}
	return DAC_CAL_OK;
}

dac_cal_status_t app_calibration_dac(DAC_t *dac)
{
	bool result = true;
	for (uint8_t i = 0U; i < NUM_SENS; i++)
	{
		dac_errors[i] =  auto_set_channel_value(dac, i);
		result &= (dac_errors[i] == DAC_CAL_OK);
	}
	if (!result)
		return DAC_CAL_ERR;
	return DAC_CAL_OK;
}

void app_dac_init()
{
	dac_init(&hspi2, &dacs[0], LTC1660_CS_GPIO_Port,LTC1660_CS_Pin);
	dac_init(&hspi2, &dacs[1], LTC1660_CS_2_GPIO_Port,LTC1660_CS_2_Pin);
}
