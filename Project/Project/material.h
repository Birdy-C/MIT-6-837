#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include <iostream>
#include "vectors.h"
#include "ray.h"
#include "hit.h"

// You will extend this class in later assignments

class Material {

protected:

  // REPRESENTATION
  Vec3f diffuseColor;

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }

  //It takes as input the viewing ray, the Hit data structure, and light information and returns the color for that pixel. 
  // dir may not be normaized for information of direction
  virtual Vec3f Material::Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;
  
  virtual void glSetMaterial(void) const = 0;

  
};


class PhongMaterial :
	public Material
{
	Vec3f specularColor;
	float exponent;
public:
	PhongMaterial(const Vec3f &diffuseColor,const Vec3f &specularColor, float texponent);
	Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const;
	void glSetMaterial(void) const;
	Vec3f getSpecularColor() const { return specularColor; }
};

#endif
