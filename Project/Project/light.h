#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vectors.h"
#include "object3d.h"

// ====================================================================
// ====================================================================

class Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Light() {}
  virtual ~Light() {}

  // VIRTUAL METHODS
  virtual void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col, 
				float &distanceToLight) const = 0;
  virtual void glInit(int id) = 0;

};

// ====================================================================
// ====================================================================

class DirectionalLight : public Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  DirectionalLight() { 
    direction = Vec3f(0,0,0);
    color = Vec3f(1,1,1); }
  DirectionalLight(const Vec3f &d, const Vec3f &c) {
    direction = d; direction.Normalize();
    color = c; }
  ~DirectionalLight() {}

  // VIRTUAL METHODS
  void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col, 
			float &distanceToLight) const {
    // the direction to the light is the opposite of the
    // direction of the directional light source
    dir = direction * (-1.0f);
    col = color;
    // the distance is INFINITY
    distanceToLight = INFINITY; 
  }

  void glInit(int id);

private:

  // REPRESENTATION
  Vec3f direction;
  Vec3f color;

};

// ====================================================================
// ====================================================================


#endif
