#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

#include <stdlib.h>

class ArgParser;
class SplineParser;
class Spline;

// ====================================================================
// A simplied version of the code from the OpenGL assignment

class GLCanvas {

public:
  // Constructor and destructor
  GLCanvas(void) { }
  ~GLCanvas(void) { }

  // Set up the canvas and enter the rendering loop
  // Note that this function will not return but can be
  // terminated by calling 'exit(0)'
  static void initialize(ArgParser *args, SplineParser *splines);

private:

  static ArgParser *args;
  static SplineParser *splines;

  // viewport information
  static int width;
  static int height;
  static float size;

  // the currently selected point (for editing)
  static Spline* selected_spline;
  static int selected_control_point;

  // Callback functions for mouse and keyboard events
  static void display(void);
  static void reshape(int w, int h);
  static void mouse(int button, int state, int x, int y);
  static void motion(int x, int y);
  static void keyboard(unsigned char key, int x, int y);
  static void mouseToScreen(int i, int j, float &x, float &y, float &epsilon);
};

// ====================================================================

#endif
