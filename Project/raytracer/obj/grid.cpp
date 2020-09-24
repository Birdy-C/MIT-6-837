#include "stdafx.h"
#include "grid.h"
#include <algorithm>
#include "rayTree.h"
#include <set>
#include <map>

Grid::Grid(BoundingBox *bb, int _nx, int _ny, int _nz)
    : nx(_nx), ny(_ny), nz(_nz)
{
    itsboundingbox = bb;

    record = std::vector<std::vector<Object3D*>>(nx * ny * nz);
    block = itsboundingbox->getMax() - itsboundingbox->getMin();
    block.Divide(nx, ny, nz);
    halfdiag = block.Length() / 2;
    itsMaterial = new PhongMaterial(Vec3f(1), Vec3f(0), 0);
    indicatesMaterial[0] = new PhongMaterial(Vec3f(1), Vec3f(0), 0);
    for (int i = 1; i < 10; i++)
    {
        Vec3f color = Vec3f(rand() % 256, rand() % 256, rand() % 256) * (1. / 256.);
        indicatesMaterial[i] = new PhongMaterial(color, Vec3f(0), 0);
    }
}

Grid::~Grid()
{
    for (int i = 0; i < 10; i++)
    {
        delete indicatesMaterial[i];
    }
}

std::vector<Object3D*>* Grid::getRecord(int x, int y, int z)
{
    if (x < 0 || x >= nx) return nullptr;
    if (y < 0 || y >= ny) return nullptr;
    if (z < 0 || z >= nz) return nullptr;

    return &record[x * (ny * nz) + y * nz + z];
}

bool Grid::checkinside(int x, int y, int z, Vec3f point)
{
    Vec3f t = Vec3f(x, y, z);
    Vec3f minimum = itsboundingbox->getMin() + block * t;
    Vec3f maximum = itsboundingbox->getMin() + block * (t + 1);
    if (point.x() >= minimum.x() && point.y() >= minimum.y() && point.z() >= minimum.z() &&
        point.x() <= maximum.x() && point.y() <= maximum.y() && point.z() <= maximum.z())
    {
        return true;
    }
    return false;
}

bool Grid::checkintersect(Object3D *obj, const Ray & r, Hit & h, float tmin)
{
    if (matrixrecord.find(obj) == matrixrecord.end())
    {
        return obj->intersect(r, h, tmin);
    }
    else
    {
        return Transform::intersectWarp(obj, matrixrecord[obj], r, h, tmin);
    }
}

Vec3f Grid::center(int x, int y, int z)
{
    return itsboundingbox->getMin() + (Vec3f(x, y, z) + 0.5) * block;  
}


bool Grid::intersect(const Ray & r, Hit & h, float tmin)
{
    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);
    int count = 2; // if outside of grid twice, abort
    while (1)
    {
        std::vector<Object3D*>*  temp = getRecord(mi.i, mi.j, mi.k);
        if (!temp)
        {
            count--;
            if (count == 0)
            {
                return false;
            }
            else
            {
                mi.nextCell();
                continue;
            }
        }
        int size = temp->size();
        size = min(size, 9);
        for (int i = 0; i < 6; i++)
        {
            auto faces = getFace(mi.i, mi.j, mi.k, i);
            auto normal = getNormal(i);
            RayTree::AddHitCellFace(faces[0], faces[1], faces[2], faces[3], normal, indicatesMaterial[size]);
        }
        {
            int index = mi.nor.Dot3(Vec3f(1, 3, 5));
            if (index < 0) index = -index;
            else index -= 1;

            index = max(min(index, 5), 0);
            auto faces = getFace(mi.i, mi.j, mi.k, index);
            RayTree::AddEnteredFace(faces[0], faces[1], faces[2], faces[3], mi.nor, itsMaterial);
        }
        if (size > 0)
        {

            h.set(200, indicatesMaterial[size - 1], mi.nor, r);
            return true;
        }
        mi.nextCell();
    }
    return false;
}


bool Grid::intersectReal(const Ray & r, Hit & h, float tmin)
{
    bool result = false;
    for (Object3D* obj : infinite_record)
    {
        result = checkintersect(obj, r, h, tmin);
    }
    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);
    int count = 2; // if outside of grid twice, abort
    set<Object3D*> checked;
    while (1)
    {
        std::vector<Object3D*>* temp = getRecord(mi.i, mi.j, mi.k);
        if (!temp)
        {
            count--;
            if (count == 0)
            {
                return result;
            }
            else
            {
                mi.nextCell();
                continue;
            }
        }
        int size = temp->size();   
        if (size > 0)
        {
            for (Object3D* obj : *temp)
            {
                if (checked.find(obj) == checked.end())
                {
                    checkintersect(obj, r, h, tmin);
                    checked.insert(obj);
                }
            }
            if (checkinside(mi.i, mi.j, mi.k, h.getIntersectionPoint()))
            {
                return true;
            }
        }
        mi.nextCell();
    }
    return result;
}

void Grid::insertMatrix(Object3D * obj, Matrix * mat)
{
    if (matrixrecord.find(obj) == matrixrecord.end())
    {
        if (mat)
        {
            matrixrecord[obj] = *mat;
        }
        else
        {
            Matrix Identity;
            Identity.SetToIdentity();
            matrixrecord[obj] = Identity;
        }
    }
}


