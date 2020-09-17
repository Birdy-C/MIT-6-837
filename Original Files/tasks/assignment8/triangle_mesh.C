
#include "triangle_mesh.h"

// ============================================================
// ============================================================

TriangleMesh::TriangleMesh(int num_verts, int num_tris) {
  num_vertices = num_verts;
  num_triangles = num_tris;
  // create the storage space
  if (num_vertices == 0) vertices = NULL;
  else vertices = new Vec3f[num_vertices];
  if (num_triangles == 0) triangles = NULL;
  else triangles = new int[num_triangles*3]; 
  // NOTE: the data structures are not zero-ed out
}


void TriangleMesh::Merge(const TriangleMesh &m) {
  // create the storage space to store all the old & new vertices
  int new_num_vertices = num_vertices + m.num_vertices;
  int new_num_triangles = num_triangles + m.num_triangles;
  Vec3f *new_vertices = new Vec3f[new_num_vertices];
  int *new_triangles = new int[new_num_triangles*3];

  // copy the old & new vertices & triangles
  int i;
  for (i = 0; i < num_vertices; i++) {
    new_vertices[i] = vertices[i]; }
  for (i = 0; i < m.num_vertices; i++) {
    new_vertices[i+num_vertices] = m.vertices[i]; }
  for (i = 0; i < num_triangles*3; i++) {
    new_triangles[i] = triangles[i]; }
  for (i = 0; i < m.num_triangles*3; i++) {
    new_triangles[i+num_triangles*3] = m.triangles[i]+num_vertices; } 
  
  // cleanup the old structures
  delete [] vertices;
  delete [] triangles;
  num_vertices = new_num_vertices;
  num_triangles = new_num_triangles;
  vertices = new_vertices;
  triangles = new_triangles;
}

void TriangleMesh::Output(FILE *file) {
  // save the data to an .obj file
  for (int i = 0; i < num_vertices; i++) {
    fprintf (file,"v %f %f %f\n", vertices[i].x(),vertices[i].y(),vertices[i].z());
  }
  // .obj files are usually indexed by 1 (but not always... grr)
  for (int t = 0; t < num_triangles; t++) {
    fprintf (file,"f %d %d %d\n", triangles[t*3+0]+1,triangles[t*3+1]+1,triangles[t*3+2]+1);
  }
}

// ============================================================
// ============================================================

TriangleNet::TriangleNet (int _u_tess, int _v_tess) 
  : TriangleMesh((_u_tess+1)*(_v_tess+1), _u_tess*_v_tess*2) { 
  u_tess = _u_tess;
  v_tess = _v_tess;
  // initialize fixed connectivity for triangles
  for (int i = 0; i < u_tess; i++) {
    for (int j = 0; j < v_tess; j++) {
      int index = (i*v_tess + j)*2;
      int a1 =  i   *(v_tess+1) +  j;
      int a2 = (i+1)*(v_tess+1) +  j;
      int b1 =  i   *(v_tess+1) + (j+1); 
      int b2 = (i+1)*(v_tess+1) + (j+1);
      SetTriangle(index  ,a1,b1,a2);
      SetTriangle(index+1,b1,b2,a2);
    }
  }
}

// ============================================================
// ============================================================
