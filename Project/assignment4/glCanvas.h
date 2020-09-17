// ====================================================================
// GLCanvas class by Rob Jagnow.
//
// The GLCanvas can be created with a call to the 'initialize' routine,
// which takes as parameters a pointer to the global SceneParser and
// the name of the routine that will perform the ray tracing.  Once the
// OpenGL interface is open, the scene can be rendered from the current
// camera position by pressing the 'r' key.
// ====================================================================

#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

#include <stdlib.h>

class SceneParser;

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight 
// artifact for small specular exponents (wide specular lobe)

//   0: don't fix   
//   1: do fix
#define SPECULAR_FIX 0 

// ====================================================================

class GLCanvas {

private:
  // A reference to the function that performs the raytracing
  // This gets called from the 'keyboard' routine
  static void (*renderFunction)(void);

  // A reference to the function that traces the ray tree for a single pixel
  // This gets called from the 'keyboard' routine
  static void (*traceRayFunction)(float,float);

  // A pointer to the global SceneParser
  static SceneParser *scene;

  // State of the mouse cursor
  static int mouseButton;
  static int mouseX;
  static int mouseY;

  // Helper function for the display routine
  static void drawAxes(void);

  // Callback functions for mouse and keyboard events
  static void display(void);
  static void reshape(int w, int h);
  static void mouse(int button, int state, int x, int y);
  static void motion(int x, int y);
  static void keyboard(unsigned char key, int x, int y);

public:
  // Constructor and destructor
  GLCanvas(void) {
    renderFunction = NULL;
    traceRayFunction = NULL; }
 ~GLCanvas(void) { }

  // Set up the canvas and enter the rendering loop
  // Note that this function will not return but can be
  // terminated by calling 'exit(0)'

  void initialize(SceneParser *_scene, void (*_renderFunction)(void), void (*_traceRayFunction)(float,float));
};

// ====================================================================

#endif
