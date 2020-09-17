// Included files for OpenGL Rendering
#include <GL/gl.h>
#include <GL/glut.h>

#include "glCanvas.h"
#include "parser.h"
#include "system.h"
#include "particle.h"
#include "matrix.h"

// ========================================================

// STATIC VARIABLES
// the system
Parser* GLCanvas::parser = NULL;

// viewport & mouse
Vec3f GLCanvas::camera_pos = Vec3f(0,0,20);
int GLCanvas::width = 250;
int GLCanvas::height = 250;
int GLCanvas::mouse_button = -1;
int GLCanvas::mouse_x = 0;
int GLCanvas::mouse_y = 0;

// actions & rendering
int GLCanvas::paused = 0;
float GLCanvas::refresh = 100;
float GLCanvas::dt = 100;
int GLCanvas::integrator_color = 0;
int GLCanvas::draw_vectors = 0;
float GLCanvas::acceleration_scale = 0;
int GLCanvas::motion_blur = 0;

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void GLCanvas::initialize(Parser *_parser, float _refresh, float _dt, 
			  int _integrator_color, int _draw_vectors, float _acceleration_scale, int _motion_blur) {
  parser = _parser;
  refresh = _refresh;
  dt = _dt;
  integrator_color = _integrator_color;
  draw_vectors = _draw_vectors;
  acceleration_scale = _acceleration_scale;
  motion_blur = _motion_blur;

  // Set global lighting parameters
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);

  // Set window parameters
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Particle System");

  // Ambient light
  GLfloat ambArr[] = { 0.1,0.1,0.1,1 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glDisable(GL_CULL_FACE);

  // Initialize callback functions
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  glutTimerFunc(0,idle,0);

  // Enter the main rendering loop
  glutMainLoop();
}

// ========================================================
// Callback for the OpenGL display loop.  To request that
// this function get called, use glutPostRedisplay()
// ========================================================

void GLCanvas::display(void) {

  // Clear the display buffer
  glClearColor(0,0,0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // setup a light
  GLfloat pos[4] = {1,1,1,1};
  GLfloat one[4] = {1,1,1,1};
  GLfloat zero[4] = {0,0,0,1};
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, one);
  glLightfv(GL_LIGHT1, GL_SPECULAR, zero);
  glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
  glEnable(GL_LIGHT1);

  // Set the camera parameters
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera_pos.x(),camera_pos.y(),camera_pos.z(),
	    0,0,0,
	    0,1,0);

  // Draw the geometry
  glEnable(GL_LIGHTING);
  for (int i = 0; i < parser->getNumSystems(); i++) {
    parser->getSystem(i)->PaintGeometry(); }
  glDisable(GL_LIGHTING);

  // Draw the systems
  for (int i = 0; i < parser->getNumSystems(); i++) {
    parser->getSystem(i)->Paint(dt,integrator_color,draw_vectors,acceleration_scale,motion_blur); }

  // swap buffers
  glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void GLCanvas::reshape(int w, int h) {

  // Set the OpenGL viewport to fill the entire window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  // Set the camera parameters to reflect the changes
  width = w;
  height = h;

  // perspective camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = float(w)/float(h);
  float asp_angle = 30; 
  if (aspect > 1) asp_angle /= aspect;
  gluPerspective(asp_angle, aspect, 0.1, 1000.0);
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void GLCanvas::mouse(int button, int state, int x, int y) {
  if (button != GLUT_LEFT_BUTTON &&
      button != GLUT_RIGHT_BUTTON) return;
  
  if (state == 0) {
    // mouse press
    mouse_x = x;
    mouse_y = y;
    mouse_button = button;
  } else {
    mouse_button = -1;
  }

  // Redraw the scene 
  glutPostRedisplay();
}

// ========================================================
// Callback function for mouse drag
// ========================================================

void GLCanvas::motion(int x, int y) {
  if (mouse_button == -1) return;

  // LEFT BUTTON ROTATES
  if (mouse_button == GLUT_LEFT_BUTTON) {
    float rx = 0.01*(mouse_x - x);
    float ry = -0.01*(mouse_y - y);
    // rotate around the center, keeping the y axis up
    Vec3f dir = camera_pos; 
    dir.Normalize();
    Vec3f up = Vec3f(0,1,0);
    Vec3f horiz;  
    Vec3f::Cross3(horiz,dir,up);
    horiz.Normalize();
    float tiltAngle = acos(up.Dot3(dir));
    if (tiltAngle-ry > 3.13)      ry = tiltAngle - 3.13;
    else if (tiltAngle-ry < 0.01) ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horiz, ry);
    rotMat.TransformDirection(dir);
    // keep camera same distance from origin
    float length = camera_pos.Length();
    camera_pos = dir * length;
  }
  
  // RIGHT BUTTON ZOOMS
  else {
    assert(mouse_button == GLUT_RIGHT_BUTTON);
    // zoom proportional to distance to origin
    float exponent = 0.001*(mouse_x - x);
    float zoom = pow(10,exponent);
    camera_pos *= zoom;
  }

  // update mouse position
  mouse_x = x;
  mouse_y = y;

  // Redraw the scene
  glutPostRedisplay();
}

// ========================================================
// Callback function for keyboard events
// ========================================================

void GLCanvas::keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'p':  case 'P':
    if (paused == 0) {
      printf ("pause (press 'p' again to un-pause)\n");
      paused = 1;
    } else {
      printf ("un-pause\n");
      paused = 0;
    }
    break;
  case 'r':  case 'R':
    printf ("restart\n");
    restart();
    break;
  case 's':  case 'S':
    step();
    break;
  case 'q':  case 'Q':
    printf ("quit!\n");
    exit(0);
    break;
  default:
    printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
  }
}

// ========================================================
// Callback function for continuous animation
// ========================================================

void GLCanvas::idle(int value) {
  int refresh_milliseconds = int(1000*refresh);
  glutTimerFunc(refresh_milliseconds,idle,0);
  if (paused) return;
  step();
}

void GLCanvas::step() {
  for (int i = 0; i < parser->getNumSystems(); i++) {
    parser->getSystem(i)->Update(dt); }
  glutPostRedisplay();
}

void GLCanvas::restart() {
  for (int i = 0; i < parser->getNumSystems(); i++) {
    parser->getSystem(i)->Restart(); }
  glutPostRedisplay();
}

// ========================================================
// ========================================================
