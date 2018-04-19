#include "spline_parser.h"
#include "spline.h"
#include "curve.h"
#include "surface.h"
#include "triangle_mesh.h"
#include "arg_parser.h"

// ====================================================================
// ====================================================================

SplineParser::SplineParser(const char *spline_file) {

  // open the file for reading
  assert (spline_file != NULL);
  file = fopen(spline_file,"r");
  assert (file != NULL);
  char token[MAX_PARSER_TOKEN_LENGTH];

  // read in the number of splines in this file
  getToken(token);
  assert (!strcmp(token,"num_splines"));
  num_splines = readInt();
  splines = new (Spline*)[num_splines];

  // read the splines
  for (int i = 0; i < num_splines; i++) {
    Spline *s = ParseSpline();
    assert (s != NULL);
    splines[i] = s;
  }

  fclose(file);
}


SplineParser::~SplineParser() {
  // cleanup
  for (int i = 0; i < num_splines; i++) {
    delete splines[i]; }
  delete [] splines;
}

// ====================================================================

Spline* SplineParser::ParseSpline() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token);
  Spline *answer = NULL;
  if (!strcmp(token,"bezier")) {
    answer = ParseBezierCurve();
  } else if (!strcmp(token,"bspline")) {
    answer = ParseBSplineCurve();
  } else if (!strcmp(token,"surface_of_revolution")) {
    answer = ParseSurfaceOfRevolution();
  } else if (!strcmp(token,"bezier_patch")) {
    answer = ParseBezierPatch();
  } else {
    printf ("ERROR unknown spline type %s\n", token);
    assert(0);
  }
  return answer;
}

Curve* SplineParser::ParseBezierCurve() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token);
  assert (!strcmp(token,"num_vertices"));
  int num_vertices = readInt();
  assert (num_vertices >= 4);
  // a bezier curve is just a list of vertices
  BezierCurve *answer = new BezierCurve(num_vertices);
  for (int i = 0; i < num_vertices; i++) {
    Vec3f v = readVec3f();
    answer->set(i,v);
  }
  return answer;
}

Curve* SplineParser::ParseBSplineCurve() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token);
  assert (!strcmp(token,"num_vertices"));
  int num_vertices = readInt();
  assert (num_vertices >= 4);
  // a bspline curve is just a list of vertices
  BSplineCurve *answer = new BSplineCurve(num_vertices);
  for (int i = 0; i < num_vertices; i++) {
    Vec3f v = readVec3f();
    answer->set(i,v);
  }
  return answer;
}

Surface* SplineParser::ParseSurfaceOfRevolution() {
  // a surface of revolution is just a curve spun around 
  // the y axis (x = 0, z = 0)
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token);
  Curve *c = NULL;
  if (!strcmp(token,"bezier")) {
    c = ParseBezierCurve();
  } else if (!strcmp(token,"bspline")) {
    c = ParseBSplineCurve();
  } else {
    printf ("ERROR unknown curve type %s\n", token);
  }
  assert (c != NULL);
  Surface *answer = new SurfaceOfRevolution(c);
  return answer;
}

Surface* SplineParser::ParseBezierPatch() {
  // a bezier patch is just a list of 16 vertices
  BezierPatch *answer = new BezierPatch();
  for (int i = 0; i < 16; i++) {
    Vec3f v = readVec3f();
    answer->set(i,v);
  }
  return answer;
}

// ====================================================================

void SplineParser::Pick(float x, float y, float epsilon, 
			Spline*& selected_curve, int &selected_control_point) {
  selected_curve = NULL;
  float distance = epsilon;
  // iterate through all the splines
  for (int i = 0; i < num_splines; i++) {
    Spline *s = getSpline(i);
    for (int j = 0; j < s->getNumVertices(); j++) {
      Vec3f v = s->getVertex(j);
      float dx = v.x()-x;
      float dy = v.y()-y;
      float d = sqrt(dx*dx+dy*dy);
      // keep the closest point (as long as it's within epsilon)
      if (d < distance) {
	selected_curve = s;
	selected_control_point = j;
      }
    }
  }
}

