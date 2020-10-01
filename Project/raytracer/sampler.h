#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <assert.h>
#include "sample.h"
#include "info.h"


class Sampler {
public:
    virtual Vec2f getSamplePosition(int n) = 0;
};

class RandomSampler : public Sampler
{
public:
    virtual Vec2f getSamplePosition(int n) {
        return Vec2f(rand() % 256 / 256., rand() % 256 / 256.);
    }
};

class UniformSampler : public Sampler
{
    int len;
public:
    UniformSampler(int n) { len = sqrt(n); }
    virtual Vec2f getSamplePosition(int n) {
        Vec2f t = Vec2f(0.5 + n % len, 0.5 + (n / len) % len);
        return Vec2f(1.f / len * t.x(), 1.f / len * t.y());
    }
};

class JitteredSampler : public Sampler
{
    int len;
    UniformSampler uniform_s;
    RandomSampler random_s;
public:
    JitteredSampler(int n) : uniform_s(n){ len = sqrt(n); }
    virtual Vec2f getSamplePosition(int n) {
        Vec2f result = uniform_s.getSamplePosition(n);
        Vec2f result2 = random_s.getSamplePosition(n);
        result2 += Vec2f(-0.5f, -0.5f);
        result2 *= (1.0 / len);
        result += result2;
        return result;
    }
};

// ==================================================================

#endif
