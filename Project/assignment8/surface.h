 #pragma once
#include "arg_parser.h"
#include "vectors.h"
#include "triangle_mesh.h"
#include "spline.h"

class Surface : public Spline
{
public:
    Surface() {};
    ~Surface() {};
};

class SurfaceOfRevolution : public Surface
{
    Curve *curve;
public:
    SurfaceOfRevolution(Curve *c) : curve(c){}
};

class BezierPatch : public Surface
{
    vector<Vec3f> vertices;
public:
    void set(int i, Vec3f v) { vertices[i] = v; }
};
