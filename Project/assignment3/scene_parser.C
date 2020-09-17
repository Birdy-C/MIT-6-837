#include <stdio.h>
#include <string.h>

#include "scene_parser.h"
#include "matrix.h"
#include "camera.h" 
#include "light.h"
#include "material.h"
#include "object3d.h"
#include "group.h" 
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "transform.h"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

// ====================================================================
// ====================================================================
// CONSTRUCTOR & DESTRUCTOR

SceneParser::SceneParser(const char* filename) {

  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
  ambient_light = Vec3f(0,0,0);
  num_lights = 0;
  lights = NULL;
  num_materials = 0;
  materials = NULL;
  current_material = NULL;

  // parse the file
  assert(filename != NULL);
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".txt"));
  file = fopen(filename,"r");
  assert (file != NULL);
  parseFile();
  fclose(file); 
  file = NULL;

  // if no lights are specified, set ambient light to white
  // (do solid color ray casting)
  if (num_lights == 0) {
    printf ("WARNING:  No lights specified\n");
    ambient_light = Vec3f(1,1,1);
  }
}

SceneParser::~SceneParser() {
  if (group != NULL) 
    delete group;
  if (camera != NULL) 
    delete camera;
  int i;
  for (i = 0; i < num_materials; i++) {
    delete materials[i]; }
  delete [] materials;
  for (i = 0; i < num_lights; i++) {
    delete lights[i]; }
  delete [] lights;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
  //
  // at the top level, the scene can have a camera, 
  // background color and a group of objects
  // (we add lights and other things in future assignments)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];    
  while (getToken(token)) { 
    if (!strcmp(token, "OrthographicCamera")) {
      parseOrthographicCamera();
    } else if (!strcmp(token, "PerspectiveCamera")) {
      parsePerspectiveCamera();
    } else if (!strcmp(token, "Background")) {
      parseBackground();
    } else if (!strcmp(token, "Lights")) {
      parseLights();
    } else if (!strcmp(token, "Materials")) {
      parseMaterials();
    } else if (!strcmp(token, "Group")) {
      group = parseGroup();
    } else {
      printf ("Unknown token in parseFile: '%s'\n", token);
      exit(0);
    }
  }
}

// ====================================================================
// =============================