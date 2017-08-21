/***************************************************************************
*
*     File Information
*
*     Name of File: main_module_tasks.c
*
*     Authors (Include Email):
*       1. Kai Strubel			kstrubel@purdue.edu
*       2. Ben Ng				xbenng@gmail.com
*
*     File dependents: (header files, flow charts, referenced documentation)
*       1. main_module_tasks.h
*
*     File Description: Takes inputs from the pedal box,
*       handles safety checks, and sets throttle level
*
***************************************************************************/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "car.h"
#include "CANProcess.h"
#include "main_module_tasks.h"
#include "BMS.h"
#include "WheelModule.h"
#include <math.h>

void taskLaunchControl() {
	while (1) {
		vTaskDelay(LAUNCH_CONTROL_INTERVAL_MS);
	}
}


//TODO Potential MC ping function
//TODO BMS functions

int mainModuleWatchdogTask() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: mainModuleTimeCheckIdle
*
*     Programmer's Name: Kai Strubel
*     					 Ben Ng			xbenng@gmail.com
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*       
*      Global Dependents:
*	    1.bool launchControl
*		2.float MMPB_TIME time pedal box message handler function was last run 
*		3.float MMWM_TIME time wheel module handler function was last run
*		4.float torque
*		5.float currentTime
*
*     Function Description:
*		Checks if wheel module and pedal box are still communicating
*		
***************************************************************************/
	while (1) {
		/*
		//check how old the wheel module data is, if its too old, then turn off LC
		if (current_time_ms - MMWM_TIME > LC_THRESHOLD) {
			launchControl = 0;
			//error
		}*/
		vTaskDelay(500);
	}
}

int taskHeartbeat() {
/***************************************************************************
*.
*     Function Information
*
*     Name of Function: heartbeatIdle
*
*     Programmer's Name: Kai Strubel
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*       
*      Global Dependents:
*
*     Function Description:
*		Heart beat to communicate that main module is alive
*		
***************************************************************************/
	// write to GPIO
	while (1) {
		HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
		vTaskDelay(HEARTBEAT_PERIOD);
	}
}

void initRTOSObjects() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: startTasks
*
*     Programmer's Name: Ben Ng
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*
*     Global Dependents:
*
*     Function Description:
*		all xTaskCreate calls
*		all xQueueCreate calls
*
***************************************************************************/

	/* Create Queues */

	car.q_rxcan = 			xQueueCreate(QUEUE_SIZE_RXCAN, sizeof(CanRxMsgTypeDef));
	car.q_txcan = 			xQueueCreate(QUEUE_SIZE_TXCAN, sizeof(CanTxMsgTypeDef));
	car.q_pedalboxmsg = 	xQueueCreate(QUEUE_SIZE_PEDALBOXMSG, sizeof(Pedalbox_msg_t));
	car.q_mc_frame = 		xQueueCreate(QUEUE_SIZE_MCFRAME, sizeof(CanRxMsgTypeDef));

	car.m_CAN =				xSemaphoreCreateMutex(); //mutex to protect CAN peripheral

	/* Create Tasks */

	//todo optimize stack depths http://www.freertos.org/FAQMem.html#StackSize
	xTaskCreate(taskPedalBoxMsgHandler, "PedalBoxMsgHandler", 512, NULL, 1, NULL);
	xTaskCreate(taskCarMainRoutine, "CarMainRoutine", 512, NULL, 1, NULL);
	xTaskCreate(taskTXCAN, "TX CAN", 512, NULL, 1, NULL);
	xTaskCreate(taskRXCANProcess, "RX CAN Process", 1024, NULL, 1, NULL);
	xTaskCreate(taskRXCAN, "RX CAN", 128, NULL, 1, NULL);
	xTaskCreate(taskBlink, "blink", 128, NULL, 1, NULL);
 }
extern uint8_t variable;
void taskBlink(void* can)
{
	vTaskDelay(5000); //TESTING1
	while (1)
	{
		//HAL_GPIO_TogglePin(FRG_RUN_CTRL_GPIO_Port, FRG_RUN_CTRL_Pin);
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		//HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);

		CanTxMsgTypeDef tx;
		tx.IDE = CAN_ID_STD;
		tx.RTR = CAN_RTR_DATA;
		tx.StdId = 0x300;
		tx.DLC = 1;
		tx.Data[0] = variable;
		car.phcan->pTxMsg = &tx;
		HAL_CAN_Transmit_IT(car.phcan);						//transmit staged message

		//xQueueSendToBack(rtos_can1.queue_tx, &tx, 100);
		vTaskDelay(500);
	}
}


