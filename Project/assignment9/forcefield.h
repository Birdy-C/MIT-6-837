 #pragma once
#include "vectors.h"

// TODO9
class ForceField
{
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField 
    : public ForceField
{
    Vec3f gravity;
public:
    GravityForceField(Vec3f g) : gravity(g) {};
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return gravity; };
};

class ConstantForceField
    : public ForceField
{
    Vec3f force;
public:
    ConstantForceField(Vec3f f) : force(f) {};
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return force * ( 1./ mass); };
};

class RadialForceField
    : public ForceField
{
    float magnitude;
public:
    RadialForceField(float m) : magnitude(m) {};
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return position * ( -magnitude); };
};

class VerticalForceField
    : public ForceField
{
    float magnitude;
public:
    VerticalForceField(float m) : magnitude(m) {};
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return Vec3f(0, position.y(), 0) * (-magnitude); };
};


class WindForceField
    : public ForceField
{
public:
    WindForceField(float magnitude) {};
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return Vec3f(); };
};