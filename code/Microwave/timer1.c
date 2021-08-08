/*
 * timer1.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: AL-Esraa
 */
#include"timer1.h"

void timer1_INT(void)
{
	/* FOC1A to timer1_cTc and not PWM*/
	TCCR1A =(1<<FOC1A);

	/* WGM12 to work CTC immediate */
	/* CS12 and CS10 to N = 1024 */
	TCCR1B =(1<<WGM12)|(1<<CS12)|(1<<CS10);

	TCNT1 = 0;
	/* compare when 1 SEC */
	OCR1A =1000;

	/* enable CTC*/
	TIMSK |=(1<<OCIE1A);
}
