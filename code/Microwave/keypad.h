/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the Keypad driver
 *
 * Author: Khaled Ramadan
 *
 *******************************************************************************/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define N_COL 3
#define N_ROW 4

/* Keypad Port Configurations */
#define KEYPAD_PORT_DIR DDRB
#define KEYPAD_PORT_OUT PORTB
#define KEYPAD_PORT_IN PINB


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function responsible for getting the pressed keypad key
 */
uint8 Key_getPressedKey(void);



#endif
