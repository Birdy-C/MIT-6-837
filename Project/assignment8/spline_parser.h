#ifndef _SPLINE_PARSER_H_
#define _SPLINE_PARSER_H_

#include <assert.h>
#include "vectors.h"
class Spline;
class Curve;
class Surface;
class ArgParser;

#define MAX_PARSER_TOKEN_LENGTH 100

// ====================================================================
// ====================================================================

class SplineParser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  SplineParser(const char *file);
  ~SplineParser();

  // ACCESSORS
  int getNumSplines() { return num_splines; }
  Spline* getSpline(int i) {
    assert (i >= 0 && i < num_splines);
    return splines[i]; }

  // OUTPUT
  void SaveBezier(ArgParser *args);
  void SaveBSpline(ArgParser *args);
  void SaveTriangles(ArgParser *args);

  // PICK!  
  // iterates through all of the splines and finds the closest control
  // point to the current mouse position (or the closest edge)
  void Pick(float x, float y, float epsilon,
	    Spline*& selected_spline, int &selected_control_point);
  void PickEdge(float x, float y, float epsilon,
		Spline*& selected_spline, int &selected_control_point);

private:
  // don't use this constructor
  SplineParser() { assert(0); } 

  // HELPER FUNCTIONS
  Spline* ParseSpline();
  Curve* ParseBezierCurve();
  Curve* ParseBSplineCurve();
  Surface* ParseSurfaceOfRevolution();
  Surface* ParseBezierPatch();

  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  Vec3f readVec3f();
  Vec2f readVec2f();
  float readFloat();
  int readInt();

  // REPRESENTATION
  int num_splines;
  Spline **splines;
  FILE *file;
};

// ====================================================================
// ====================================================================


#endif
