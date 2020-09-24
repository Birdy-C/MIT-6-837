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

    // The following function should be pure virtual, but cause assignment1 need to instance Material class.
    // assignment3
    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const { assert(0); return Vec3f(0, 0, 0); };
	virtual void glSetMaterial(void) const { assert(0); };
	virtual Vec3f getSpecularColor() const { assert(0); return Vec3f(0, 0, 0); };
	virtual Vec3f getRedlectiveColor() const { assert(0); return Vec3f(0, 0, 0); };
	virtual Vec3f getTransparentColor() const { assert(0); return Vec3f(0, 0, 0); };
	virtual float getIndexOfRef()const { assert(0); return 0.f; };
};


class PhongMaterial : public Material
{
	Vec3f specularColor, reflectiveColor, transparentColor;
	float exponent, indexOfRefraction;
public:
	PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float texponent);
	PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent,
		const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction);
	Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	void glSetMaterial(void) const;
	Vec3f getSpecularColor() const { return specularColor; }
	Vec3f getRedlectiveColor() const { return reflectiveColor; }
	Vec3f getTransparentColor() const { return transparentColor; }
	float getIndexOfRef()const { return indexOfRefraction; }
};

class Checkerboard : public Material
{
    Matrix *mat;
    Material *material1, *material2;
public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2) : Material(Vec3f(1)), mat(m), material1(mat1), material2(mat2) {};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getRedlectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRef(); }
};

class Noise : public Material
{
    int octaves;
    Matrix *mat;
    Material *material1, *material2;
public:
    static float NoiseCalculate(Vec3f pos, int octaves);
    Noise(Matrix *m, Material *mat1, Material *mat2, int _octaves) : Material(Vec3f(1)), mat(m), material1(mat1), material2(mat2), octaves(_octaves){};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getRedlectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRef(); }
};

class Marble : public Material
{
    int octaves;
    Matrix *mat;
    Material *material1, *material2;
    float frequency;
    float amplitude;
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int _octaves, float _frequency, float _amplitude) 
        : Material(Vec3f(1)), mat(m), material1(mat1), material2(mat2), octaves(_octaves), frequency(_frequency), amplitude(_amplitude) {};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getRedlectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRef(); }
};


class Wood : public Material
{
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Material(Vec3f(1)) {};
};

#endif
