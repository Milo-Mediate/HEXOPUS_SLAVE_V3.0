/*
 * app_LTC1660.c
 *
 *  Created on: Feb 5, 2026
 *      Author: emili
 */

#include "app_LTC1660.h"
#include "stm32h5xx_hal.h"


//#include "LTC1660.h"


DAC_t dacs[MAX_DAC_INSTALLED];

void app_dac_init()
{
	dac_init(&hspi2, &dacs[0], LTC1660_CS_GPIO_Port,LTC1660_CS_Pin, DAC_1_CHANNELS, 0);
	dac_init(&hspi2, &dacs[1], LTC1660_CS_2_GPIO_Port,LTC1660_CS_2_Pin, DAC_2_CHANNELS, 1);
}

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
	dac_cal_status_t status;
	for (uint8_t i = 0U; i < DAC_CONFIGURED; i++)
	{
		status = app_calibration_dac(&dacs[i]);
	}
	return DAC_CAL_OK;
}

dac_cal_status_t app_calibration_dac(DAC_t *dac)
{
	bool result = true;
	for (uint8_t i = 0U; i < dac->num_channels; i++)
	{
		dac->status[i] =  auto_set_channel_value(dac, i);
		result &= (dac->status[i] == DAC_CAL_OK);
	}
	if (!result)
		return DAC_CAL_ERR;
	return DAC_CAL_OK;
}

void app_set_dac_channel(uint8_t dev, uint8_t channel, uint16_t value)
{
	set_dac_channel(&dacs[dev], channel, value);
}



















