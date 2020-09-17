#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "vectors.h"

// ==================================================================
// A helper class for super-sampling.  The position specifies the 2D
// offset within the pixel from (0,0) -> (1,1).

class Sample {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Sample () { 
    position = Vec2f(0.5, 0.5);
    color = Vec3f(0,0,0); }
  ~Sample () {}

  // ACCESSORS
  Vec2f getPosition() { return position; }
  Vec3f getColor() { return color; }

  // MODIFIERS
  void set(Vec2f p, Vec3f c) { 
    assert (p.x() >= 0 && p.x() <= 1);
    assert (p.y() >= 0 && p.y() <= 1);
    position = p;
    color = c; }

private:

  // REPRESENTATION
  Vec2f position;
  Vec3f color;

};

// ==================================================================

#endif
