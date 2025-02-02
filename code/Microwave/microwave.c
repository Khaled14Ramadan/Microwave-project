/*
 * microave.c
 *
 *  Created on: ??�/??�/????
 *      Author: Khaled Ramadan
 */


#include"microwave.h"




unsigned char num_sec=0; //the number of second
unsigned char num_minuts=0; // the number of minutes
volatile unsigned char check=1;
volatile unsigned char check_cancel=0;


ISR(TIMER1_COMPA_vect)
{
	/* if press button stop timer and press again rework timer ( INTERRUPT 0)*/
	if(check %2==0)
	{
		num_sec--;

	}


}

/*
 * External INT0 Interrupt Service Routine
 * cancel button
 */
ISR(INT0_vect)
{
	check_cancel=1;
}

// External INT0 enable and configuration function
void INT0_Init(void)
{
	// Enable external interrupt pin INT0
	GICR  |= (1<<INT0);
	// Trigger INT0 with the falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);
}



int main()
{
	uint8 num_sec1=0;
	uint8 num_sec2=0;

	uint8 num_minut1=0;
	uint8 num_minut2=0;

	uint8 start =0;/* to change when bush button start */

	uint8 check_lcd=0; /*to print on LCD only one and don't repeat print */


	DDRC |=0x0F;
	PORTC &=0xF0;


	DDRA |=0x0F;
	PORTA &=0xF0;



	/* internal pull ap
	 * PB7 is the input pin to start button
	 * PA4 is the input pin to door sensor
	 * PA5 is the input pin to weight sensor
	 * PD2 is the input pin to cancel button
	 */
	CLEAR_BIT(DDRD , PD2);
	SET_BIT(PORTD , PD2);

	CLEAR_BIT(DDRB , PB7);
	SET_BIT(PORTB , PB7);

	CLEAR_BIT(DDRA , PA4);
	SET_BIT(PORTA , PA4);
	CLEAR_BIT(DDRA , PA5);
	SET_BIT(PORTA , PA5);

	/* output pins
	 * PD7 is the output pin to led
	 * PD3 is the output pin to heater
	 * PD0 and PD1 are the output pin to motor
	 * PD2 is the input pin to cancel button
	 */
	SET_BIT(DDRD , PD7);
	CLEAR_BIT(PORTD , PD7);

	/* motor */
	SET_BIT(DDRD , PD0);
	CLEAR_BIT(PORTD , PD0);
	SET_BIT(DDRD , PD1);
	CLEAR_BIT(PORTD , PD1);

	/* Heater */
	SET_BIT(DDRD , PD3);
	CLEAR_BIT(PORTD , PD3);




	/* initialize LCD */
	LCD_init();

	/* to enter the number of timer on keypad */
	LCD_clearScreen();
	LCD_displayString("Enter new time");
	while(num_minuts ==0)
	{

		num_minut2 = Key_getPressedKey();
		/* to show num_minut2 on 7_segement */
		PORTA =(PORTA & 0xF0) | 0x08;
		PORTC =(PORTC & 0xF0)|(num_minut2 & 0x0F);
		_delay_ms(500);

		num_minut1 = Key_getPressedKey();

		num_minuts=num_minut2 *10 + num_minut1;
	}


	SREG |=(1<<7);/*enable interrupts */

	/* work timer */
	timer1_INT();

	/* closed button */
	INT0_Init();




	for(;;)
	{

		/*
		 * this is macro to timer
		 */
		TIMER(num_sec1 , num_sec2 , num_minut1 , num_minut2);
		/***********************************************************/

		if(check_cancel ==0) /* this not push cancel button */
		{
			/*
			 * PA4 to to check Door sensor
			 * PA5 to check weight sensor
			 */
			if(BIT_IS_CLEAR(PINA , PA4) && BIT_IS_CLEAR(PINA , PA5) )
			{



				check_lcd =0;/*to start LCD from start button */
				while (BIT_IS_SET(PINB , PB7) && start ==0)
				{

					/*
					 * if push cancel button (interrupt 0 )
					 */
					if(check_cancel == 1)
					{
						num_sec1=0;
						num_sec2=0;
						num_minuts=0;
						num_minut1=0;
						num_minut2=0;

						PORTA =(PORTA & 0xF0) | 0x01;
						PORTC =(PORTC & 0xF0)|(num_sec1 & 0x0F);

						LCD_clearScreen();
						LCD_displayString("removed time setting");
						_delay_ms(1000);
						LCD_clearScreen();
						LCD_goToRowColumn(1,0);
						LCD_displayString("Enter new time ");

						while(num_minuts ==0)
						{

							num_minut2 = Key_getPressedKey();
							/* to show num_minut2 on 7_segement */
							PORTA =(PORTA & 0xF0) | 0x08;
							PORTC =(PORTC & 0xF0)|(num_minut2 & 0x0F);
							_delay_ms(500);

							num_minut1 = Key_getPressedKey();

							num_minuts=num_minut2 *10 + num_minut1;
						}
						check_cancel=0;
						check_lcd=0;

					}
					/********************************************/
					/*
					 * to display on LCD
					 */
					if(check_lcd == 0)
					{
						LCD_clearScreen();
						LCD_displayString("push start button");
						check_lcd=1;
					}

					TIMER(num_sec1 , num_sec2 , num_minut1 , num_minut2);
				}

				/*
				 * to display on LCD
				 */
				if(check_lcd == 1)
				{
					LCD_clearScreen();
					LCD_displayString("heater is working");
					check_lcd=0;
				}


				/*  macro
				 *  Lamp is ON
				 *   Heater is ON
				 *   Motor is ON
				 *    Display shows remaining time
				 *
				 */
				ON_HEATER
				/****************************************************************/

				start = 1; /* to do not go to pooling to check start button */
				check = 0 ;/* to start decrement time */

				/* to calculate the time */
				if(num_sec > 0)
				{
					if(num_sec == 60)
					{
						num_sec1 = 0;
						num_sec2 = 0;

					}
					else
					{
						num_minut1=(num_minuts % 10);
						num_minut2=(num_minuts /10);

						num_sec1=(num_sec % 10);
						num_sec2=(num_sec /10);
					}

				}
				else if(num_minuts > 0)
				{
					/*when num_sec=0 */
					num_sec=60;


					num_minut1=(num_minuts % 10);
					num_minut2=(num_minuts /10);
					num_minuts--;

				}

				else/* finish time */
				{
					num_sec1=0;

					/*  macro
					 *  Lamp is OFF
					 *   Heater is OFF
					 *   Motor is OFF
					 *    stop time
					 *
					 */

					OFF_HEATER

					/****************************************/

					if(check_lcd == 1)
					{
						LCD_clearScreen();
						LCD_displayString("heating is finish");
						check_lcd=0;
					}

				}
			}

			else /* door is not closed or food is not found */
			{
				/*  macro
				 *  Lamp is OFF
				 *   Heater is OFF
				 *   Motor is OFF
				 *    stop time
				 *
				 */

				OFF_HEATER

				/****************************************/
				if(check_lcd == 0 || start == 1)
				{
					/*to display on LCD
					 * check_lcd==0 if it do not start work
					 * start==0 if it started work
					 */
					LCD_clearScreen();
					LCD_displayString("Door isn't closed");
					LCD_goToRowColumn(1,0);
					LCD_displayString("or not found food");
					check_lcd=1;
					start = 0;
				}
			}

		}


		/* push cancel button */
		else
		{
			if(start==1 )
			{
				/*
				 * heater is working
				 *
				 */

				/*  macro
				 *  Lamp is OFF
				 *   Heater is OFF
				 *   Motor is OFF
				 *    stop time
				 */

				OFF_HEATER
				/****************************************/

				start =0; /* push start button to rework heater */

				check_cancel = 0;
			}
			else
			{
				/*
				 * heater is not working
				 *remove the time
				 */
				num_sec1=0;
				num_sec2=0;
				num_minuts=0;
				num_minut1=0;
				num_minut2=0;


				PORTA =(PORTA & 0xF0) | 0x01;
				PORTC =(PORTC & 0xF0)|(num_sec1 & 0x0F);

				LCD_clearScreen();
				LCD_displayString("removed time setting");
				_delay_ms(1000);
				LCD_clearScreen();
				LCD_goToRowColumn(1,0);
				LCD_displayString("Enter new time ");

				while(num_minuts ==0)
				{

					num_minut2 = Key_getPressedKey();
					/* to show num_minut2 on 7_segement */
					PORTA =(PORTA & 0xF0) | 0x08;
					PORTC =(PORTC & 0xF0)|(num_minut2 & 0x0F);
					_delay_ms(500);

					num_minut1 = Key_getPressedKey();

					num_minuts=num_minut2 *10 + num_minut1;
				}
				check_cancel=0;
				check_lcd=0;
			}

		}



	}
}









