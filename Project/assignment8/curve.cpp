#include "curve.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <matrix.h>

void Curve::Paint(ArgParser * args)
{
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 1; i < len; i++)
    {
        glVertex3f(vertices[i - 1].x(), vertices[i - 1].y(), vertices[i - 1].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();

    glColor3f(1.f, 1.f, 1.f);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i = 0; i < len; i++)
    {
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();

    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(1);
    glBegin(GL_LINES);
    bool start = true;
    vector<Vec3f> vertices = getPoints(args->curve_tessellation);
    Vec3f m;
    for (int i = 0; i < vertices.size(); i++)
    {
        m = vertices[i];
        if (start)
        {
            start = false;
            glVertex3f(m.x(), m.y(), m.z());
        }
        else
        {
            glVertex3f(m.x(), m.y(), m.z());
            glVertex3f(m.x(), m.y(), m.z());
        }
    }
    glVertex3f(m.x(), m.y(), m.z());
    glEnd();


}


Vec3f BezierCurve::getPos(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4, float t)
{
    static const float f[] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };
    static const Matrix m(f);
    Vec4f TVector(t * t * t, t * t, t, 1);
    m.Transform(TVector);
    return v1 * TVector.x() + v2 * TVector.y() + v3 * TVector.z() + v4 * TVector.w();
}

Vec3f BezierCurve::getPos(int l, float t)
{
    static const float f[]  = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };
    static const Matrix m(f);
    Vec4f TVector(t * t * t, t * t, t, 1);
    m.Transform(TVector);
    return vertices[l] * TVector.x() + vertices[l + 1] * TVector.y() + vertices[l + 2] * TVector.z() + vertices[l + 3] * TVector.w();
}


vector<Vec3f> BezierCurve::getPoints(int tessellation)
{
    vector<Vec3f> result;
    float delta_t = 1. / tessellation;
    
    for (int i = 0; i < len - 3; i += 3)
    {
        for (int j = 0; j <= tessellation; j++)
        {
            Vec3f m = getPos(i, delta_t * j);
            result.push_back(m);
        }
    }
    return result;
}

Vec3f BSplineCurve::getPos(int l, float t)
{
    static const float f[] = { -1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0 };
    static const Matrix m(f);
    Vec4f TVector(t * t * t, t * t, t, 1);
    TVector *= (1.f / 6);
    m.Transform(TVector);
    return vertices[l] * TVector.x() + vertices[l + 1] * TVector.y() + vertices[l + 2] * TVector.z() + vertices[l + 3] * TVector.w();
}


vector<Vec3f> BSplineCurve::getPoints(int tessellation)
{
    vector<Vec3f> result;
    float delta_t = 1. / tessellation;

    for (int i = 0; i < len - 3; i++)
    {
        for (int j = 0; j <= tessellation; j++)
        {
            Vec3f m = getPos(i, delta_t * j);
            result.push_back(m);
        }
    }
    return result;
}
