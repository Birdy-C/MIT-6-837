 #pragma once
#include "arg_parser.h"
#include "vectors.h"
#include "triangle_mesh.h"
#include "spline.h"
#include <vector>
class Curve : public Spline
{
    int len;
    vector<Vec3f> vertices;
public:
    Curve(int l) : len(l), vertices(l) {};
    ~Curve() {};
    virtual void Paint(ArgParser *args);
    void set(int i, Vec3f v) { vertices[i] = v; }
    Vec3f getVertex(int i) { return vertices[i]; };
    virtual int getNumVertices() { return len; };
    virtual void moveControlPoint(int selectedPoint, float x, float y) { vertices[selectedPoint].Set(x, y, 0); };
    virtual void addControlPoint(int selectedPoint, float x, float y) { vertices.insert(vertices.begin() + selectedPoint, Vec3f(x, y, 0)); len++; };
    virtual void deleteControlPoint(int selectedPoint) { vertices.erase(vertices.begin() + selectedPoint); len--; };
};

class BezierCurve : public Curve
{
public:
    BezierCurve(int l) : Curve(l) {}
};

class BSplineCurve : public Curve
{
public:
    BSplineCurve(int l) : Curve(l) {}
};
