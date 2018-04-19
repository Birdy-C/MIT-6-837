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

  DirectionalLight(); //don't use

  // REPRESENTATION
  Vec3f direction;
  Vec3f color;

};

// ====================================================================
// ====================================================================

class PointLight : public Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  PointLight(const Vec3f &p, const Vec3f &c, float a1, float a2, float a3) {
    position = p; 
    color = c; 
    attenuation_1 = a1;
    attenuation_2 = a2;
    attenuation_3 = a3; }
  ~PointLight() {}

  // VIRTUAL METHODS
  void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col, float &distanceToLight) const {
    dir = position - p;
    // grab the length before the direction is normalized
    distanceToLight = dir.Length(); 
    dir.Normalize(); 
    float attenuation = 1 / (attenuation_1 + 
			     attenuation_2*distanceToLight + 
			     attenuation_3*distanceToLight*distanceToLight);
    if (attenuation < 0) attenuation = 0;
    col = color * attenuation;
  }

  void glInit(int id);

private:

  PointLight(); // don't use

  // REPRESENTATION
  Vec3f position;
  Vec3f color;

  // attenuation of the light
  float attenuation_1;
  float attenuation_2;
  float attenuation_3;

};

// ====================================================================
// ====================================================================


#endif
