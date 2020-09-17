#ifndef _TRIANGLE_MESH_
#define _TRIANGLE_MESH_

#include <assert.h>
#include <stdio.h>
#include "vectors.h"

// =================================================================
// a simple triangle mesh class that can be saved to obj format

class TriangleMesh {

public:
  // CONSTRUCTORS & DESTRUCTORS
  TriangleMesh(int num_verts, int num_tris);
  ~TriangleMesh() {
    delete [] vertices; 
    delete [] triangles; }

  // MODIFIERS
  void SetVertex(int i, Vec3f v) {
    assert (i >= 0 && i < num_vertices);
    vertices[i] = v; }
  void SetTriangle(int i, int a, int b, int c) {
    assert (i >= 0 && i < num_triangles);
    triangles[i*3+0] = a;
    triangles[i*3+1] = b;
    triangles[i*3+2] = c; }
  // this useful function renumbers the vertices & triangles to
  // concatenate triangle meshes correctly
  void Merge(const TriangleMesh &m);

  // SAVE TO OBJ
  void Output(FILE *file);
  
protected:

  // don't use this constructor
  TriangleMesh() { assert(0); } 

  // REPRESENTATION
  int num_vertices;
  int num_triangles;
  Vec3f *vertices;
  // stores 3 indices (into vertex array) per triangle
  int *triangles; 
};

// =================================================================
// This derived class stores a special type of triangle mesh with
// uniform connectivity.  The user chooses a density of triangles
// along each of 2 dimensions and the connectivity is automatic.  The
// user just needs to specify the grid of points. 

// NOTE: the two input parameters is the resolution of the triangles.
// The grid of vertices is one bigger in each dimension.

class TriangleNet : public TriangleMesh {

public:

  // CONSTRUCTOR & DESTRUCTOR
  TriangleNet (int _u_tess, int _v_tess);
  ~TriangleNet() {}

  // MODIFIER
  void SetVertex(int i, int j, Vec3f v) {
    assert (i >= 0 && i <= u_tess);
    assert (j >= 0 && j <= v_tess);
    int index = i*(v_tess+1) + j;
    TriangleMesh::SetVertex(index,v); }

private:

  // don't use this constructor
  TriangleNet() { assert(0); }

  // REPRESENTATION
  int u_tess;
  int v_tess;
};

// =================================================================

#endif

