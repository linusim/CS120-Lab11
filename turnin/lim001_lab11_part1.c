/*	Author: lim001
 *  Partner(s) Name: Festo Bwogi
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "keypad.h"
#include "io.h"
#include "scheduler.h"
#include "timer.h"

unsigned char val = 0x1F;


enum States{init, press} state;

int tick(int state){
	
	switch(state){
		default:
		state = init;
		val = 0x1F;
		break;
		
		case init:
		if (val == 0x1F){
			state = init;
		}
		else{
			state = press;
		}
		break;
		
		case press:
		if (val == 0x1F){
			state = init;
		}
		else{
			state = press;
		}
		break;
		
		
		
	}
	
	switch(state){
		case init:
		break;
		
		case press:
		val = GetKeypadKey();
		break;
		
		default:
		val = 0x1F;
		break;
		
		
	}
	
	return state;
}

int main(void)
{
	unsigned char val;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xF0;
	PORTC = 0x0F;
	
	static task task1;
	
	task *tasks[] = {&task1};
	
	unsigned long int task_period = 10;
	unsigned long int task_tick = 10;
	const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	
	
	task1.state = -1;
	task1.period = task_period;
	task1.elapsedTime = task_tick;
	task1.TickFct = &tick;
	
	TimerSet(task_tick);
	TimerOn();
	
	while (1)
	{
		
		for (unsigned short i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state =
				tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		val = GetKeypadKey();
		switch (val) {
			case '\0': PORTB = 0x1F; break;
			case '1': PORTB = 0x01; break;
			case '2': PORTB = 0x02; break;
			case '3': PORTB = 0x03; break;
			case '4': PORTB = 0x04; break;
			case '5': PORTB = 0x05; break;
			case '6': PORTB = 0x06; break;
			case '7': PORTB = 0x07; break;
			case '8': PORTB = 0x08; break;
			case '9': PORTB = 0x09; break;
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break;
			case 'D': PORTB = 0x0D; break;
			case '*': PORTB = 0x0E; break;
			case '0': PORTB = 0x00; break;
			case '#': PORTB = 0x0F; break;
						default: PORTB = 0x1B; break;
			
		}
	}
}
