/*
		Using the ATtiny13 microcontroller to control
		a stepper motor using the 
		Stepper Motor Driver v1.1 from RRRF
		
		Samuel Johnson
		19/07/2014
		
		F_CPU = 4.8Mhz
		-U lfuse:w:0x79:m -U hfuse:w:0xff:m
*/
#define CPU_CLOCK 4800000UL 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DIR     PB3
#define STEP	PB4
#define STEP_BUTTON PB0
#define DIR_BUTTON  PB1

volatile int8_t can_step = 0;

// Reduces flash cost of _delay_ms() at compile time, due to passing it a variable
// instead of a constant
// http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=105891&start=20
void delay_ms(int32_t count) {
		while(count--) {
				_delay_ms(1);
		}
}

ISR(TIM0_OVF_vect) {
		if(can_step) {
				PORTB ^= (1<<STEP); // Toggle STEP pin
		} else {
				PORTB &= ~(1<<STEP);
		}
}

int main(void) {
		cli();
		DDRB = 0;
		DDRB |= (1<<DIR) | (1<<STEP); // Two outputs
		PORTB = 0; // All outputs low 
		
		// Setup for 8 bit timer
		TCCR0B |= (1<<CS01) | (1<<CS00); // 64 Prescaler
		TIMSK0 |= (1<<TOIE0); // Enable overflow interrupt		
							// For Timer0 Overflow, flag is TIM0_OVF
							
		sei();
		
		// Main loop
		while(1) {
				// Check buttons for step and direction input
				if(PINB & (1<<STEP_BUTTON)) {
						can_step = 1;
				} else {
						can_step = 0;
				}
				
				if(PINB & (1<<DIR_BUTTON)) {
						PORTB |= (1<<DIR);
				} else {
						PORTB &= ~(1<<DIR);
				}
				
		}
		
		return 0;
}
