 #pragma once
#include "vectors.h"
#include "particle.h"

static Vec3f randomVec3f()
{
    return Vec3f(rand() % 255, rand() % 255, rand() % 255) * (1 / 125.f) - 1.f;
}

class Generator
{
protected:
    Vec3f m_color; Vec3f m_dead_color; float m_color_randomness;
    float m_lifespan; float m_lifespan_randomness; int m_desired_num_particles;
    float m_mass; float m_mass_randomness;

public:
    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) 
    {
        m_color = color; m_dead_color = dead_color; m_color_randomness = color_randomness; 
    };
    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) 
    {
        m_lifespan = lifespan; m_lifespan_randomness = lifespan_randomness; m_desired_num_particles = desired_num_particles;
    };
    void SetMass(float mass, float mass_randomness) 
    {
        m_mass = mass; m_mass_randomness = mass_randomness;
    };

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;
    virtual Particle* Generate(float current_time, int i) = 0;

    // for the gui
    void Paint() const {};
    void Restart() { srand(0); };
};

class HoseGenerator : public Generator
{
    Vec3f m_position;
    float m_position_randomness;
    Vec3f m_velocity;
    float m_velocity_randomness;
public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness):
        m_position(position), m_position_randomness(position_randomness), m_velocity(velocity), m_velocity_randomness(velocity_randomness)
    {};
    virtual int numNewParticles(float current_time, float dt) const 
    {
        return dt * m_desired_num_particles / m_lifespan; 
    };
    Particle* Generate(float current_time, int i)
    {
        Particle *newp = new Particle(m_position + randomVec3f() * m_position_randomness, m_velocity + randomVec3f() * m_velocity_randomness, m_color, m_dead_color, m_mass, m_lifespan);
        return newp;
    };
};

class RingGenerator : public Generator
{
    float m_position_randomness;
    Vec3f m_velocity;
    float m_velocity_randomness;
public:
    RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness)
        : m_position_randomness(position_randomness), m_velocity(velocity), m_velocity_randomness(velocity_randomness) 
    {};
    virtual int numNewParticles(float current_time, float dt) const
    {
        return dt * m_desired_num_particles / m_lifespan * current_time * 0.1;
    };
    Particle* Generate(float current_time, int i)
    {
        Vec3f r = randomVec3f();
        r = Vec3f(r.x(), 0, r.z());
        r.Normalize();
        Particle *newp = new Particle(0.25 * r * current_time, m_velocity + randomVec3f() * m_velocity_randomness, m_color, m_dead_color, m_mass, m_lifespan);
        return newp;
    };
};

