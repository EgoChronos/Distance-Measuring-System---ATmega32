/*
 * ultrasonic.c
 *
 *  Created on: 24 May 2024
 *      Author: ahmed
 */
#include "ultrasonic.h"
#include <util/delay.h>

/*******************************************************************************
 *                              Static global variables                           *
 *******************************************************************************/
static uint16 highEdgeTime = 0; // timer for high edge of echo signal
static uint8 edgeCount = 0;     // counter to count calling call back function

/*******************************************************************************
 *                              Functions Definition                           *
 *******************************************************************************/

/* Setup the ICU call back function.
 * Initialize the ICU driver as required.
 * Setup the direction for the trigger pin as output pin through the GPIO driver.
 * */
void Ultrasonic_init(void)
{
	/*set callback fcn*/
	ICU_setCallbackFcn(Ultrasonic_edgeProcessing);

	/*initialise the ICU*/
	ICU_ConfigType ICU_configurations = {ICU_PRESCALER_8, Rising_Edge};
	ICU_init(&ICU_configurations);

	/*set the trigger pin of the Ultrasonic as output*/
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, PIN_OUTPUT);
}

/* Send the Trigger pulse to the ultrasonic for 10 microsecond*/
void Ultrasonic_Trigger(void)
{
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);
}

/* Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment
 */
uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();

	/*return the distance travelled*/
	return ((uint16) (((float)highEdgeTime)/57.5));
}

/* This is the call back function called by the ICU driver
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void)
{
	/* when we detect a rising edge we clear the timer to start counting
	 * and wait for a falling edge
	 */
	if (edgeCount == 0)
	{
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(Falling_Edge);
		edgeCount = 1;
	}
	/* when we catch a falling edge
	 * we save the timer value at highEdgeTime
	 * , clear the timer and wait for a rising edge.
	 */
	else if (edgeCount == 1)
	{
		highEdgeTime = ICU_getInputCaptureValue();
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(Rising_Edge);
		edgeCount = 0;
	}
}

