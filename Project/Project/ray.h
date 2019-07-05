#ifndef _RAY_H
#define _RAY_H

#include <iostream>
using namespace std;

#include "vectors.h"

// ====================================================================
// ====================================================================

// Ray class mostly copied from Peter Shirley and Keith Morley

class Ray {
	// direction may not be normalized
public:

  // CONSTRUCTOR & DESTRUCTOR
  Ray () {}
  Ray (const Vec3f &orig, const Vec3f &dir) {
    origin = orig; 
    direction = dir; }
  Ray (const Ray& r) {*this=r;}

  // this function is used for calculate the 
  float caculatemin(int type, float value) const {
	  switch (type) {
	  case 0:
		  if (direction.x() == 0) return value;
		  return (value - origin.x()) / direction.x();
	  case 1:
		  if (direction.y() == 0) return value;
		  return (value - origin.y()) / direction.y();
	  case 2:
		  if (direction.z() == 0) return value;
		  return (value - origin.z()) / direction.z();
	  } 
  }

  // ACCESSORS
  const Vec3f& getOrigin() const { return origin; }
  const Vec3f& getDirection() const { return direction; }
  Vec3f pointAtParameter(float t) const {
	  return origin + direction * t;
  }

private:

  // REPRESENTATION
  Vec3f origin;
  Vec3f direction;
};

inline ostream &operator<<(ostream &os, const Ray &r) {
  os << "Ray <o:" <<r.getOrigin()<<", d:"<<r.getDirection()<<">";
  return os;
}

// ====================================================================
// ====================================================================

#endif
