/*
 * main.c
 *
 *  Created on: 24 May 2024
 *      Author: ahmed
 */

#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>

int main(void)
{
	/* variable to store the target distance*/
	uint16 remoteDistance = 0;

	/*enable global interrupts to use the ICU*/
	SREG |= (1 << 7);
	/* Installs the LCD and the ultrasonic
	 * clear the LCD screen and display "Distance=" */
	LCD_init();
	Ultrasonic_init();
	LCD_clearScreen();
	LCD_displayString("Distance=");

	while (1)
	{
		/*store the ultrasonic target distance in cm*/
		remoteDistance = Ultrasonic_readDistance();

		/*move the cursor and display the distance in cm*/
		LCD_moveCursor(0, 10);
		LCD_displayIntegerToString(remoteDistance);
		/* print space after the value for transition between 3-2-1 digit numbers */
		LCD_displayCharacter(' ');

		LCD_moveCursor(0, 14);
		LCD_displayString("cm");
	}
}
