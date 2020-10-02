#include <stdio.h>
#include <assert.h>
using namespace std;

#include "glCanvas.h"
#include "parser.h"

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  // command line argument defaults
  const char *filename = NULL;
  float refresh = 0.1;  // in seconds
  float dt = 0.1;       // in seconds
  int integrator_color = 0;
  int draw_vectors = 0;
  float acceleration_scale = 1;
  int motion_blur = 0;
 
  // parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i],"-input")) {
      i++; assert (i < argc);      
      filename = argv[i];
    } else if (!strcmp(argv[i],"-refresh")) {
      i++; assert (i < argc);      
      refresh = atof(argv[i]);
    } else if (!strcmp(argv[i],"-dt")) {
      i++; assert (i < argc);      
      dt = atof(argv[i]);
    } else if (!strcmp(argv[i],"-integrator_color")) {
      integrator_color = 1;
    } else if (!strcmp(argv[i],"-motion_blur")) {
      motion_blur = 1;
    } else if (!strcmp(argv[i],"-draw_vectors")) {
      draw_vectors = 1;
      i++; assert (i < argc);      
      acceleration_scale = atof(argv[i]);
    } else {
      printf ("WARNING:  unknown command line argument %s\n", argv[i]);
      assert(0);
    }
  }

  // load the particle system
  assert (filename != NULL);
  Parser *parser = new Parser(argv[2]);
  
  // launch viewer!   and it never returns...
  GLCanvas glcanvas;
  glcanvas.initialize(parser,refresh,dt,integrator_color,
		      draw_vectors,acceleration_scale,motion_blur);
  return 0;
}

// ====================================================================
// ====================================================================



