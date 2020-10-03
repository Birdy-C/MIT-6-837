 #pragma once
#include "arg_parser.h"
#include "vectors.h"
#include "triangle_mesh.h"
#include "spline.h"
#include <vector>

// TODO9
class Curve : public Spline
{
protected:
    int len;
    vector<Vec3f> vertices;
public:
    Curve(int l) : len(l), vertices(l) {};
    ~Curve() {};
    virtual void Paint(ArgParser *args);
    void set(int i, Vec3f v) { vertices[i] = v; }
    virtual Vec3f getPos(int l, float t) = 0;

    Vec3f getVertex(int i) { return vertices[i]; };
    virtual int getNumVertices() { return len; };
    virtual void moveControlPoint(int selectedPoint, float x, float y) { vertices[selectedPoint].Set(x, y, 0); };
    virtual void addControlPoint(int selectedPoint, float x, float y) { vertices.insert(vertices.begin() + selectedPoint, Vec3f(x, y, 0)); len++; };
    virtual void deleteControlPoint(int selectedPoint) { vertices.erase(vertices.begin() + selectedPoint); len--; };
    virtual vector<Vec3f> getPoints(int tessellation) = 0;
};

class BezierCurve : public Curve
{
public:
    static Vec3f getPos(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4, float t);
    Vec3f getPos(int l, float t);
    BezierCurve(int l) : Curve(l) {}
    vector<Vec3f> getPoints(int tessellation);
};

class BSplineCurve : public Curve
{
public:
    Vec3f getPos(int l, float t);
    BSplineCurve(int l) : Curve(l) {}
    vector<Vec3f> getPoints(int tessellation);
};
