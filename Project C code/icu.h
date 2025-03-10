/*
 * icu.h
 *
 *  Created on: 5 Jun 2024
 *      Author: ahmed
 */

#ifndef ICU_H_
#define ICU_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/*choose port and pin of the ICU*/
#define ICU_PORT_ID 			PORTD_ID
#define ICU_PIN_ID 				PIN6_ID


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	Falling_Edge, Rising_Edge,
}ICU_edgeCaptureType;

typedef enum
{
	ICU_NO_PRESCALER, ICU_PRESCALER_1, ICU_PRESCALER_8,
	ICU_PRESCALER_64, ICU_PRESCALER_256, ICU_PRESCALER_1024,
}ICU_PrescalerConfig;
typedef struct
{
	ICU_PrescalerConfig prescaler;
	ICU_edgeCaptureType edgeType;
}ICU_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallbackFcn(void (*ptr2fcn)(void));

/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const ICU_ConfigType* configType_ptr);

/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const ICU_edgeCaptureType a_edgeType);

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void);

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void);

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void);


#endif /* ICU_H_ */
