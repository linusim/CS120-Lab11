/*	Author: lim001
 *  Partner(s) Name: Festo Bwogi
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "io.h"
#include "io.c"
#include "scheduler.h"
#include "timer.h"
#include "bit.h"

char* str = "C120b is Legend... wait for it DARY! ";
unsigned char cnt = 0;
unsigned char message[16];



enum States{start} state;

int tick(int state){
	for (unsigned int j = 0; j < 16; j++){
		message[j] = str[(cnt + j) % 38];
	}
	cnt = (cnt + 1) % 38;
	LCD_DisplayString(1, message);
	state = start;
	
	
	
	
	return state;
}

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	
	static task t;
	task *tasks[] = {&t};
	const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	
	
	t.state = -1;
	t.period = 35;
	t.elapsedTime = 35;
	t.TickFct = &tick;
	
	TimerSet(35);
	TimerOn();
	LCD_init();
	
	while (1)
	{
		for (unsigned short i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		while(!TimerFlag){};
		TimerFlag = 0;
		
		
	}
}
