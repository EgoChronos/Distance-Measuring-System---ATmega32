/*
 * icu.c
 *
 *  Created on: 5 Jun 2024
 *      Author: ahmed
 */


#include "common_macros.h"
#include "gpio.h"
#include "icu.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                              Private Global variables                           *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callbackFcn_ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER1_CAPT_vect)
{
	if (g_callbackFcn_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callbackFcn_ptr)();
	}
}

/*******************************************************************************
 *                              Functions Definition                           *
 *******************************************************************************/
/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallbackFcn(void (*ptr2fcn)(void))
{
	g_callbackFcn_ptr = ptr2fcn;
}

/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const ICU_ConfigType* configType_ptr)
{
	/*Setup the pin direction as input pin to receive the wave*/
	GPIO_setupPinDirection(ICU_PORT_ID, ICU_PIN_ID, PIN_INPUT);
	/* Timer1 always operates in Normal Mode */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	 /* insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | (configType_ptr->edgeType <<6);
	/*setting the pre-scaler*/
	TCCR1B = (TCCR1B & 0xF8) | ( configType_ptr->prescaler& (0x07));
	/*make the initial value of Timer1 = 0*/
	TCNT1 = 0;
	/*make the initial value of input capture register = 0*/
	ICR1 = 0;
	/*Input capture interrupt enable*/
	SET_BIT(TIMSK, TICIE1);
}

/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const ICU_edgeCaptureType a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType<<6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void)
{
	/* Clear All Timer1/ICU Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1<<TICIE1);

	/* Reset the global pointer value */
	g_callbackFcn_ptr = NULL_PTR;
}













