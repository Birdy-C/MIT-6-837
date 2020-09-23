#pragma once
#include "object3d.h"
#include "boundingbox.h"
#include <vector>
#include <algorithm>
#include "raytracing_stats.h"

// TODO5

// store the information for the current ray and the current grid cell
struct MarchingInfo
{
    float tmin;
    int i, j, k;  // current grid cell
    Vec3f ri;
    Vec3f dis;
    Vec3f nor;
    Vec3f rs;
    Vec3f pos;
    bool sign_x, sign_y, sign_z;
    void nextCell() {
        // https://www.shadertoy.com/view/4dfGzs
        RayTracingStats::IncrementNumIntersections();
        Vec3f dire = Vec3f(dis.x() < min(dis.y(), dis.z()), dis.y() < min(dis.x(), dis.z()), dis.z() < min(dis.x(), dis.y()));
        // Fix problem for parallel
        if (dire.Length() < 0.001 || dire.Length() > 1.1)
        {
            dis += Vec3f(rand(), rand(), rand()) * 0.01 * Vec3f(abs(ri.x()) > 100, abs(ri.y()) > 100, abs(ri.z()) > 100);
            dire = Vec3f(dis.x() < min(dis.y(), dis.z()), dis.y() < min(dis.x(), dis.z()), dis.z() < min(dis.x(), dis.y()));
        }
        if (dire.Length() < 0.001 || dire.Length() > 1.1)
        {
            dis += Vec3f(rand(), rand(), rand()) * 0.01;
            dire = Vec3f(dis.x() < min(dis.y(), dis.z()), dis.y() < min(dis.x(), dis.z()), dis.z() < min(dis.x(), dis.y()));
        }
        nor = dire * rs * (-1);
        dis += dire * rs * ri;
        pos += dire * rs;
        updateijk();
    }

    void updateijk()
    {
        Vec3f t = pos;
        i = t.x();
        j = t.y();
        k = t.z();
    }
};


class Grid :
    public Object3D
{
    Material* indicatesMaterial[10];
public:

    Grid(BoundingBox *bb, int nx, int ny, int nz);
    ~Grid();

    void setRecord(int x, int y, int z, Object3D* obj);
    Vec3f center(int x, int y, int z);
    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectReal(const Ray &r, Hit &h, float tmin);
    vector<Vec3f> getFace(int x, int y, int z, int type);
    Vec3f getNormal(int type);
    void paint(void);
    const int nx, ny, nz;
    float gethalfdiag() {
        return halfdiag;
    }
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
    Vec3f getBlockSize() {
        return block;
    };
private:

    void drawface(int i, int j, int k, int type);
    std::vector<std::vector<Object3D*>> record;
    float halfdiag;
    Vec3f block;

    std::vector<Object3D*>* getRecord(int x, int y, int z);
    bool checkinside(int x, int y, int z, Vec3f point);
};

