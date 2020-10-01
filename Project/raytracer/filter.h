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
    Vec3f getColor(int i, int j, Film *film);
    virtual int getSupportRadius() = 0;
};

class BoxFilter:
    public Filter
{
    float radius;
public:
    BoxFilter(float _radius) : radius(_radius) {};
    float getWeight(float x, float y);
    int getSupportRadius() { return ceil(radius); };
};

class TentFilter : 
    public Filter
{
    float radius;
public:
    TentFilter(float _radius) : radius(_radius) {};
    float getWeight(float x, float y);
    int getSupportRadius() { return ceil(radius); };
};

class GaussianFilter :
    public Filter
{
    float radius;
    float sigma;
public:
    GaussianFilter(float _sigma) : radius(2 * _sigma), sigma(_sigma) {};
    float getWeight(float x, float y);
    int getSupportRadius() { return ceil(radius); };
};

// ==================================================================

#endif