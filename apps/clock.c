/*
 * Assignment 2: GPIO and Clock
 * Matthew Trost
 * CS107e Spring 2018 staff
 */
#include "../gpio.h"
#include "../timer.h"

#define A 0b00000001 //segment A
#define B 0b00000010 //segment B
#define C 0b00000100 //segment C
#define D 0b00001000
#define E 0b00010000
#define F 0b00100000
#define G 0b01000000
#define DP 0b10000000 //segment decimal point, not called

#define D1 10
#define D2 11
#define D3 12
#define D4 13

static char patterncodes[16]; //each char is 8 bits, LSB is segment A of 7-segment-display, MSB is segment DP.

// Performs any initialization needed for the clock.
void clock_init() {

	for (int pin = 10; pin < 14; pin++) {
		gpio_set_output(pin);
		gpio_write(pin, 0x0);
	}

	for (int pn = 20; pn < 28; pn++) {
		gpio_set_output(pn);
		gpio_write(pn, 0x0);
	}

	  //shows which segments need to be on in order to light a certain pattern.
	  patterncodes[0] = F | E | A | B | C | D; //0b00111111;
	  patterncodes[1] = B | C; 
	  patterncodes[2] = A | B | G | E | D;
	  patterncodes[3] = A | B | G | C | D;
	  patterncodes[4] = F | G | B | C;
	  patterncodes[5] = A | F | G | C | D;
	  patterncodes[6] = A | F | G | C | E | D;
	  patterncodes[7] = A | B | C;
	  patterncodes[8] = A | B | C | D | E | F | G;
	  patterncodes[9] = A | B | F | G | C | D;
	  patterncodes[0xA] = F | A | B | G | E | C;
	  patterncodes[0xb] = F | G |  C | D | E; //lowercase to distinguish it from 8
	  patterncodes[0xC] = A | F | E | D;
	  patterncodes[0xd] = B | G | E | C | D; //lowercase to distinguish it from 0
	  patterncodes[0xE] = A | F | G | E | D;
	  patterncodes[0xF] = A | F | G | E;

	  //set GPIO 2 and 3 as inputs (for buttons)
	  gpio_set_function(2, 0x0); 
	  gpio_set_function(3, 0x0); 
}


void display_digit(char patterncode, unsigned int digit_pin) {
	unsigned int curr_segment_pin = 20;
	unsigned int mask = 0b1;
	while (patterncode > 0) {
		if ((patterncode & mask) == 1) {
			gpio_write(curr_segment_pin, 0x1);
		}
		curr_segment_pin++;
		patterncode = patterncode>>1;
	}
	gpio_write(digit_pin, 0x1);
}

void clear_digit(unsigned int digit_pin) {
	for (int pin = 20; pin < 28; pin++) {
		gpio_write(pin, 0x0);
	}
	gpio_write(digit_pin, 0x0);
}


void display_refresh(char digit1, char digit2, char digit3, char digit4) {
	display_digit(digit1, D1);
	timer_delay_us(2500);
	clear_digit(D1);

	display_digit(digit2, D2);
	timer_delay_us(2500);
	clear_digit(D2);

	display_digit(digit3, D3);
	timer_delay_us(2500);
	clear_digit(D3);

	display_digit(digit4, D4);
	timer_delay_us(2500);
	clear_digit(D4);
}


void display_time(unsigned int num_seconds_elapsed) {
	unsigned int sec_ones = (num_seconds_elapsed % 60) % 10;
	unsigned int sec_tens = (num_seconds_elapsed % 60) / 10; 
	unsigned int min_ones = (num_seconds_elapsed / 60) % 10;
	unsigned int min_tens = (num_seconds_elapsed / 60) / 10;

	display_refresh(patterncodes[min_tens], patterncodes[min_ones], patterncodes[sec_tens], patterncodes[sec_ones]);
}

// pressing both at the same time resets time to 0, enters reset mode
// pressing left will increment minutes by 1
// pressing right will increment seconds by 1
// pressing both will while in reset mode will reset to 0 again and remain in reset mode.
// will automatically switch back to counting time (out of reset mode) after 4 seconds of inactivity (no button presses)
unsigned int reset_time() {
	timer_delay_ms(120);
	unsigned int num_seconds_elapsed = 0;
	unsigned int last_system_tick_count = timer_get_ticks();
	unsigned int INACTIVITY_TRIGGER = 4000000; // 5 seconds of inactivity
	while (1) {
		if (gpio_read(2)==0x0) {
			if (gpio_read(3)==0x0) {
				//if both pressed, reset to zero again.
				last_system_tick_count = timer_get_ticks();
				num_seconds_elapsed = 0;
				timer_delay_ms(120);
			} else {
				last_system_tick_count = timer_get_ticks();
				// add a minute
				num_seconds_elapsed += 60;
				timer_delay_ms(120);
			}
		} else if (gpio_read(3)==0x0) {
			last_system_tick_count = timer_get_ticks();
			// add a second
			num_seconds_elapsed += 1;
			timer_delay_ms(120);
		}

		if (timer_get_ticks() - last_system_tick_count >= INACTIVITY_TRIGGER) {
			return num_seconds_elapsed;
		}
		display_time(num_seconds_elapsed); // continue to display the time, except it doesn't change during this period.
	}
}


void clock_run() {
	unsigned int num_seconds_elapsed = 0;
	unsigned int TICKS_PER_SECOND = 0xF4240; //0xF4240 = hex value of 1,000,000us = 1 second;
	unsigned int MAX_DISPLAYABLE_SECONDS = 5999;

	unsigned int difference = 0x0;
	unsigned int last_system_tick_count = timer_get_ticks();
	while(1) {

		if (gpio_read(2) == 0x0 && gpio_read(3) == 0x0) {
			num_seconds_elapsed = reset_time();
			timer_delay_ms(200);
		}

		difference = timer_get_ticks() - last_system_tick_count;
	    if(difference >= TICKS_PER_SECOND) {
	      last_system_tick_count = timer_get_ticks();
	      difference = difference - TICKS_PER_SECOND;
	      num_seconds_elapsed = num_seconds_elapsed + 1;
	    }

		display_time(num_seconds_elapsed);

		if (num_seconds_elapsed == MAX_DISPLAYABLE_SECONDS) {
			num_seconds_elapsed = 0;
		}
	}
}


void main(void)
{
	clock_init();
   	display_refresh(patterncodes[0], patterncodes[0], patterncodes[0], patterncodes[0]);
   	clock_run();
}