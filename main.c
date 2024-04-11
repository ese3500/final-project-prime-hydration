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

volatile uint16_t accent;
char String[20];
int xdir = 0;
int ydir = 0;
const int NUM_INGREDIENTS = 5;
const int NUM_DRINKS = 3;
const char *DRINKS[] = {"GIN & TONIC", "LONDON MULE", "TOM COLLINS"};
const char *INGREDIENTS[] = {"gin", "tonic water", "club soda", "lime juice", "lemon juice"};
const int AMOUNTS[3][5] = {
    {2, 4, 0, 0, 0},  // gin and tonic
    {2, 0, 3, 1, 0},   // london mule
    {2, 0, 5, 0, 1}  // tom collins
};
int selectedDrink = 0;
int drinkStrength = 60;


void Initialize() {
	lcd_init();

    accent = rgb565(147, 186, 219);

    cli();

    // set up joystick (ADC)
    ADMUX |= (1<<REFS0);  // Vref = AVcc
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));  // pin C0
    // ADMUX |= (1<<MUX1);  // pin C1
    ADCSRA |= (1<<ADATE);  // autotriggering of ADC
    DIDR0 |= (1<<ADC0D);  // disable digital input buffer on ADC pin
    ADCSRA |= (1<<ADEN);  // enable ADC
    ADCSRA |= (1<<ADSC);

    sei();

    // draw board
    drawBoard();
    selectScreen();
    // drinkScreen();
}

void drawBoard() {
    LCD_setScreen(0);
}

void selectScreen() {
    // header
    LCD_drawLine((LCD_WIDTH - 20) / 2 - 20, 12, (LCD_WIDTH - 20) / 2 - 5, 12, 0xFFFF);
    LCD_drawString((LCD_WIDTH - 20) / 2, 10, "MENU", 0xFFFF, 0);
    LCD_drawLine((LCD_WIDTH - 20) / 2 + 25, 12, (LCD_WIDTH - 20) / 2 + 40, 12, 0xFFFF);

    // print options
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = (i == selectedDrink) ? accent : 0xFFFF;
        uint8_t y = 35 + i * 20;
        LCD_drawCircle(10, y + 2, 3, circleColor);
        LCD_drawString(20, y, DRINKS[i], 0xFFFF, 0);
    }

    // strength bar
    LCD_drawString(100, 50, "STRENGTH", 0xFFFF, 0);
    LCD_drawBlock(90, 60, 150, 70, 0xFFFF);
    LCD_drawBlock(90, 60, 90 + 60 * drinkStrength / 100, 70, accent);
}

void redrawSelect() {
    for (int i = 0; i < sizeof(DRINKS) / sizeof(DRINKS[0]); i++) {
        uint16_t circleColor = (i == selectedDrink) ? accent : 0xFFFF;
        uint8_t y = 35 + i * 20;
        LCD_drawCircle(10, y + 2, 3, circleColor);
    }
}

void drinkScreen() {
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
            uint8_t y = 20 + numIngredient * 20;
            LCD_drawString(40, y, INGREDIENTS[i], 0xFFFF, 0);
        }
    }

    // make button
    LCD_drawBlock(60, 90, 100, 110, accent);
    LCD_drawString(70, 95, "MAKE", 0xFFFF, accent);
}

void ADCtoDir() {
    // ADMUX &= ~(1<<MUX1); 
    if (ADC < 300) {
        selectedDrink++;
        redrawSelect();
    } else if (ADC > 700) {
        selectedDrink--;
        redrawSelect();
    } 
    selectedDrink %= NUM_DRINKS;
    // ADMUX |= (1<<MUX1); 
    // if (ADC < 400) {
    //     ydir = -1;
    // } else if (ADC > 600) {
    //     ydir = 1;
    // } else {
    //     ydir = 0;
    // }
}

int main(void) {
	Initialize();
		
    while (1) {
        ADCtoDir();
        // sprintf(String, "xdir %i", xdir);
        // LCD_drawString(60, 2, String, 0xFFFF, 0);
        // sprintf(String, "ydir %i", ydir);
        // LCD_drawString(60, 40, String, 0xFFFF, 0);
    }
}