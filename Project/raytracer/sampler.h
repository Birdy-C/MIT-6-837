#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <assert.h>
#include "sample.h"


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

};

class JitteredSampler : public Sampler
{

};

// ==================================================================

#endif
