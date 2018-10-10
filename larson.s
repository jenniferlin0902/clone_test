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
//lsl r1, r1, #3
str r1, [r0]
//mov r1, #(0b1000)

// set bit 20
// mov r1, #(0b10<<20) 
mov r1, #(0b11111<<18)
//mov r4, #(0b10001<<18)
//mov r5, #(0b01010<<18)
//mov r4, #(1<<19)
//mov r5, #(1<<20)
//mov r6, #(1<<21)
//mov r7, #(1<<22)

// set r3 to 1 = forward
mov r3, #1

loop: 

// set GPIO 20 high
ldr r0, SET0
str r1, [r0] 

// delay 1 and 5
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// set GPIO 18 and GPIO 23 low - turn off 1 and 5
ldr r0, CLR0
//and r1, r1, r4
str r1, [r0] 

// delay 2 and 4
//mov r2, #DELAY
//wait2:
  //  subs r2, #1
   // bne wait2

// set GPIO 19 and 22 low - turn off 2 and 4
//ldr r0, CLR0
//and r1, r1, r5
//str r1, [r0]

// delay
//mov r2, #DELAY
//wait3:
  //  subs r2, #1
   // bne wait3

// check if on last GPIO pin (GPIO27), if so, switch to put counter backward.
cmp r1, #(0b11111<<25) 
beq backward
b done
backward: mov r3, #0
done:

// check if on first pin (GPIO20), if so, switch to put counter forward
cmp r1, #(0b11111<<18)
beq forward
b done1
forward: mov r3, #1
done1:

// if counter is forward, left shift r1.
cmp r3, #1
beq onwards
b done2
onwards: lsl r1, r1, #1
//	lsl r4, r4, #1
//	lsl r5, r5, #1
done2: 

// if counter is backwards, right shift r1.
cmp r3, #0
beq retreat
b done3
retreat: lsr r1, r1, #1
//	lsr r4, r4, #1
//	lsr r5, r5, #1
done3: 


//backward:  lsr r1, r1, #1 // backward
//mov r3, #0
//done:

b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
