#ifndef _RANDOM_H_
#define _RANDOM_H_

// ==========================================================
// ==========================================================

// This class can be used instead of drand48 when you want 
// multiple identical streams of random floating point numbers.

// To do this, create multiple instances and seed them with 
// the same number.


// ==========================================================
// A linear congruential generator
//
// It is fast, simple, and (if instantiated with the right
// constants) gives reasonable pseudorandom numbers. The nth random
// number is a function of the (n-1)st random number:
//
// R_n = (a*R_n-1 + c) mod m
// a = 1366
// c = 150889
// m = 714025
//

#include "vectors.h"

class Random {

public:

  Random(int seed = 0) { last = seed; }
  ~Random() {}

  // returns a random floating point between 0 & 1
  float next() {
    last = (1366*last + 150889) % 714025;
    float answer = last / float (714025);
    return answer; }

  // return a random vector with each component from -1 -> 1
  Vec3f randomVector() {
    float x = next()*2 - 1;
    float y = next()*2 - 1;
    float z = next()*2 - 1;
    return Vec3f(x,y,z);
  }

private:

  int last;
};

// ==========================================================

#endif
