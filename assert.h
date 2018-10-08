#ifndef ASSERT_H
#define ASSERT_H

// abort runs an infinite loop that flashes the red power LED on the
// Pi board.
extern void abort();

// You call assert on an expression that you expect to be true.
// If expr evaluates to false, assert calls abort to stop your program
// and flash the red light of doom.
#define assert(expr) if(!(expr)) abort()

#endif
