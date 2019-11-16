/*	Author: lim001
 *  Partner(s) Name: Festo Bwogi
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "keypad.h"
#include "io.h"
#include "io.c"
#include "timer.h"
#include "scheduler.h"


unsigned char key = 0x00;
unsigned char cursor_pos = 0x01;

int Tick(int state)
{
	switch(state)
	{
		default:
		key = GetKeypadKey();
		if(key != '\0')
		{
			LCD_WriteData(key);
			if(cursor_pos>15)
			{
				cursor_pos=1;
				LCD_Cursor(cursor_pos);
			}
			else
			{
				cursor_pos++;
				LCD_Cursor(cursor_pos);
			}
		}
		break;
	}
	return 0;
}


int main()
{
	DDRB = 0xFF;PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	static task t;

	task *tasks[] = {&t};

	t.state = -1;
	t.period = 300;
	t.elapsedTime = 30;
	t.TickFct = &Tick;

	TimerOn();
	TimerSet(300);
	LCD_init();
	const char* a = "Congratulations!";
	LCD_DisplayString(1,a);
	LCD_Cursor(cursor_pos);

	while(1)
	{
		tasks[0]->TickFct(0);
		while(!TimerFlag){}
		TimerFlag=0;
	}
}
