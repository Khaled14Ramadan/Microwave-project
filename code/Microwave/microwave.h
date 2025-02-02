/*
 * microwave.h
 *
 *  Created on: ??�/??�/????
 *      Author: khaled Ramandan
 */

#ifndef MICROWAVE_H_
#define MICROWAVE_H_

#include"keypad.h"
#include"lcd.h"
#include"timer1.h"


#define TIMER(X1 , X2 , Y1 ,Y2) 	{ \
		PORTA =(PORTA & 0xF0) | 0x01;\
		PORTC =(PORTC & 0xF0)|(X1 & 0x0F);\
		_delay_ms(2);\
		\
		PORTA =(PORTA & 0xF0) | 0x02;\
		PORTC =(PORTC & 0xF0)|(X2 & 0x0F);\
		_delay_ms(2);\
		\
		PORTA =(PORTA & 0xF0) | 0x04;\
		PORTC =(PORTC & 0xF0)|(Y1 & 0x0F);\
		_delay_ms(2);\
		\
		PORTA =(PORTA & 0xF0) | 0x08;\
		PORTC =(PORTC & 0xF0)|(Y2 & 0x0F);\
		_delay_ms(2);\
}

/*  macro ON_HEATER
 *  Lamp is ON
 *   Heater is ON
 *   Motor is ON
 *
 */
#define ON_HEATER {\
		SET_BIT(PORTD ,PD7);\
		SET_BIT(PORTD ,PD0);\
		SET_BIT(PORTD ,PD3);\
}

/*  macro OFF_HEATER
 *  Lamp is OFF
 *   Heater is OFF
 *   Motor is OFF
 *    stop time
 *
 */
#define OFF_HEATER {\
		CLEAR_BIT(PORTD ,PD7);\
		CLEAR_BIT(PORTD ,PD0);\
		CLEAR_BIT(PORTD , PD3);\
		check=1;\
}

#endif /* MICROWAVE_H_ */