vector<Vec3f> Grid::getFace(int x, int y, int z, int type)
{
    Vec3f r = itsboundingbox->getMax() - itsboundingbox->getMin();
    Vec3f side = block * 0.5;
    Vec3f centerpoint = center(x, y, z);
    Vec3f points[] = { Vec3f(1,-1,-1), Vec3f(1,1,-1), Vec3f(-1,1,-1), Vec3f(-1,-1,-1),
        Vec3f(1,-1,1), Vec3f(1,1,1), Vec3f(-1,1,1), Vec3f(-1,-1,1) };
    int index[6][4] = { {0,1,5,4}, {2,3,7,6}, {1,2,6,5}, {3,0,4,7}, {4,5,6,7}, {3,2,1,0} };
    vector<Vec3f> result;
    for (int i = 0; i < 4; i++)
    {
        Vec3f t = points[index[type][i]];
        t = t * side + centerpoint;
        result.push_back(t);
    }
    return result;
}

Vec3f Grid::getNormal(int type)
{
    static Vec3f nor[] = { Vec3f(1,0,0),Vec3f(-1,0,0),Vec3f(0,1,0),Vec3f(0,-1,0),
        Vec3f(0,0,1),Vec3f(0,0,-1) };
    return nor[type];
}

void Grid::drawface(int x, int y, int z, int type)
{
    Vec3f nor = getNormal(type);
    glNormal3f(nor.x(), nor.y(), nor.z());
    vector<Vec3f> points = getFace(x, y, z, type);
    for(auto t: points)
    { 
        glVertex3f(t.x(), t.y(), t.z());
    }
}


void Grid::paint(void)
{
    glSetMaterial();
    // TODO delete useless face
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            for (int k = 0; k < nz; k++)
            {
                int size = getRecord(i, j, k)->size();
                if (size > 0)
                {
                    size = min(size, 9);
                    indicatesMaterial[size - 1]->glSetMaterial();
                    glBegin(GL_QUADS);
                    for (int t = 0; t < 6; t++)
                    {
                        drawface(i, j, k, t);
                    }
                    glEnd();
                }
            }
        }
    }
}



void Grid::initializeRayMarch(MarchingInfo & mi, const Ray & r, float tmin) const
{
    Vec3f t1 = (itsboundingbox->getMax() - r.getOrigin());
    t1.Divide(r.getDirection());
    Vec3f t2 = (itsboundingbox->getMin() - r.getOrigin());
    t2.Divide(r.getDirection());
    float txmin = min(t1.x(), t2.x());
    float txmax = max(t1.x(), t2.x());
    float tymin = min(t1.y(), t2.y());
    float tymax = max(t1.y(), t2.y());
    float tzmin = min(t1.z(), t2.z());
    float tzmax = max(t1.z(), t2.z());

    float allmin = max(txmin, max(tymin, tzmin));
    float allmax = min(txmax, min(tymax, tzmax));
    if (allmin > allmax)
    {
        // no intersection
    }

    mi.tmin = max(tmin, allmin);
    Vec3f pos = (r.pointAtParameter(mi.tmin) - itsboundingbox->getMin());
    pos.Divide(block);
    mi.sign_x = r.getDirection().x() > 0;
    mi.sign_y = r.getDirection().y() > 0;
    mi.sign_z = r.getDirection().z() > 0;

    mi.pos = Vec3f(floor(pos.x()), floor(pos.y()), floor(pos.z()));
    mi.rs = Vec3f(mi.sign_x, mi.sign_y, mi.sign_z) * 2. - 1.;
    Vec3f vel = r.getDirection();
    vel.Divide(block);
    vel.Normalize();
    if (vel.x() == 0)
    {
        vel += Vec3f(0.000001, 0, 0) * mi.rs.x();
    }
    if (vel.y() == 0)
    {
        vel += Vec3f(0, 0.000001, 0) * mi.rs.y();
    }
    if (vel.z() == 0)
    {
        vel += Vec3f(0, 0, 0.0000001)* mi.rs.z();
    }

    mi.ri = 1;
    mi.ri.Divide(vel);
    mi.dis = (mi.pos - pos + 0.5 + 0.5 * mi.rs) * mi.ri;
    {
        Vec3f normal = Vec3f(0);
        if (txmin + 0.001 > allmin) normal += !mi.sign_x ? Vec3f(1, 0, 0) : Vec3f(-1, 0, 0);
        if (tymin + 0.001 > allmin) normal += !mi.sign_y ? Vec3f(0, 1, 0) : Vec3f(0, -1, 0);
        if (tzmin + 0.001 > allmin) normal += !mi.sign_z ? Vec3f(0, 0, 1) : Vec3f(0, 0, -1);
        mi.nor = normal;
    }
    mi.updateijk();
}

void Grid::insertInfinite(Object3D * obj)
{
    infinite_record.push_back(obj);
}

void Grid::setRecord(int x, int y, int z, Object3D* obj)
{
    getRecord(x, y, z)->push_back(obj);
}
