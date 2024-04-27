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
#include <stdbool.h>
#include <time.h>

#include "lib\ST7735.h"
#include "lib\LCD_GFX.h"

// menu
const int NUM_INGREDIENTS = 6;
const int NUM_DRINKS = 5;
char *DRINKS[] = {"VODKA CRAN", "COSMOPOLITAN", "CITRUS VODKA SPRITZ", "CRAN LIME COOLER", "SHOT"};
char *INGREDIENTS[] = {"vodka", "lime juice", "simple syrup", "triple sec", "cran juice", "club soda"};
int AMOUNTS[5][6] = {  // 1 unit = 0.5 ounces
    {4, 0, 4, 0, 0, 8},  // vodka cran
    {3, 3, 2, 2, 0, 0},   // cosmo
    {3, 2, 0, 2, 0, 6},  // citrus vodka spritz
    {3, 2, 0, 0, 4, 0},  // cran lime
    {3, 0, 0, 0, 0, 0}  // shot
};

// vodka: 3 = shot (1.5 oz)
// lime juice
// cran
// triple
// syrup
// club soda: 6 = 6 oz

// other vars
volatile uint16_t accent;
int overflow = 0;
char String[20];
int selectedDrink = 0;
int drinkStrength = 70;
int currScreen = 0;  // 0 = select, 1 = ingredients, 2 = making


void Initialize() {
	lcd_init();

    accent = rgb565(114, 165, 247);

    cli();

    // set up button (drinks)
    DDRC &= ~(1<<DDC2);  // set input pin (PD2)
    PORTC |= (1<<PORTC2);  // internal pull up resistor
	
	// set up button (mixer)
	DDRB &= ~(1<<DDB4); // set input pin (PB4)
	PORTB |= (1<<PORTB4);  // internal pull up resistor

    // set up timer
	TCCR1B |= (1 << CS10);  // set up timer (no prescaling)
	TIMSK1 |= (1<<TOIE1);  // enable interrupt for timer overflow

    // set up joystick (ADC)
    ADMUX |= (1<<REFS0);  // Vref = AVcc
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));  // pin C0
    ADCSRA |= (1<<ADATE);  // autotriggering of ADC
    DIDR0 |= (1<<ADC0D);  // disable digital input buffer on ADC pin
    ADCSRA |= (1<<ADEN);  // enable ADC
    ADCSRA |= (1<<ADSC);

    // set up mixing motor PWM (PD5)
    // DDRD |= (1 << DDD5);
	TCCR0A |= (1 << WGM00);
	TCCR0A &= ~(1 << WGM01);
	TCCR0B |= (1 << WGM02);  // timer 0 phase correct PWM mode (prescaler 1)
	TCCR0B |= (1 << CS00);  // no prescaler
	TCCR0A &= ~(1 << COM0B0);
	TCCR0A |= (1 << COM0B1);
    OCR0A = 255;	
	OCR0B = 100;

    // set up pumps (PD0, PD1, PD3, PD4, PC5, PC4)
    DDRD |= ((1<<DDD0) | (1<<DDD1) | (1<<DDD3) | (1<<DDD4));
    DDRC |= ((1<<DDC5) | (1<<DDC4));

    sei();

    // draw board
    LCD_setScreen(0);
    menuScreen(false);
}

ISR(TIMER1_OVF_vect) {  // overflow interrupt
	overflow++;
}

void menuScreen(bool clear) {
    uint16_t textColor = clear ? 0 : 0xFFFF;
    if (currScreen != 0) return;
    // header
    LCD_drawLine((LCD_WIDTH - 20) / 2 - 20, 12, (LCD_WIDTH - 20) / 2 - 5, 12, textColor);
    LCD_drawString((LCD_WIDTH - 20) / 2, 10, "MENU", textColor, 0);
    LCD_drawLine((LCD_WIDTH - 20) / 2 + 25, 12, (LCD_WIDTH - 20) / 2 + 40, 12, textColor);

    // print options
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = clear ? 0 : ((i == selectedDrink) ? accent : textColor);
        uint8_t y = 28 + i * 20;
        LCD_drawCircle(30, y + 2, 3, circleColor);
        LCD_drawString(40, y, DRINKS[i], textColor, 0);
    }
}

void redrawSelect() {
    if (currScreen != 0) return;
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = (i == selectedDrink) ? accent : 0xFFFF;
        uint8_t y = 28 + i * 20;
        LCD_drawCircle(30, y + 2, 3, circleColor);
    }
}

