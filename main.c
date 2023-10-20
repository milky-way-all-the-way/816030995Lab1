/*
 * 816030995
 *
 * Created: 9/22/2023
 * Author : Deandre Radgman
 */ 


#include "sam.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#define SLOTX 4
#define CYCLEX 5
#define SLOT_T 5000
#include <atmel_start.h>


	
int tps, cycle=0, slot=0;

//struct tms n;
void init_TC3();
void enable_interrupts();
int state =0;
int tick =0;
// Global error flag for TC3
volatile uint8_t TC3_error = 0;
	
void sleepy(int n){
	int now=tick;

	while ((tick-now)<n){
		printf("-");
	}
}

void one() {
	printf("task 1 running\n");
	sleepy(1);
}

void two() {
	printf("task 2 running\n");
	sleepy(2);
}

void three() {
	printf("task 3 running\n");
	sleepy(3);
}

void four() {
	printf("task 4 running\n");
	sleepy(4);
}

void five() {
	printf("task 5 running\n");
	sleepy(5);
	
}



void burn() {
	
	int now=tick;

	while ((tick-now)<4){
		printf(".");
	}
	printf("burn time = %2.2dms\n\n", (tick-now)*1000);
	cycle = CYCLEX;
}

void (*ttable[SLOTX][CYCLEX]) () = {
	{one, two, burn, burn,burn},
	{one, three, burn, burn, burn},
	{one, four, burn, burn, burn},
	{burn, burn, burn, burn, burn}
};
	/* Replace with your application code */
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	SystemInit(); // Initialize the SAM system
	enable_interrupts();
	init_TC3();
	tps = CLOCKS_PER_SEC;
	printf("clock ticks/sec = %d\n\n", tps);
	while (1) {
		for(slot=0; slot<SLOTX; slot++){
			for(cycle=0; cycle<CYCLEX; cycle++) {
				(*ttable[slot][cycle]) ();
			}
		}
	}
		
}

void init_TC3()
{
	
	/* Configure Timer/Counter 3 as a timer to blink LED0 */
	// Configure Clocks
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;
	REG_PM_APBCMASK |= PM_APBCMASK_TC3; // Enable TC3 bus clock
	
	// Configure TC3 (16 bit counter by default)
	REG_TC3_CTRLA |= TC_CTRLA_PRESCALER_DIV8;
	
	// Enable interrupts
	REG_TC3_INTENSET = TC_INTENSET_OVF | TC_INTENSET_ERR;
	
	// Enable TC3
	REG_TC3_CTRLA |= TC_CTRLA_ENABLE;
	while ( TC3->COUNT16.STATUS.bit.SYNCBUSY == 1 ){} // wait for TC3 to be enabled
}


void TC3_Handler()
{
	// Overflow interrupt triggered
	if ( TC3->COUNT16.INTFLAG.bit.OVF == 1 )
	{
		tick=tick+1;
		REG_TC3_INTFLAG = TC_INTFLAG_OVF;
	}
	
	// Error interrupt triggered
	else if ( TC3->COUNT16.INTFLAG.bit.ERR == 1 )
	{
		TC3_error = 1;
		REG_TC3_INTFLAG = TC_INTFLAG_ERR;
	}
}


void enable_interrupts()
{
	NVIC_EnableIRQ( TC3_IRQn );
}
