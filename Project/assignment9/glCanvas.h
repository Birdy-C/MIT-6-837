#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

#include <stdlib.h>
#include "vectors.h"

class Parser;

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
  static void initialize(Parser *parser, float refresh, float dt, 
			 int integrator_color, int draw_vectors, 
			 float acceleration_scale, int motion_blur);

private:

  // the system
  static Parser *parser;

  // viewport & mouse
  static Vec3f camera_pos;
  static int width;
  static int height;
  static int mouse_button;
  static int mouse_x;
  static int mouse_y;

  // actions & rendering
  static int paused;
  static float refresh;
  static float dt;
  static int integrator_color;
  static int draw_vectors;
  static float acceleration_scale;
  static int motion_blur;

  // Callback functions for mouse and keyboard events
  static void display(void);
  static void reshape(int w, int h);
  static void mouse(int button, int state, int x, int y);
  static void motion(int x, int y);
  static void keyboard(unsigned char key, int x, int y);
  static void idle(int value);

  // helper functions
  static void step();
  static void restart();
};

// ====================================================================

#endif
