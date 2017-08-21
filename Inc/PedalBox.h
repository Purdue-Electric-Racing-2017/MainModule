/*
 * PedalBox.h
 *
 *  Created on: Jan 12, 2017
 *      Author: ben
 */

#ifndef PEDALBOX_H_
#define PEDALBOX_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

typedef enum {
	PEDALBOX_STATUS_ERROR = 1,
	PEDALBOX_STATUS_NO_ERROR = 0
} Pedalbox_status_t;

// Structure to hold data passed through the queue to pedalBoxMsgHandler
typedef struct _pedalbox_msg {
	Pedalbox_status_t 		EOR; 				// EV 2.4.6: Encoder out of range
	Pedalbox_status_t 		APPS_Implausible; 	// EV 2.3.5
	uint32_t 				throttle1_raw;		// raw throttle data from pedalbox
	uint32_t 				throttle2_raw;
	uint32_t  				brake1_raw;
	uint32_t 				brake2_raw;

} Pedalbox_msg_t;

typedef struct {
	uint16_t				count;
	uint16_t				period_ms;
	Pedalbox_msg_t			msg;
} GeneratePedalboxMessages_t;

#endif /* PEDALBOX_H_ */
