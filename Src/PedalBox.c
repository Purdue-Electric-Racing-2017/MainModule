/*
 * PedalBox.c
 *
 *  Created on: Jan 12, 2017
 *      Author: ben
 */

#include "PedalBox.h"
#include "car.h"


void taskPedalBoxMsgHandler() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: pedalBoxMessageHandler
*
*     Programmer's Name: 	Kai Strubel
*     						Ben Ng			xbenng@gmail.com
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.Pedalbox_msg_t msg
			brake_level from pedalbox potentiometer
*			throttle_level from pedalbox potentiometer
*			APPS_Implausible flag
*			EOR flag
*		2.
*
*      Global Dependents:
*
*     Function Description:
*			Takes message from pedal box, runs safetly check, sets throttle.
*			Designed this way so pedalboxmsg's can be generated multiple ways,
*			and not disrupt the logic behind processing the data.
***************************************************************************/

	Pedalbox_msg_t pedalboxmsg;		//struct to store pedalbox msg

	while (1) {

		if(xQueueReceive(car.q_pedalboxmsg, &pedalboxmsg, 1000)){
			//get current time in ms
			uint32_t current_time_ms = xTaskGetTickCount() / portTICK_PERIOD_MS;;
			// update time stamp, indicates when a pedalbox message was last received
			car.pb_msg_rx_time = current_time_ms;

			//check if calibration values should be updated
			if (car.calibrate_flag == CALIBRATE_THROTTLE_MIN) {
				car.throttle1_min = pedalboxmsg.throttle1_raw;
				car.throttle2_min = pedalboxmsg.throttle2_raw;
				car.calibrate_flag = CALIBRATE_NONE;
			} else if (car.calibrate_flag == CALIBRATE_THROTTLE_MAX) {
				car.throttle1_max = pedalboxmsg.throttle1_raw;
				car.throttle2_max = pedalboxmsg.throttle2_raw;
				car.calibrate_flag = CALIBRATE_NONE;
			} else 	if (car.calibrate_flag == CALIBRATE_BRAKE_MIN) {
				car.brake1_min = pedalboxmsg.brake1_raw;
				car.brake2_min = pedalboxmsg.brake2_raw;
				car.calibrate_flag = CALIBRATE_NONE;
			} else if (car.calibrate_flag == CALIBRATE_BRAKE_MAX) {
				car.brake1_max = pedalboxmsg.brake1_raw;
				car.brake2_max = pedalboxmsg.brake2_raw;
				car.calibrate_flag = CALIBRATE_NONE;
			}

			/////////////PROCESS DATA///////////////
			Pedalbox_status_t  	pedalbox_status = PEDALBOX_STATUS_NO_ERROR;  //local flag to be set if there is an error
			float			throttle1_cal = ((float)(pedalboxmsg.throttle1_raw - car.throttle1_min)) / (car.throttle1_max - car.throttle1_min);  //value 0-1, throttle 1 calibrated between min and max
			float			throttle2_cal = ((float)(pedalboxmsg.throttle2_raw - car.throttle2_min)) / (car.throttle2_max - car.throttle2_min);;  //value 0-1, throttle 2 calibrated between min and max
			float 			brake1_cal	  = ((float)(pedalboxmsg.brake1_raw - car.brake1_min)) / (car.brake1_max - car.brake1_min);  //value 0-1, brake 1 calibrated between min and max
			float 			brake2_cal	  = ((float)(pedalboxmsg.brake2_raw - car.brake2_min)) / (car.brake2_max - car.brake2_min);  //value 0-1, brake 2 calibrated between min and max
			float 			throttle_avg  = (throttle1_cal + throttle2_cal) / 2.0;
			float			brake_avg     = (brake1_cal + brake2_cal) / 2.0;



			// EV 2.4.6: Encoder out of range
			if ((pedalboxmsg.throttle1_raw - car.throttle1_min) > car.throttle1_max ||
				pedalboxmsg.throttle2_raw >	car.throttle2_max)
			{
				pedalbox_status = PEDALBOX_STATUS_ERROR;
			}

			//APPS Implausibility error handling, EV 2.3.5,
			//	error if throttle sensors disagree more than 10% travel
			//	for more than 100ms
			if (fabs(throttle1_cal - throttle2_cal) > .1 ) //legacy: (pedalboxmsg.APPS_Implausible == PEDALBOX_STATUS_ERROR)
			{
				//if error is persistent
				if (car.apps_imp_last_state == PEDALBOX_STATUS_ERROR)
				{
					//if time between first error and this error >= 100ms
					if (car.apps_imp_first_time_ms - current_time_ms >= 100)
					{
						pedalbox_status = PEDALBOX_STATUS_ERROR;
					}
				} else {  //else this is the first message to have an imp error
					//record the time
					car.apps_imp_first_time_ms = current_time_ms;
				}
			}
			//update last state variable so we know this current state, for next time we get an error
			car.apps_imp_last_state = pedalboxmsg.APPS_Implausible;


			//Brake
			//check if brake level is greater than the threshold level
			if (brake_avg >= BRAKE_PRESSED_THRESHOLD) {
				//brake is presssed
				carSetBrakeLight(BRAKE_LIGHT_ON);  //turn on brake light


				//EV 2.5, check if the throttle level is greater than 25% while brakes are on
				if (throttle_avg > APPS_BP_PLAUS_THRESHOLD) {
					//set apps-brake pedal plausibility error
					car.apps_bp_plaus = PEDALBOX_STATUS_ERROR;
				}
			} else {
				//brake is not pressed
				carSetBrakeLight(BRAKE_LIGHT_OFF);  //turn off brake light
			}


			if (car.apps_bp_plaus == PEDALBOX_STATUS_ERROR) {
				//EV 2.5.1, reset apps-brake pedal plausibility error only if throttle level is less than the .05
				if(throttle_avg <= APPS_BP_PLAUS_RESET_THRESHOLD){
					car.apps_bp_plaus = PEDALBOX_STATUS_NO_ERROR;
				}
			}

			//set the car's throttle to the throttle just received
//			if (pedalbox_status == PEDALBOX_STATUS_NO_ERROR)
			{
				//no errors, set throttle to value received from pedalbox
				car.throttle = throttle_avg * MAX_THROTTLE_LEVEL;
			}
//			else
//			{
//				//if there were errors, set throttle = 0
//				car.throttle = 0;
//			}
		}
	}

	//if this task breaks from the loop kill it
	vTaskDelete(NULL);
}

void taskGeneratePedalboxMessages(void * asdf) {
	while (1) {
		//xQueueSendToBack();
		//car.q_pedalboxmsg;
	}

	//if this task breaks from the loop kill it
	vTaskDelete(NULL);
}

//int storeToFlash() {
//	HAL_FLASH_Unlock();
//	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
//)
//};


