#ifndef _FILTER_H
#define _FILTER_H

#include <assert.h>
#include "sample.h"
#include "film.h"

class Filter {
public:
    // returns the weight for point (x + pixel center, y + pixel center)
    virtual float getWeight(float x, float y) = 0;

    // computes the appropriately filtered color for pixel (i,j) from the samples stored in the Film instance
    Vec3f getColor(int i, int j, Film *film) {};

    virtual int getSupportRadius() = 0;
};

class BoxFilter:
    public Filter
{
public:
    float getWeight(float x, float y) { return 0.f; };
    int getSupportRadius() { return 1; };
};

class TentFilter : 
    public Filter
{
public:
    float getWeight(float x, float y) { return 0.f; };
    int getSupportRadius() { return 1; };
};

class GaussianFilter :
    public Filter
{
public:
    float getWeight(float x, float y) { return 0.f; };
    int getSupportRadius() { return 1; };
};

// ==================================================================

#endif