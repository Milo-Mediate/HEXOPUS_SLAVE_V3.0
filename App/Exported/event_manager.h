/*
 * event_manager.h
 *
 *  Created on: Nov 6, 2025
 *      Author: emili
 */

#ifndef INC_EVENT_MANAGER_H_
#define INC_EVENT_MANAGER_H_

#include "definition.h"
#include "main.h"

#ifdef MASTER
/* ToDo Qui sto usando due settori a caso della flash per salvare gli eventi poi il tutto va spostato nei due banchi che permettono 100k scritture */
#define FLASH_EVENT_SECTOR_ADDRESS1 113
#define FLASH_EVENT_SECTOR_ADDRESS2 115
#define FLASH_EVENT_SECTORS 1
#define BASE_EVENT_ADDR 0x08000000U
/*
	#define EVENT_VECTOR_ADDR1 0x09000000U
	#define EVENT_VECTOR_ADDR2 0x0D000000U
	#define FLASH_EVENT_SECTOR 1
*/

#define FLASH_EVENT_SIZE   1024
#endif

typedef enum {
	EVENT_RESET = 0,
	// COM
	EVENT_USB_SET,
	EVENT_USB_RESET,
	EVENT_FAULT_USB,
	EVENT_FAULT_CAN,
	EVENT_TIMEOUT_ETH,
	EVENT_TIMEOUT_FSoE,

	// FLASH
	EVENT_FLASH_WRITE,
	EVENT_FLASH_READ,
	EVENT_FAULT_FLASH_READ,
	EVENT_FAULT_FLASH_WRITE,

#ifdef MASTER
	// SAFETY
	EVENT_OSSD_1_SET,
	EVENT_OSSD_2_SET,
	//EVENT_OSSD_1_RESET,
	//EVENT_OSSD_2_RESET,
	EVENT_STOP,
	EVENT_PRE_STOP,
	EVENT_FAULT_OSSD_1,
	EVENT_FAULT_OSSD_2,

	EVENT_DRY_CONTACT_1_SET,
	EVENT_DRY_CONTACT_2_SET,
	EVENT_DRY_CONTACT_3_SET,
	EVENT_DRY_CONTACT_4_SET,
	EVENT_DRY_CONTACT_1_RESET,
	EVENT_DRY_CONTACT_2_RESET,
	EVENT_DRY_CONTACT_3_RESET,
	EVENT_DRY_CONTACT_4_RESET,

	EVENT_EXPIRED,
	EVENT_BATTERY_EXPIRED,

	EVENT_MUTING_SET,
	EVENT_MUTING_RESET,
#endif
// HW CHECK
	// Calibration
	EVENT_FAULT_CAL_SENS_1,
	EVENT_FAULT_CAL_SENS_2,
	EVENT_FAULT_CAL_SENS_3,
	EVENT_FAULT_CAL_SENS_4,
	EVENT_FAULT_CAL_SENS_5,
	EVENT_FAULT_CAL_SENS_6,
	EVENT_FAULT_CAL_DUMMY_1,
	EVENT_FAULT_CAL_DUMMY_2,
	EVENT_FAULT_CAL_DUMMY_3,
	EVENT_FAULT_CAL_DUMMY_4,
	EVENT_FAULT_CAL_DUMMY_5,
	EVENT_FAULT_CAL_DUMMY_6,

	EVENT_FAULT_HEARTBEAT,
	EVENT_FAULT_WATCHDOG,

	EVENT_FAULT_XOR_SENS_1,
	EVENT_FAULT_XOR_SENS_2,
	EVENT_FAULT_XOR_SENS_3,
	EVENT_FAULT_XOR_SENS_4,
	EVENT_FAULT_XOR_SENS_5,
	EVENT_FAULT_XOR_SENS_6,
	EVENT_FAULT_XOR_DUMMY_1,
	EVENT_FAULT_XOR_DUMMY_2,
	EVENT_FAULT_XOR_DUMMY_3,
	EVENT_FAULT_XOR_DUMMY_4,
	EVENT_FAULT_XOR_DUMMY_5,
	EVENT_FAULT_XOR_DUMMY_6,
	EVENT_FAULT_VS,
	EVENT_FAULT_10K,
} EventCode;

typedef struct {
	uint8_t min;        // minuto evento
	uint8_t hour;       // ora evento
	uint8_t day;        // giorno del mese evento
	uint8_t month;      // mese dell'evento
	uint8_t year;       // anno dell'evento
	uint8_t code;       // codice dell'evento
	uint8_t device_id;  // id del device che ha generato l'evento
} Event_t;

extern int sprint_Event(char *string, const Event_t *event);

#ifdef MASTER
/**
 * @brief Struct for event data, 32-bit aligned for STM32H563.
 * @note Fields are ordered to minimize padding. Size: 7178 bytes (excluding flash_crc), 7180 bytes total.
 * @note Padding bytes are initialized to 0 to ensure consistent CRC calculation.
 */
typedef struct {

	Event_t events[FLASH_EVENT_SIZE];

	uint32_t working_minute;

	uint8_t init_flag;

	uint16_t event_counter;
	uint16_t next_event_pos;
	uint16_t flash_crc; // valore che viene generato ogni volta che si scrive la struct sulla flash
} __attribute__((aligned(32))) MasterEventData_t;

extern MasterEventData_t ev_data_write_;
extern MasterEventData_t ev_data_read_;


void init_event();
void write_event(EventCode event_code, uint8_t dev_id);
HAL_StatusTypeDef read_event();
HAL_StatusTypeDef save_event();

#endif
#endif /* INC_EVENT_MANAGER_H_ */
