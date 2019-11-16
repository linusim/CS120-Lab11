/*	Author: lim001
 *  Partner(s) Name: Festo Bwogi
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/interrupt.h>
#include "io.c"
#include "io.h"
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#include <avr/io.h>

enum States{start, press} state;
	
unsigned char val = 0x1F;
unsigned char temp = 0;

int tick(int state){
	
	switch(state){
		default:
			state = start;
			val = 0x1F;
			break;
		
		case start:
			if (val == 0x1F){
				state = start;
			}
			else{
				state = press;
			}
			break;
		
		case press:
			if (val == 0x1F){
				state = start;
			}
			else{
				state = press;
			}
			break;
		
		
		
		
	}
	
	switch(state){
		case start:
			val = GetKeypadKey();
			break;
		
		case press:
			val = GetKeypadKey();
			temp = GetKeypadKey();
			break;
		
		default:
		break;
	
	}
	
	return state;
}



int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xF0;
	PORTC = 0x0F;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	static task t;
	
	task *tasks[] = {&t};
	
	unsigned long int task_period = 300;
	unsigned long int task_tick = 300;
	const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	
	
	t.state = -1;
	t.period = task_period;
	t.elapsedTime = task_tick;
	t.TickFct = &tick;
	
	TimerSet(task_tick);
	TimerOn();
	LCD_init();
	
	unsigned char val1 = 0;
	
	while (1)
	{
		for (unsigned short i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		switch (temp) {
			LCD_ClearScreen();
			
			case '\0': PORTB = 0x1F; break;
			case '1': PORTB = 0x01; val1 = 1; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '2': PORTB = 0x02; val1 = 2; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '3': PORTB = 0x03; val1 = 3; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '4': PORTB = 0x04; val1 = 4; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '5': PORTB = 0x05; val1 = 5; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '6': PORTB = 0x06; val1 = 6; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '7': PORTB = 0x07; val1 = 7; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '8': PORTB = 0x08; val1 = 8; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '9': PORTB = 0x09; val1 = 9; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			case '0': PORTB = 0x00; val1 = 0; LCD_Cursor(1); LCD_WriteData(val1 + '0'); break;
			
			case 'A': PORTB = 0x0A; val1 = 65; LCD_Cursor(1); LCD_WriteData(val1); break;
			case 'B': PORTB = 0x0B; val1 = 66; LCD_Cursor(1); LCD_WriteData(val1); break;
			case 'C': PORTB = 0x0C; val1 = 67; LCD_Cursor(1); LCD_WriteData(val1); break;
			case 'D': PORTB = 0x0D; val1 = 68; LCD_Cursor(1); LCD_WriteData(val1); break;
			case '*': PORTB = 0x0E; val1 = 42; LCD_Cursor(1); LCD_WriteData(val1); break;
			case '#': PORTB = 0x0F; val1 = 35; LCD_Cursor(1); LCD_WriteData(val1); break;
			default: PORTB = 0x1B; break;
			
		}
		
		while(!TimerFlag){};
		TimerFlag = 0;
		
		
	}
}
