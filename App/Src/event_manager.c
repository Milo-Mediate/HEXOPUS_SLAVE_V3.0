/*
 * event_manager.c
 *
 *  Created on: Nov 6, 2025
 *      Author: emili
 */

#include "event_manager.h"

#include <stdint.h>
#include "global.h"
#include "logging.h"

#ifdef MASTER
MasterEventData_t ev_data_write_;
MasterEventData_t ev_data_read_;

/**
 * @brief Initializes the events vector.
 *
 * Clears and prepares the internal events vector for use (e.g. reserves memory,
 * resets size/counter, sets initial state).
 */
void init_event() {

	memset((char *)ev_data_write_.events, 0, sizeof(Event_t)*FLASH_EVENT_SIZE);

	ev_data_write_.init_flag = 1;
	ev_data_write_.working_minute = 0;
	ev_data_write_.event_counter = 0;
	ev_data_write_.next_event_pos = 0;

	ev_data_write_.flash_crc = calculate_crc16_hw((uint8_t*)&ev_data_write_, (sizeof(ev_data_write_) - sizeof(ev_data_write_.flash_crc)));
	save_event();
}

/**
 * @brief Stores a new event in the current events vector.
 *
 * @param event_code  Event code to be saved
 * @param dev_id  ID of the device that generated the event
 */
void write_event(EventCode event_code, uint8_t dev_id) {

	uint16_t i = ev_data_write_.next_event_pos;
	Event_t *e = &ev_data_write_.events[i];

	ds1302_get_time(RxTime_);

	e->code = event_code;
	e->device_id = dev_id;
	e->hour  = RxTime_[HOURS];
	e->min   = RxTime_[MINUTES];
	e->day   = RxTime_[DATE];
	e->month = RxTime_[MONTHS];
	e->year  = RxTime_[YEARS];

    ev_data_write_.next_event_pos = (ev_data_write_.next_event_pos + 1) % FLASH_EVENT_SIZE;
	ev_data_write_.event_counter = min(ev_data_write_.event_counter + 1, FLASH_EVENT_SIZE);

    // if (ev_data_write_.event_counter < FLASH_EVENT_SIZE) {
    //     ev_data_write_.event_counter++;
    // }

	event_new_flag_ = 1;
}

/**
 * @brief Reads the latest events vector from flash memory.
 *
 * Loads the most recent stored version of the events vector from non-volatile
 * flash storage into the internal runtime structure.
 */
HAL_StatusTypeDef read_event() {

	HAL_StatusTypeDef status = HAL_OK;
    uint32_t src_address1 = BASE_EVENT_ADDR + (FLASH_EVENT_SECTOR_ADDRESS1 * FLASH_SECTOR_SIZE);
    //char data_to_send[128];
    //uint8_t debug = 0;
    // ToDo questa va poi riscritta per due banchi nell'indirizzi di memoria preposti

    uint32_t bank2_end = BASE_EVENT_ADDR + (128 * FLASH_SECTOR_SIZE);
    if ((src_address1 + sizeof(MasterEventData_t)) > bank2_end) {
        return HAL_ERROR;
    }

    const uint8_t *flash_ptr = (const uint8_t *)src_address1;

    uint8_t *dst = (uint8_t *)&ev_data_read_;
    for (size_t i = 0; i < sizeof(MasterEventData_t); i++) {
        dst[i] = flash_ptr[i];
        if ((i & 0x0F) == 0x0F) {
            //char buf[64];
        }
    }
    return status;

}

/**
 * @brief Saves the current events vector to flash memory.
 *
 * Writes the entire content of the internal events vector to non-volatile
 * flash storage, persisting the data across power cycles.
 */
HAL_StatusTypeDef save_event() {

    HAL_StatusTypeDef status = HAL_OK;
    const uint32_t dst_address1 = BASE_EVENT_ADDR + (FLASH_EVENT_SECTOR_ADDRESS1 * FLASH_EVENT_SIZE);
    //const uint32_t dst_address2 = BASE_EVENT_ADDR + (FLASH_EVENT_SECTOR_ADDRESS2 * FLASH_EVENT_SIZE);
	const uint32_t total_len   = (uint32_t)sizeof(MasterEventData_t);
	// ToDo [ISELQUI] qui scrivo la procedura per un solo settore, poi andrà riscritto per due settori, nell'area predisposta
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t sectorError = 0;
	//char data_to_send[128];
	//const uint8_t debug = 0;

	ev_data_write_.flash_crc = calculate_crc16_hw((const uint8_t*)&ev_data_write_, total_len - (uint32_t)sizeof(ev_data_write_.flash_crc));

	uint8_t flat_buffer[sizeof(MasterEventData_t)];
	memcpy(flat_buffer, &ev_data_write_, total_len);

	HAL_FLASH_Unlock();

	uint32_t first_sector1 = (dst_address1 - BASE_EVENT_ADDR) / FLASH_EVENT_SIZE;
	uint32_t last_sector1  = (dst_address1 + total_len - 1u - BASE_EVENT_ADDR) / FLASH_EVENT_SIZE;

	//uint32_t first_sector2 = (dst_address2 - BASE_EVENT_ADDR) / FLASH_EVENT_SIZE;
	//uint32_t last_sector2  = (dst_address2 + total_len - 1u - BASE_EVENT_ADDR) / FLASH_EVENT_SIZE;

	eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseInit.Banks     = FLASH_BANK_1;
	eraseInit.Sector    = first_sector1;
	eraseInit.NbSectors = (last_sector1 - first_sector1 + 1u);

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);
	if (status != HAL_OK) {
		HAL_FLASH_Lock();
		return status;
	}

	const volatile uint8_t *p = (const volatile uint8_t *)dst_address1;
    uint32_t bad = 0xFFFFFFFFu;
    for (uint32_t i = 0; i < total_len; i++) {
        if (p[i] != 0xFFu) { bad = i; break; }
    }
    if (bad != 0xFFFFFFFFu) {
        HAL_FLASH_Lock();
        return HAL_ERROR;
    }

    for (uint32_t offset = 0; offset < total_len; offset += 16U) {
    	uint8_t quad[16];
    	const uint32_t remaining = total_len - offset;
    	const uint32_t to_copy   = (remaining >= 16U) ? 16U : remaining;

    	memset(quad, 0xFF, sizeof(quad));                  // padding per eventuale ultimo blocco
    	memcpy(quad, &flat_buffer[offset], to_copy);

    	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, dst_address1 + offset,
                               (uint32_t)(uintptr_t)&quad[0]);
    	if (status != HAL_OK) {
    		break;
    	}
    }
    HAL_FLASH_Lock();

    if (status != HAL_OK) return status;

    return status;

}
#endif

#ifndef MASTER

void send_event(EventCode event) {
	char data[100];
	sprintf(data, "ERROR: %d", event);
	UART_Print(data);
	// ToDo: implementare il send del messaggio evento su CAN
}

#endif
