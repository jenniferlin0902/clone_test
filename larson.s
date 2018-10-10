/*
 * LARSON SCANNER: Extension
 * Kyle D'Souza
 * I use 8 LEDs for this extension (specifically GPIO pins 20-27)
 * Citation: Talked about assignment with Juliana Lu, Jonathan (not sure
 * last name), Siddhesh Dhanawade, and Elisa Lupin, all in office hours.
 *
 */

.equ DELAY, 0x100000 	// Delay Constant Advised For Submission

// configure GPIO 20-27 for output
ldr r0, FSEL2
mov r1, #1
orr r1, #(1<<3)
orr r1, #(1<<6)
orr r1, #(1<<9)
orr r1, #(1<<12)
orr r1, #(1<<15)
orr r1, #(1<<18)
orr r1, #(1<<21)

str r1, [r0]

// set bit 18: 5 pins of data
mov r1, #(0b11111<<18) // setting starting position - GPIO 20,21,22
mov r4, #(0b10001<<18) 
mov r5, #(0b01010<<18)

mov r3, #1 // set counter to go forwards

loop: 

// larger loop moves larson display, smaller while loop
// controls brightness, and SET and CLR conditions.
mov r2, #DELAY	 //delay variable.
wait1: 		 //while loop
  subs r2, #1 	 //decrement
  
  // set GPIO values high from i to i+4  - turn on
  ldr r0, SET0 
  str r1, [r0]
  
  // turn off values for i and i+4
  ldr r0, CLR0
  and r6, r1, r4
  str r6, [r0]
  // turn off values for i+1 and i+3 as well
  ldr r0, CLR0
  and r7, r1, r4
  str r7, [r0]
  bne wait1  // checker for while loop

// check if reached last GPIO pin (GPIO27).
// If so, switch to put reverse counter direction.
cmp r1, #(0b11111<<25) 
beq backward
b done
backward: mov r3, #0
done:

// check if on reached first pin (GPIO20).
// If so, switch to put counter forward (reverse direction)
cmp r1, #(0b11111<<18)
beq forward
b done1
forward: mov r3, #1
done1:

// if counter is forward, left shift r1, r4, r5
cmp r3, #1
beq onwards
b done2
onwards: lsl r1, r1, #1
	lsl r4, r4, #1
	lsl r5, r5, #1
done2: 

// if counter is backwards, right shift r1, r4, r5
cmp r3, #0
beq retreat
b done3
retreat: lsr r1, r1, #1
	lsr r4, r4, #1
	lsr r5, r5, #1
done3: 

b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
