/*
 * can_TLC5916.h
 *
 *  Created on: 19 feb 2026
 *      Author: emili
 */

#ifndef SRC_CAN_TLC5916_H_
#define SRC_CAN_TLC5916_H_

HAL_StatusTypeDef can_get_tlc_status();
void can_set_tlc_status(const uint8_t *RxData);
#endif /* SRC_CAN_TLC5916_H_ */
