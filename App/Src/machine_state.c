/*
 * machine_state.c
 *
 *  Created on: 26 feb 2026
 *      Author: emili
 */
#include "machine_state.h"

static MachineStatus_t MS;

void machine_state_init()
{
	MS.state = IDLE;
	MS.stop_1 = false;
	MS.stop_2 = false;
	MS.msg_1_sent = false;
	MS.msg_2_sent = false;
}

void set_machine_state(MachineStatus new_state)
{
	MS.state = new_state;
}

MachineStatus get_machine_state()
{
	return MS.state;
}

void set_stop_1(bool status)
{
	if (MS.stop_1 == status)
		return;

	MS.stop_1 = status;
	set_msg_1(false);
}

bool get_stop_1()
{
	return MS.stop_1;
}

void set_stop_2(bool status)
{
	MS.stop_2 = status;
}

bool get_stop_2()
{
	return MS.stop_2;
}

void set_msg_1(bool status)
{
	MS.msg_1_sent = status;
}

bool get_msg_1_sent()
{
	return MS.msg_1_sent;
}

void set_msg_2(bool status)
{
	MS.msg_1_sent = status;
}

bool get_msg_2_sent()
{
	return MS.msg_2_sent;
}