float DistanceToLineSegment(Vec2f a, Vec2f b, Vec2f pt) {
  Vec2f dir = b; dir -= a;
  Vec2f va = pt; va -= a;
  Vec2f vb = pt; vb -= b;
  float denom = dir.Dot2(dir);
  // if points are too close to each other...
  if (denom < 0.00001) return va.Length();
  float t = dir.Dot2(va) / denom;
  // find the projection of the point onto the line
  Vec2f proj = dir; 
  proj *= t; 
  proj += a; 
  proj -= pt;
  // if the projection is past either end of the segment
  // return the distance to the closest endpoint
  if (t <= 0) return va.Length();
  if (t >= 1) return vb.Length();
  // otherwise return the distance to the projection
  return proj.Length();
}


void SplineParser::PickEdge(float x, float y, float epsilon, 
			    Spline*& selected_curve, int &selected_control_point) {
  selected_curve = NULL;
  float distance = epsilon;
  for (int i = 0; i < num_splines; i++) {
    Spline *s = getSpline(i);
    for (int j = 1; j < s->getNumVertices(); j++) {
      Vec3f v1 = s->getVertex(j-1);
      Vec3f v2 = s->getVertex(j);
      float d = DistanceToLineSegment(Vec2f(v1.x(),v1.y()),
				      Vec2f(v2.x(),v2.y()),
				      Vec2f(x,y));
      // keep the closest edge
      // the new point should be added along the segment from 
      // selected_control_point-1 to selected_control_point
      // the index of the new point will be selected_control_point
      if (d < distance) {
	selected_curve = s;
	selected_control_point = j;
      }
    }
  }
}


// ====================================================================

void SplineParser::SaveBezier(ArgParser *args) {
  // save the current set of splines, force them to be bezier when possible
  if (args->output_bezier_file == NULL) return;
  FILE *file = fopen(args->output_bezier_file,"w");
  assert (file != NULL);
  fprintf (file, "num_splines %d\n", getNumSplines());
  for (int i = 0; i < getNumSplines(); i++) {
    getSpline(i)->OutputBezier(file);
  }
  fclose(file);
}

void SplineParser::SaveBSpline(ArgParser *args) {
  // save the current set of splines, force them to be bspline when possible
  if (args->output_bspline_file == NULL) return;
  FILE *file = fopen(args->output_bspline_file,"w");
  assert (file != NULL);
  fprintf (file, "num_splines %d\n", getNumSplines());
  for (int i = 0; i < getNumSplines(); i++) {
    getSpline(i)->OutputBSpline(file);
  }
  fclose(file);
}

void SplineParser::SaveTriangles(ArgParser *args) {
  // save a triangle representation of the current set of splines, as an .obj file
  if (args->output_file == NULL) return;
  TriangleMesh mesh = TriangleMesh(0,0);
  for (int i = 0; i < getNumSplines(); i++) {
    TriangleMesh *m2 = getSpline(i)->OutputTriangles(args);
    mesh.Merge(*m2);
    delete m2;
  }
  FILE *file = fopen(args->output_file,"w");
  assert (file != NULL);
  mesh.Output(file);
  fclose(file);
}

// ====================================================================
// HELPER FUNCTIONS

int SplineParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
    token[0] = '\0';
    return 0;
  }
  return 1;
}


Vec3f SplineParser::readVec3f() {
  float x,y,z;
  int count = fscanf(file,"%f %f %f",&x,&y,&z);
  if (count != 3) {
    printf ("Error trying to read 3 floats to make a Vec3f\n");
    assert (0);
  }
  return Vec3f(x,y,z);
}


Vec2f SplineParser::readVec2f() {
  float u,v;
  int count = fscanf(file,"%f %f",&u,&v);
  if (count != 2) {
    printf ("Error trying to read 2 floats to make a Vec2f\n");
    assert (0);
  }
  return Vec2f(u,v);
}


float SplineParser::readFloat() {
  float answer;
  int count = fscanf(file,"%f",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 float\n");
    assert (0);
  }
  return answer;
}


int SplineParser::readInt() {
  int answer;
  int count = fscanf(file,"%d",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 int\n");
    assert (0);
  }
  return answer;
}

// ====================================================================
