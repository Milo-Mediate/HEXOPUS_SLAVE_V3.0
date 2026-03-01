/*
 * mschine_state.h
 *
 *  Created on: 26 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_MACHINE_STATE_H_
#define EXPORTED_MACHINE_STATE_H_

#include <stdbool.h>
#include "can_manager.h"

typedef enum {
	IDLE = 0, /** */
	PRE_SETUP, /** */
	SETUP, /** */
	RUNNING, /** */
	STOP, /** */
	MUTING, /** */
	FAULT, /** */
	EXTERNAL, /** */
	EXPIRED,
	SEND_PARAMS,
	SLAVE_RESET
} MachineStatus;

typedef struct {
	MachineStatus state;
	bool stop_1;
	bool stop_2;
	bool msg_1_sent;
	bool msg_2_sent;
}MachineStatus_t;


void machine_state_init();
void set_machine_state(MachineStatus new_state);
MachineStatus get_machine_state();
void set_stop_1(bool status);
bool get_stop_1();
void set_stop_2(bool status);
bool get_stop_2();
void set_msg_1(bool status);
bool get_msg_1_sent();
void set_msg_2(bool status);
bool get_msg_2_sent();

#endif /* EXPORTED_MACHINE_STATE_H_ */
