/*
 * ESE 350 final project
 * PRIME Hydration
 * Karen Li and Quinn Liu
 */ 

#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initialize() {
	cli();

	DDRD |= (1 << DDD5);
	
	TCCR0A |= (1 << WGM00);
	TCCR0A &= ~(1 << WGM01);
	TCCR0B |= (1 << WGM02);  // timer 0 phase correct PWM mode (prescaler 1)
	TCCR0B |= (1 << CS00);  // no prescaler
	TCCR0A &= ~(1 << COM0B0);
	TCCR0A |= (1 << COM0B1);
    OCR0A = 255;	
	OCR0B = 255;

	sei();
}

int main(void) {
	initialize();
	while (1) {
	}
	return 0;
}

// void initialize() {
// 	cli();
	
// 	DDRD |= (1 << DDD5); // setting PD5 to be motor 1 output pin
	
// 	// configuring Timer0 (prescale by 64)
// 	TCCR0B &= ~(1 << CS02);
// 	TCCR0B |= (1 << CS01);
// 	TCCR0B |= (1 << CS00);
	
// 	// select phase correct PWM mode
// 	TCCR0A |= (1 << WGM00);
// 	TCCR0A &= ~(1 << WGM01);
// 	TCCR0B |= (1 << WGM02);
	
// 	// Enable PWM output on OC0B pin (PD5's pwm)
// 	TCCR0A &= ~(1 << COM0B0);
// 	TCCR0A |= (1 << COM0B1);

// 	sei();
// }

// int main(void) {
// 	initialize();
// 	UART_init(BAUD_PRESCALER);
// 	while (1) {


//         OCR0B = OCR0A * 0.05;
// 		OCR0A = 60;

// 		// sprintf(String, "OCR0A: %d, OCR0B: %d \n");
// 		// UART_putstring(String);
// 	}
// 	return 0;
// }