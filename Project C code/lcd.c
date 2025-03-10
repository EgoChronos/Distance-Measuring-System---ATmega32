/*
 * lcd.c
 *
 *  Created on: 24 May 2024
 *      Author: ahmed
 */

#include "lcd.h"
#include "avr/delay.h"
#include "gpio.h"
#include "common_macros.h"
#include "itoafcn.h"

/*******************************************************************************
 *                              Functions Definition                           *
 *******************************************************************************/
/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void)
{
	/*setup RS Pin as output to choose between Command or Data*/
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);

	/*setup enable of LCD as output*/
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, PIN_OUTPUT);

/*make the data/command port/pins as output*/
#if (LCD_NUM_OF_DATA_PINS == 8)

		GPIO_setupPortDirection(LCD_DATA_Command_PORT_ID, PORT_OUTPUT);
#elif (LCD_NUM_OF_DATA_PINS == 4)
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+0,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+1,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+2,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+3,PIN_OUTPUT);
#endif

	/*make delay to give the LCD time to start*/
	_delay_ms(20);

#if (LCD_NUM_OF_DATA_PINS == 8)
	/*Set command to setup LCD with 2 lines - 5*8 MATRIX*/
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#elif (LCD_NUM_OF_DATA_PINS == 4)
	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);
#endif

	/*make cursor OFF*/
	LCD_sendCommand(LCD_CURSOR_OFF);
	/*Clear screen*/
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}

/*
 * Description :
 * de-Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver as input to return to default*/

void LCD_deinit(void)
{
	/*setup RS as input*/
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID, PIN_INPUT);

	/*setup enable of LCD as input*/
	GPIO_setupPinDirection(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, PIN_INPUT);

/*make the data/command port/pins as input*/
#if (LCD_NUM_OF_DATA_PINS == 8)

		GPIO_setupPortDirection(LCD_DATA_Command_PORT_ID, PORT_INPUT);
#elif (LCD_NUM_OF_DATA_PINS == 4)
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+2,PIN_INPUT);
	GPIO_setupPinDirection(LCD_DATA_Command_PORT_ID,LCD_DATA_Command_FIRST_PIN_ID+3,PIN_INPUT);
#endif

}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(const uint8 command)
{
	/*put RS bit to send command*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID, SEND_COMMAND);

	/*delay to set the enable*/
	_delay_ms(1);

	/*enable LCD*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_ENABLE);

	/*delay after the enable*/
	_delay_ms(1);

	/*send Command*/
	/*configuration of 8 or 4 data pins*/
#if (LCD_NUM_OF_DATA_PINS == 8)
	GPIO_writePort(LCD_DATA_Command_PORT_ID, command);

#elif (LCD_NUM_OF_DATA_PINS == 4)
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+0, GET_BIT(command, 4));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+1, GET_BIT(command, 5));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+2, GET_BIT(command, 6));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+3, GET_BIT(command, 7));

	_delay_ms(1);
	/*disable LCD/ clear enable pin to disable the LCD*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_DISABLE);
	_delay_ms(1);
	/*enable LCD*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_ENABLE);
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+0, GET_BIT(command, 0));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+1, GET_BIT(command, 1));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+2, GET_BIT(command, 2));
	GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+3, GET_BIT(command, 3));
#endif
	/*delay after writing data*/
	_delay_ms(1);

	/*disable LCD/ clear enable pin to disable the LCD*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_DISABLE);

	/*delay after clearing enable pin of LCD*/
	_delay_ms(1);
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(const uint8 data)
{
	/*setup RS to send data*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_RS_PIN_ID, SEND_DATA);

	/*delay to set the enable*/
	_delay_ms(1);

	/*set LCD enable*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_ENABLE);

	/*delay after the enable*/
	_delay_ms(1);

	/*send Data*/
	/*configuration of 8 or 4 data pins*/
	#if (LCD_NUM_OF_DATA_PINS == 8)
		GPIO_writePort(LCD_DATA_Command_PORT_ID, data);

	#elif (LCD_NUM_OF_DATA_PINS == 4)
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+0, GET_BIT(data, 4));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+1, GET_BIT(data, 5));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+2, GET_BIT(data, 6));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+3, GET_BIT(data, 7));

		_delay_ms(1);
		/*disable LCD/ clear enable pin to disable the LCD*/
		GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_DISABLE);
		_delay_ms(1);
		/*enable LCD*/
		GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_ENABLE);
		_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+0, GET_BIT(data, 0));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+1, GET_BIT(data, 1));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+2, GET_BIT(data, 2));
		GPIO_writePin(LCD_DATA_Command_PORT_ID, LCD_DATA_Command_FIRST_PIN_ID+3, GET_BIT(data, 3));
	#endif

	/*delay after WRITING data*/
	_delay_ms(1);

	/*clear LCD enable*/
	GPIO_writePin(LCD_CONTROL_PORT_ID, LCD_ENABLE_PIN_ID, LCD_SET_DISABLE);

	/*delay after clearing enable LCD*/
	_delay_ms(1);
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const uint8* ptr2Str)
{
	/*display the string until its end*/
	if (ptr2Str == NULL_PTR)
	{
		LCD_displayCharacter('E');
		LCD_displayCharacter('R');
		LCD_displayCharacter('O');
		LCD_displayCharacter('R');
		LCD_displayCharacter('R');
		LCD_displayCharacter('0');
	}
	else
	{
	while(*ptr2Str != '\0')
	{
		LCD_displayCharacter(*ptr2Str);
		++ptr2Str;
	}
	}
}

/*Description
 * move the cursor to the required position
 */
void LCD_moveCursor(const uint8 row,const uint8 column)
{
	uint8 lcd_memory_address;
	switch (row)
	{
	case 0:
		lcd_memory_address = column;
		break;
	case 1:
		lcd_memory_address = 0x40 + column;
		break;
	case 2:
		lcd_memory_address = 0x10 + column;
		break;
	case 3:
		lcd_memory_address = 0x50 + column;
		break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | (LCD_SET_CURSOR_LOCATION));

}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displaystringRowColumn(const uint8 row,const uint8 column,const uint8* ptr2Str)
{
	/* go to to the required LCD position */
	LCD_moveCursor(row, column);
	/*display the string*/
	LCD_displayString(ptr2Str);

}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_displayIntegerToString(const uint16 data)
{
	/*a string to hold the ASCII result*/
	uint8 buff[16];
	/* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	itoa_FCN(data,(char*)buff,10);
	/* Display the string */
	LCD_displayString(buff);
}
