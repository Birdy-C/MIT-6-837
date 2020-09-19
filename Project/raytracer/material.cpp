// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight 
// artifact for small specular exponents (wide specular lobe)
// ====================================================================
#include "stdafx.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <algorithm>
#include <cmath>
#include "material.h"
#include "info.h"
#include <gl/glut.h>

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

Vec3f Material::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
    assert(false);
    return Vec3f();
}

void Material::glSetMaterial(void) const
{
    assert(false);
}

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

PhongMaterial::PhongMaterial(const Vec3f & diffuseColor, const Vec3f & tspecularColor, float texponent)
	:Material(diffuseColor), specularColor(tspecularColor), exponent(texponent) {}

Vec3f PhongMaterial::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
	// for directional light sources, dirToLight is normalized
	// coefficient is not set yet
	// 这里没有计算 ambient light
	Vec3f dirlightnormal = dirToLight;
	dirlightnormal.Normalize();
	// 
	float t = dirlightnormal.Dot3(hit.getNormal());
	if (t < 0)
		return Vec3f(0, 0, 0);
	Vec3f Diffuse = t *getDiffuseColor();
	//Vec3f Diffuse;

	// specular

	Vec3f raydire = ray.getDirection();
	raydire.Normalize();
	Vec3f Specular;
	if (Blinn)
	{
		Vec3f r = dirlightnormal - raydire;
		r.Normalize();
		Specular = std::pow(std::max(hit.getNormal().Dot3(r), (float)0), exponent)*getSpecularColor();
	}
	else
	{
		Vec3f r = 2 * t * hit.getNormal() - dirlightnormal;
		Specular = std::pow(std::max(-raydire.Dot3(r), (float)0), exponent)*getSpecularColor();
	}
	//Vec3f Specular = std::pow(std::max(-raydire.Dot3(r), (float)0), 40)*getSpecularColor();

	//Vec3f Specular;
	//float len = dirToLight.Length();

	Vec3f result = (Diffuse + Specular) * lightColor;

	//result /= (len*len);
	if (result.x() != 0)
	{
		return result;
	}
	return result;
}


void PhongMaterial::glSetMaterial(void) const
{

	GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat specular[4] = {
		getSpecularColor().r(),
		getSpecularColor().g(),
		getSpecularColor().b(),
		1.0 };
	GLfloat diffuse[4] = {
		getDiffuseColor().r(),
		getDiffuseColor().g(),
		getDiffuseColor().b(),
		1.0 };

	// NOTE: GL uses the Blinn Torrance version of Phong...      
	float glexponent = exponent;
	if (glexponent < 0) glexponent = 0;
	if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);//漫反射
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);//环境光
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);//漫反射
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);//环境光

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

	// OPTIONAL: 3 pass rendering to fix the specular highlight 
	// artifact for small specular exponents (wide specular lobe)

	if (SPECULAR_FIX_WHICH_PASS == 0) {
		// First pass, draw only the specular highlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	}
	else if (SPECULAR_FIX_WHICH_PASS == 1) {
		// Second pass, compute normal dot light 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}
	else {
		// Third pass, add ambient & diffuse terms
		assert(SPECULAR_FIX_WHICH_PASS == 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
}

#endif
}
