/*
 * lcd.h
 *
 *  Created on: 24 May 2024
 *      Author: ahmed
 */

#ifndef LCD_H_
#define LCD_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*Define the number of data pins 4 or 8*/
#define LCD_NUM_OF_DATA_PINS 					8

#if((LCD_NUM_OF_DATA_PINS != 4) && (LCD_NUM_OF_DATA_PINS != 8))

#error "Number of Data bits should be equal to 4 or 8"
#endif

/*DATA and COMMAND port and pins*/
#define LCD_DATA_Command_PORT_ID 		   PORTA_ID
#define LCD_DATA_Command_FIRST_PIN_ID 		PIN0_ID

/*control LCD port*/
#define LCD_CONTROL_PORT_ID PORTB_ID
/*RS pin and choose command state or data state*/
#define LCD_RS_PIN_ID 						PIN0_ID
#define SEND_COMMAND          			   LOGIC_LOW
#define SEND_DATA 						   LOGIC_HIGH
/*enable LCD pin and states*/
#define LCD_ENABLE_PIN_ID				    PIN1_ID
#define LCD_SET_ENABLE 					   LOGIC_HIGH
#define LCD_SET_DISABLE 				   LOGIC_LOW

/* LCD Commands */
#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);

/*de-init of the LCD*/
void LCD_deinit(void);

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(const uint8 command);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(const uint8 data);

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const uint8* ptr2Str);

/*Description
 * move the cursor to the required position
 */
void LCD_moveCursor(const uint8 row,const uint8 column);

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displaystringRowColumn(const uint8 row,const uint8 column,const uint8* ptr2Str);

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_displayIntegerToString(const uint16 data);
#endif /* LCD_H_ */
