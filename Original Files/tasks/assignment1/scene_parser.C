#include <stdio.h>
#include <string.h>

#include "scene_parser.h"
#include "matrix.h"
#include "camera.h" 
#include "material.h"
#include "object3d.h"
#include "group.h" 
#include "sphere.h"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

// ====================================================================
// ====================================================================
// CONSTRUCTOR & DESTRUCTOR

SceneParser::SceneParser(const char* filename) {

  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
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
    } else if (!strcmp(token, "Background")) {
      parseBackground();
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
// ====================================================================

void SceneParser::parseOrthographicCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the camera parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "up"));
  Vec3f up = readVec3f();
  getToken(token); assert (!strcmp(token, "size"));
  float size = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  camera = new OrthographicCamera(center,direction,up,size);
}

void SceneParser::parseBackground() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the background color
  getToken(token); assert (!strcmp(token, "{"));  
  while (1) {
    getToken(token); 
    if (!strcmp(token, "}")) { 
      break;  
    } else if (!strcmp(token, "color")) {
      background_color = readVec3f();
    } else {
      printf ("Unknown token in parseBackground: '%s'\n", token);
      assert(0);
    }
  }
}

// ====================================================================
// ====================================================================

void SceneParser::parseMaterials() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numMaterials"));
  num_materials = readInt();
  materials = new (Material*)[num_materials];
  // read in the objects
  int count = 0;
  while (num_materials > count) {
    getToken(token); 
    if (!strcmp(token, "Material")) {
      materials[count] = parseMaterial();
    } else {
      printf ("Unknown token in parseMaterial: '%s'\n", token); 
      exit(0);
    }
    count++;
  }
  getToken(token); assert (!strcmp(token, "}"));
}  

Material* SceneParser::parseMaterial() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  Vec3f diffuseColor(1,1,1);
  getToken(token); assert (!strcmp(token, "{"));
  while (1) {
    getToken(token); 
    if (!strcmp(token, "diffuseColor")) {
      diffuseColor = readVec3f();
    } else {
      assert (!strcmp(token, "}"));
      break;
    }
  }
  Material *answer = new Material(diffuseColor);
  return answer;
}

// ====================================================================
// ====================================================================

Object3D* SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
  Object3D *answer = NULL;
  if (!strcmp(token, "Group")) {      
    answer = (Object3D*)parseGroup();
  } else if (!strcmp(token, "Sphere")) {      
    answer = (Object3D*)parseSphere();
  } else {
    printf ("Unknown token in parseObject: '%s'\n", token);
    exit(0);
  }
  return answer;
}

// ====================================================================
// ====================================================================

Group* SceneParser::parseGroup() {
  //
  // each group starts with an integer that specifies
  // the number of objects in the group
  //
  // the material index sets the material of all objects which follow,
  // until the next material index (scoping for the materials is very
  // simple, and essentially ignores any tree hierarchy)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));

  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numObjects"));
  int num_objects = readInt();

  Group *answer = new Group(num_objects);

  // read in the objects
  int count = 0;
  while (num_objects > count) {
    getToken(token); 
    if (!strcmp(token, "MaterialIndex")) {
      // change the current material
      int index = readInt();
      assert (index >= 0 && index <= getNumMaterials());
      current_material = getMaterial(index);
    } else {
      Object3D *object = parseObject(token);
      assert (object != NULL);
      answer->addObject(count,object);
      count++;
    }
  }
  getToken(token); assert (!strcmp(token, "}"));
  
  // return the group
  return answer;
}

// ====================================================================
// ====================================================================

Sphere* SceneParser::parseSphere() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "radius"));
  float radius = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  assert (current_material != NULL);
  return new Sphere(center,radius,current_material);
}

// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
    token[0] = '\0';
    return 0;
  }
  return 1;
}


Vec3f SceneParser::readVec3f() {
  float x,y,z;
  int count = fscanf(file,"%f %f %f",&x,&y,&z);
  if (count != 3) {
    printf ("Error trying to read 3 floats to make a Vec3f\n");
    assert (0);
  }
  return Vec3f(x,y,z);
}


Vec2f SceneParser::readVec2f() {
  float u,v;
  int count = fscanf(file,"%f %f",&u,&v);
  if (count != 2) {
    printf ("Error trying to read 2 floats to make a Vec2f\n");
    assert (0);
  }
  return Vec2f(u,v);
}


float SceneParser::readFloat() {
  float answer;
  int count = fscanf(file,"%f",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 float\n");
    assert (0);
  }
  return answer;
}


int SceneParser::readInt() {
  int answer;
  int count = fscanf(file,"%d",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 int\n");
    assert (0);
  }
  return answer;
}

// ====================================================================
// ====================================================================