void drinkScreen() {
    if (currScreen != 1) return;
    // header
    int drinkLen = strlen(DRINKS[selectedDrink]) * 5;
    LCD_drawLine((LCD_WIDTH - drinkLen) / 2 - 20, 12, (LCD_WIDTH - drinkLen) / 2 - 5, 12, 0xFFFF);
    LCD_drawString((LCD_WIDTH - drinkLen) / 2, 10, DRINKS[selectedDrink], 0xFFFF, 0);
    LCD_drawLine((LCD_WIDTH - drinkLen) / 2 + drinkLen + 5, 12, (LCD_WIDTH - drinkLen) / 2 + drinkLen + 15, 12, 0xFFFF);
    
    // ingredients
    int numIngredient = 0;
    for (int i = 0; i < NUM_INGREDIENTS; i++) {
        if (AMOUNTS[selectedDrink][i] > 0) {
            numIngredient++;
            uint8_t y = 12 + numIngredient * 15;
            LCD_drawString(20, y, INGREDIENTS[i], 0xFFFF, 0);
        }
    }

    // make button
    LCD_drawBlock(55, 100, 105, 120, accent);
    LCD_drawString(70, 105, "MAKE", 0xFFFF, accent);

    // strength bar
    LCD_drawString(100, 30, "STRENGTH", 0xFFFF, 0);
    LCD_drawBlock(90, 40, 150, 50, 0xFFFF);
    LCD_drawBlock(90, 40, 90 + 60 * drinkStrength / 100, 50, accent);
}

void redrawStrength(bool increase) {
    if (increase) {
        LCD_drawBlock(90, 40, 90 + 60 * drinkStrength / 100, 50, accent);
    } else {
        LCD_drawBlock(90 + 60 * drinkStrength / 100, 40, 150, 50, 0xFFFF);
    }
}

void ADCtoDir() {
    if (currScreen == 0) {  // choose drink
        ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));  // pin C0
        if (ADC < 300) {
            selectedDrink++;
            _delay_ms(100);
        } else if (ADC > 700) {
            selectedDrink--;
            _delay_ms(100);
        } 
        if (selectedDrink < 0) {
            selectedDrink = NUM_DRINKS - 1;
        } 
        selectedDrink %= NUM_DRINKS;
        redrawSelect();
    } else if (currScreen == 1) {  // adjust strength
        ADMUX |= (1<<MUX0);  // pin C1
        if (ADC < 300) {  // increase
            drinkStrength += 2;
            drinkStrength = (drinkStrength > 100) ? 100 : drinkStrength;
            _delay_ms(50);
            redrawStrength(true);
        } else if (ADC > 700) {  // decrease
            drinkStrength -= 2;
            drinkStrength = (drinkStrength < 0) ? 0 : drinkStrength;
            _delay_ms(50);
            redrawStrength(false);
        } 
    }
}

void dispenseDrink() {
    if (currScreen != 1) return;
    for (int i = 0; i < NUM_INGREDIENTS; i++) {
		int time = (int) (AMOUNTS[selectedDrink][i] * 30);
        if (i == 0) time = (int) (time * drinkStrength / 100);
		if (i == 1) time = (int) (AMOUNTS[selectedDrink][i] * 20);
		if (i == 2) time = (int) (AMOUNTS[selectedDrink][i] * 30);
		if (i == 3) time = (int) (AMOUNTS[selectedDrink][i] * 40);
		if (i == 4) time = (int) (AMOUNTS[selectedDrink][i] * 30);
		if (i == 5) time = (int) (AMOUNTS[selectedDrink][i] * 30); 

		if (i == 0) PORTC |= (1 << PORTC4);
		if (i == 1) PORTD |= (1 << PORTD0);
		if (i == 4) PORTD |= (1 << PORTD1);
		if (i == 3) PORTD |= (1 << PORTD3);
		if (i == 2) PORTD |= (1 << PORTD4);
		if (i == 5) PORTC |= (1 << PORTC5);
		for (int i = 0; i < time; i++) {
			while (overflow < 6);
			overflow = 0;
		}
		if (i == 0) PORTC &= ~(1 << PORTC4);
		if (i == 1) PORTD &= ~(1 << PORTD0);
		if (i == 4) PORTD &= ~(1 << PORTD1);
		if (i == 3) PORTD &= ~(1 << PORTD3);
		if (i == 2) PORTD &= ~(1 << PORTD4);
		if (i == 5) PORTC &= ~(1 << PORTC5);
    }
}

int main(void) {
	Initialize();
		
    while (1) {
        ADCtoDir();
        if (!(PINC & (1<<PINC2))) {  // button pressed
            if (currScreen == 0) {  // select to drink screen
                menuScreen(true);
                currScreen++;
                drinkScreen();
                _delay_ms(200);
            } else if (currScreen == 1) {  // drink screen to make screen
                LCD_drawString(58, 105, "MIXING...", 0xFFFF, accent);
                dispenseDrink();
                _delay_ms(200);
                LCD_setScreen(0);
                currScreen = 0;  // menu screen
                menuScreen(false);
            }
        }
		if (!(PINB & (1<<PINB4))) { // enable mixer when button 2 is pressed
			DDRD |= (1 << DDD5);
		} else {
			DDRD &= ~(1 << DDD5);
		}
    }
}