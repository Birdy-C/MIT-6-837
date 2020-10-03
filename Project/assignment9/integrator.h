#pragma once
#include "particle.h"
#include "forcefield.h"

// TODO9
class Integrator
{
public:
    virtual Vec3f getColor() { return Vec3f(1.0); }
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
};

class EulerIntegrator : public Integrator
{
public:
    virtual Vec3f getColor() { return Vec3f(1.0, 0, 0); }

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) 
    {
        Vec3f p = particle->getPosition();
        Vec3f v = particle->getVelocity();
        Vec3f a = forcefield->getAcceleration(p, particle->getMass(), t);
        particle->setPosition(p + v * dt);
        particle->setVelocity(v + a * dt);
        particle->increaseAge(dt);
    }
};

class MidpointIntegrator : public Integrator
{
public:
    virtual Vec3f getColor() { return Vec3f(0, 1.0, 0); }

    void Update(Particle *particle, ForceField *forcefield, float t, float dt)
    {
        Vec3f p = particle->getPosition();
        Vec3f v = particle->getVelocity();
        Vec3f a = forcefield->getAcceleration(p, particle->getMass(), t);

        Vec3f pm = p + v * dt * 0.5;
        Vec3f vm = v + a * dt * 0.5;
        Vec3f am = forcefield->getAcceleration(pm, particle->getMass(), t + dt * 0.5);
        particle->setPosition(p + vm * dt);
        particle->setVelocity(v + am * dt);
        particle->increaseAge(dt);
    }
};


class RungeKuttaIntegrator : public Integrator
{
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {}
};
