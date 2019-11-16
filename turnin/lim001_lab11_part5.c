/*	Author: lim001
 *  Partner(s) Name: Festo Bwogi
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #5
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
#include "timer.h"
#include "scheduler.h"


unsigned char position[32];
unsigned char pos = 0;
unsigned char run = 1;
unsigned char cnt = 0;
unsigned char display[32];
char* str = "        *          *          *      ";

enum States{init} state;

int tick(int state){
	
	for (unsigned int j = 0; j < 32; j++){
		display[j] = str[(cnt + j) % 36];
	}
	cnt = (cnt + 1) % 36;
	
	position[32] = 0;
	
	if (display[pos] == '*'){
		LCD_DisplayString(1, "You lose! :-((");
		run = 0;
	}
	else{
		LCD_DisplayString(1, display);
	}
	
	if (!GetBit(PINA, 1)){
		pos = 1;
	}
	else if (!GetBit(PINA, 2)){
		pos = 17;
	}
	
	LCD_Cursor(pos);
	
	
	return state;
}

int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	static task t;
	task *tasks[] = {&t};
	const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	t.state = -1;
	t.period = 50;
	t.elapsedTime = 50;
	t.TickFct = &tick;
	
	TimerSet(50);
	TimerOn();
	LCD_init();
	
	while (1) {
		
		if (run){
			for (unsigned short i = 0; i < numTasks; i++ ) {
				if ( tasks[i]->elapsedTime == tasks[i]->period ) {
					tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
					tasks[i]->elapsedTime = 0;
				}
				tasks[i]->elapsedTime += 1;
			}
		}
		
		if (!GetBit(PINA, 0)){
			run = 1;
		}
		
		while(!TimerFlag){};
		TimerFlag = 0;
		
	}
}
