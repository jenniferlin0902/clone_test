/*
 * LARSON SCANNER
 *
 * Currently this code is the blink code copied from lab 1.
 *
 * Modify the code to implement the larson scanner for assignment 1.
 *
 * Make sure to use GPIO pins 20-23 (or 20-27) for your scanner.
 */

.equ DELAY, 0x3F0000
.equ LEDCOUNT, 0x3
.equ LEDBASE, 20

// configure GPIO 20-23 for output
ldr r0, FSEL2
ldr r1, FSEL2VAL
str r1, [r0]

// set base bit
mov r4, #LEDCOUNT 		// use r4 as led counter 
mov r5, #1 				// set direction flag 
mov r6, #0 				// set r6 = 0 for constant 

loop:
	// calculate GPIO pin 
	add r0, r4, #LEDBASE
	mov r3, #1
	lsl r3, r0

setHigh:
	ldr r0, SET0
	str r3, [r0] 

// delay
	mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

	// set GPIO pin low
	ldr r0, CLR0 	
	str r3, [r0] 

// delay
	mov r2, #DELAY
wait2:
    subs r2, #1
    bne wait2

nextLED:
	// move to next LED 
	cmp r4, #0
	beq changeDir  			// change dir if r4 == 0
	cmp r4, #LEDCOUNT
	bne addDir 				// skip change dir if r4 != 0
	#beq changeDir

changeDir:
	sub r5, r6, r5 			// negate direction 

addDir:
	add r4, r4, r5 		// move to next LED toward direction 
	b loop



FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
FSEL2VAL: .word 0x249
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C

