#include <avr/io.h>
#include "timer.h"
#include <time.h>
#include <util/delay.h>

#define SRCLR (0x08)
#define SRCLK (0x02)
#define RCLK (0x04)
#define SER (0x01)

unsigned char row = 0;
unsigned char col = 0;

//global vars
static unsigned char column_valAnode = 0x01; // sets the pattern displayed on columns
static unsigned char column_selR = 0xFF; // grounds column to display pattern
static unsigned char column_selB = 0xFF;
static unsigned char column_selG = 0xFF;

void transmit_data(unsigned char data1, unsigned char data2,
unsigned char data3, unsigned char data4) {
	int i;
	for(i = 0; i < 8; i++) {
		PORTC = SRCLR;
		PORTC |= ((data1 >> i) & SER);
		PORTC |= SRCLK;
	}
	for(i = 0; i < 8; i++) {
		PORTC = SRCLR;
		PORTC |= ((data2 >> i) & SER);
		PORTC |= SRCLK;
	}
	for(i = 7; i >= 0; i--) {
		PORTC = SRCLR;
		PORTC |= ((data3 >> i) & SER);
		PORTC |= SRCLK;
	}
	for(i = 7; i >= 0; i--) {
		PORTC = SRCLR;
		PORTC |= ((data4 >> i) & SER);
		PORTC |= SRCLK;
	}
	PORTC |= RCLK;
	PORTC = 0x00;
}

/*
unsigned char Cols_Matrix{0};
cols = [1][1];
cols = [1][1];
*/

/// this file has other functions(defines all the ports)
// ====================
// SM1: DEMO LED matrix
// ====================
enum SM1_States {sm1_display} state;
void SM1_Tick(state) {
	
	// === Transitions ===
	switch (state) {
		case sm1_display:    break;
		default:   	        state = sm1_display;
		break;
	}
	
	// === Actions ===
	switch (state) {
		case sm1_display:   // If illuminated LED in bottom right corner
		if (column_selR == 0xFE && column_valAnode == 0x80) {
			column_selR = 0x7F; // display far left column
			column_valAnode = 0x01; // pattern illuminates top row
		}
		// else if far right column was last to display (grounded)
		else if (column_selR == 0xFE) {
			column_selR = 0x7F; // resets display column to far left column
			column_valAnode = column_valAnode << 1; // shift down illuminated LED one row
		}
		// else Shift displayed column one to the right
		else {
			column_selR = (column_selR >> 1) | 0x80;
		}
		break;
		default:   	        break;
	}
	
	transmit_data(column_valAnode,column_selR, column_selB, column_selG); // PORTB selects column to display pattern
	// anode, red, blue, green
	// the column_val is passed in, into the function, then u want to test values like FF
	// column_valAnode controls the row and the color controls the column
};

// 2-D ARRAY
//Initializations
//unsigned char Matrix_LED();
/*
unsigned char MatrixArray [2][8]= {{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}, {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFF}};

void Matrix_LED(unsigned char i, unsigned char j)
{
	for( i = 0; i < 8; i++){ //controls columns
		for(j = 0; j < 1; ++j){ // control rows
			column_valAnode = MatrixArray[0][i];
			column_selR = MatrixArray[1] [j];
transmit_data(column_valAnode,column_selR, column_selB, column_selG);
			_delay_ms(2000);

		}
	}
}
*/
																						//64 //
unsigned char MatrixArray[2][2][8] = {{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}, {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFF},{}};

void Matrix_LED(unsigned char i, unsigned char j)

for(int i = 0; i < 2; ++i)
{
	for (int j = 0; j < 8; ++j)
	{
		column_valAnode = MatrixArray[i][1][j];
	}
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(1000);
	TimerOn();
	/* Replace with your application code */
	while (1)
	{
		Matrix_LED(row, col);
		SM1_Tick();
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}