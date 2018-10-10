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
//mov r1, #(0b10<<20) 
mov r1, #(1<<20)

// set r3 to 1 = forward
//mov r3, #1

loop: 

// set GPIO 20 high
ldr r0, SET0
str r1, [r0] 

// delay
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// set GPIO 20 low
ldr r0, CLR0
str r1, [r0] 

// delay
mov r2, #DELAY
wait2:
    subs r2, #1
    bne wait2

cmp r1, #(1<<27)
beq backward
b done
backward: mov r3, #0
done:

cmp r1, #(1<<20)
beq forward
b done1
forward: mov r3, #1
done1:

cmp r3, #1
beq onwards
b done2
onwards: lsl r1, r1, #1
done2: 

cmp r3, #0
beq retreat
b done3
retreat: lsr r1, r1, #1
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
