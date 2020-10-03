 #pragma once
#include "arg_parser.h"
#include "vectors.h"
#include "triangle_mesh.h"
#include "spline.h"
#include "matrix.h"

// TODO9
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
    virtual void Paint(ArgParser *args) { curve->Paint(args); };

    SurfaceOfRevolution(Curve *c) : curve(c)  {}
    virtual TriangleMesh* OutputTriangles(ArgParser *args) 
    {
        int len = curve->getNumVertices();
        int curve_tessellation = args->curve_tessellation;
        int revolution_tessellation = args->revolution_tessellation;

        vector<Vec3f> pos = curve->getPoints(curve_tessellation);
        int vertices = pos.size() * revolution_tessellation;
        int curvesize = pos.size();
        TriangleMesh* newmesh = new TriangleMesh(vertices, vertices * 2);
        float delta_t = 1. / curve_tessellation;

        float t = 3.1415926 * 2 / revolution_tessellation;
        int index = 0;
        for (int i = 0; i < revolution_tessellation; i++)
        {
            Matrix mat = Matrix::MakeYRotation(t * i);
            for (Vec3f p : pos)
            {
                Vec3f m = p;
                mat.Transform(p);
                newmesh->SetVertex(index++, p);
            }
        }

        index = 0;
        for (int i = 0; i < revolution_tessellation; i++)
        {
            for (int j = 0; j < curvesize; j++)
            {
                int index1 = i * curvesize + j;
                int index2 = i * curvesize + (j + 1) % curvesize;
                int index3 = ((i + 1) % revolution_tessellation) * curvesize + j;
                int index4 = ((i + 1) % revolution_tessellation) * curvesize + (j + 1) % curvesize;
                newmesh->SetTriangle(index++, index1, index2, index3);
                newmesh->SetTriangle(index++, index4, index3, index2);
            }
        }
        return newmesh;
    };
};

class BezierPatch : public Surface
{
    vector<Vec3f> vertices;
public:
    BezierPatch() : vertices(16){}
    void set(int i, Vec3f v) { vertices[i] = v; }
    virtual TriangleMesh* OutputTriangles(ArgParser *args)
    {
        int patch_tesellation = args->patch_tessellation;
        TriangleMesh* newmesh = new TriangleMesh((patch_tesellation + 1) * (patch_tesellation + 1), patch_tesellation * patch_tesellation * 2);

        float patchsize = 1. / patch_tesellation;
        int index = 0;
        for (int i = 0; i <= patch_tesellation; i++)
        {
            for (int j = 0; j <= patch_tesellation; j++)
            {
                float t = i * patchsize;
                float s = j * patchsize;
                Vec3f pos = BezierCurve::getPos(
                    BezierCurve::getPos(vertices[0], vertices[1], vertices[2], vertices[3], t),
                    BezierCurve::getPos(vertices[4], vertices[5], vertices[6], vertices[7], t),
                    BezierCurve::getPos(vertices[8], vertices[9], vertices[10], vertices[11], t),
                    BezierCurve::getPos(vertices[12], vertices[13], vertices[14], vertices[15], t),
                    s);
                newmesh->SetVertex(index++, pos);
            }
        }
        index = 0;
        int edge = patch_tesellation + 1;
        for (int i = 0; i < patch_tesellation; i++)
        {
            for (int j = 0; j < patch_tesellation; j++)
            {
                int index1 = i * edge + j;
                int index2 = i * edge + (j + 1);
                int index3 = (i + 1) * edge + j;
                int index4 = (i + 1) * edge + j + 1;
                newmesh->SetTriangle(index++, index1, index2, index3);
                newmesh->SetTriangle(index++, index4, index3, index2);
            }
        }
        return newmesh;
    };
};
