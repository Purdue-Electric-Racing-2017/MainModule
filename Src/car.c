/***************************************************************************
*
*     File Information
*
*     Name of File: car.c
*
*     Authors (Include Email):
*       1. Ben Ng				xbenng@gmail.com
*
*     File dependents: (header files, flow charts, referenced documentation)
*       1. car.h
*
*     File Description:
*     	Functions to control the physical car
*
***************************************************************************/
#include "car.h"


void carSetBrakeLight(Brake_light_status_t status)
/***************************************************************************
*
*     Function Information
*
*     Name of Function: setBrakeLight
*
*     Programmer's Name: Ben Ng xbenng@gmail.com
*
*     Function Return Type: void
*
*     Parameters (list data type, name, and comment one per line):
*       1. Brake_light_status_t status, value to write to GPIO pin
*
*      Global Dependents:
*
*     Function Description:
*			turns brakelight on or off
***************************************************************************/
{
	HAL_GPIO_WritePin(BRAKE_LIGHT_PORT, BRAKE_LIGHT_PIN, status);
}


void carInit() {
	car.state = CAR_STATE_INIT;
	car.pb_mode = PEDALBOX_MODE_DIGITAL;
	car.throttle = 0;
	car.brake = 0;
	car.phcan = &hcan1;
	car.calibrate_flag = CALIBRATE_NONE;
	car.throttle1_min = 0x0;
	car.throttle1_max = 0xfff;
	car.throttle2_min = 0x0;
	car.throttle2_max = 0xfff;
	car.throttle1_min = 0x0;
	car.throttle1_max = 0xfff;
	car.throttle2_min = 0x0;
	car.throttle2_max = 0xfff;


}

void ISR_StartButtonPressed() {
	if (car.state == CAR_STATE_INIT)
	{
//		if (car.brake >= BRAKE_PRESSED_THRESHOLD) //check if brake is pressed before starting car
			car.state = CAR_STATE_PREREADY2DRIVE;
	}
}
