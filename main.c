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
const int NUM_INGREDIENTS = 5;
const int NUM_DRINKS = 3;
const char *DRINKS[] = {"GIN & TONIC", "LONDON MULE", "TOM COLLINS"};
const char *INGREDIENTS[] = {"gin", "tonic water", "club soda", "lime juice", "lemon juice"};
const int AMOUNTS[3][5] = {
    {2, 4, 0, 0, 0},  // gin and tonic
    {2, 0, 3, 1, 0},   // london mule
    {2, 0, 5, 0, 1}  // tom collins
};

// other vars
volatile uint16_t accent;
char String[20];
int selectedDrink = 0;
int drinkStrength = 60;
int currScreen = 0;  // 0 = select, 1 = ingredients, 2 = making


void Initialize() {
	lcd_init();

    accent = rgb565(222, 150, 122);

    // set up button 
    DDRD &= ~(1<<DDD2);  // set input pin (PD2)
    PORTD |= (1<<PORTD2);  // internal pull up resistor

    cli();

    // set up joystick (ADC)
    ADMUX |= (1<<REFS0);  // Vref = AVcc
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));  // pin C0
    ADCSRA |= (1<<ADATE);  // autotriggering of ADC
    DIDR0 |= (1<<ADC0D);  // disable digital input buffer on ADC pin
    ADCSRA |= (1<<ADEN);  // enable ADC
    ADCSRA |= (1<<ADSC);

    sei();

    // draw board
    drawBoard();
    selectScreen();
}

void drawBoard() {
    LCD_setScreen(0);
}

void selectScreen() {
    if (currScreen != 0) return;
    // header
    LCD_drawLine((LCD_WIDTH - 20) / 2 - 20, 12, (LCD_WIDTH - 20) / 2 - 5, 12, 0xFFFF);
    LCD_drawString((LCD_WIDTH - 20) / 2, 10, "MENU", 0xFFFF, 0);
    LCD_drawLine((LCD_WIDTH - 20) / 2 + 25, 12, (LCD_WIDTH - 20) / 2 + 40, 12, 0xFFFF);

    // print options
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = (i == selectedDrink) ? accent : 0xFFFF;
        uint8_t y = 35 + i * 20;
        LCD_drawCircle(30, y + 2, 3, circleColor);
        LCD_drawString(40, y, DRINKS[i], 0xFFFF, 0);
    }
}

void redrawSelect() {
    if (currScreen != 0) return;
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = (i == selectedDrink) ? accent : 0xFFFF;
        uint8_t y = 35 + i * 20;
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
            uint8_t y = 15 + numIngredient * 20;
            LCD_drawString(20, y, INGREDIENTS[i], 0xFFFF, 0);
        }
    }

    // make button
    LCD_drawBlock(55, 90, 105, 110, accent);
    LCD_drawString(70, 95, "MAKE", 0xFFFF, accent);

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
            drinkStrength++;
            _delay_ms(50);
            redrawStrength(true);
        } else if (ADC > 700) {  // decrease
            drinkStrength--;
            _delay_ms(50);
            redrawStrength(false);
        } 
    }
}

int main(void) {
	Initialize();
		
    while (1) {
        ADCtoDir();
        if (!(PIND & (1<<PIND2))) {  // button pressed
            if (currScreen == 0) {  // select to drink screen
                currScreen++;
                drawBoard();
                drinkScreen();
                _delay_ms(1000);
            } else if (currScreen == 1) {  // drink screen to make screen
                LCD_drawString(58, 95, "MIXING...", 0xFFFF, accent);
                _delay_ms(1000);
            }
        }
    }
}