void taskSoundBuzzer(int* time_ms) {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: taskSoundBuzzer
*
*     Programmer's Name: Ben Ng
*
*     Function Return Type: void
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*
*     Global Dependents:
*
*     Function Description:
*		ready to drive sound task
*
***************************************************************************/
	while (1) {
		HAL_GPIO_WritePin(Buzzer_CTRL_GPIO_Port, Buzzer_CTRL_Pin, GPIO_PIN_SET); //turn on buzzer
		//enable FRG/RUN 0.5s after RFE.
		vTaskDelay((uint32_t) time_ms / portTICK_RATE_MS);
		HAL_GPIO_WritePin(Buzzer_CTRL_GPIO_Port, Buzzer_CTRL_Pin, GPIO_PIN_RESET); //turn off buzzer
		vTaskDelete(NULL);
	}
}

void taskCarMainRoutine() {
	while (1)
	{
		//get current time in ms
		uint32_t current_time_ms = xTaskGetTickCount() / portTICK_PERIOD_MS;

		if (car.state == CAR_STATE_INIT)
		{
			//assert these pins always
			HAL_GPIO_WritePin(SDC_CTRL_GPIO_Port, SDC_CTRL_Pin, GPIO_PIN_SET); //close SDC
			HAL_GPIO_WritePin(Motor_Controller_Relay_CTRL_GPIO_Port, Motor_Controller_Relay_CTRL_Pin, GPIO_PIN_SET); //turn on mc
		}
		if (car.state == CAR_STATE_PREREADY2DRIVE)
		{
			HAL_GPIO_WritePin(Pump_Relay_CTRL_GPIO_Port, Pump_Relay_CTRL_Pin, GPIO_PIN_SET); //turn on pump
			//bamocar 5.2
			//Contacts of the safety device closed,
			//enable FRG/RUN 0.5s after RFE.
			HAL_GPIO_WritePin(RFE_CTRL_GPIO_Port, RFE_CTRL_Pin, GPIO_PIN_SET);
			vTaskDelay(500/ portTICK_RATE_MS); //wait .5 sec
			HAL_GPIO_WritePin(FRG_RUN_CTRL_GPIO_Port, FRG_RUN_CTRL_Pin, GPIO_PIN_SET);
			//turn on buzzer
			xTaskCreate(taskSoundBuzzer, "buzzer is on", 64, (void *) 100, 1, NULL);
			car.state = CAR_STATE_READY2DRIVE;  //car is started
		}
		if (car.state == CAR_STATE_READY2DRIVE)
		{
			//assert these pins during r2d
			HAL_GPIO_WritePin(Pump_Relay_CTRL_GPIO_Port, Pump_Relay_CTRL_Pin, GPIO_PIN_SET);

			uint16_t torque_to_send = 0;

			//check if the age of the pedalbox message is greater than the timeout
			if (current_time_ms - car.pb_msg_rx_time > PEDALBOX_TIMEOUT && PEDALBOX_TIMEOUT != 0) {
				torque_to_send = 0;
				//todo send a CAN message to dash?
			} else {
				//launch control
				/*if (lc_status == LC_ACTIVATED) {
					if(throttle > apps_max) {
						// apps - accelerator pedal position system
						// bse - brake system encoder
						apps_max = throttle;
						throttle *= scaleFactor;
					}
					else {
						if (apps_max - throttle > LC_THRESHOLD) {
							launchControl = false;
						}
						else {
							throttle *= scaleFactor;

						}
					}

				}*/

				torque_to_send = car.throttle;
			}
			mcCmdTorque(torque_to_send);  //command the MC to move the motor
		}
		if (car.state == CAR_STATE_ERROR)
		{
			HAL_GPIO_WritePin(RFE_CTRL_GPIO_Port, RFE_CTRL_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(FRG_RUN_CTRL_GPIO_Port, FRG_RUN_CTRL_Pin, GPIO_PIN_RESET);


			mcCmdTorque(0);
		}

		//wait until
		vTaskDelay(TORQUE_SEND_PERIOD);

	}

}

