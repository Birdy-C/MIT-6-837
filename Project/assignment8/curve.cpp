#include "curve.h"
#include <GL/glut.h>
#include <GL/gl.h>

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
}
