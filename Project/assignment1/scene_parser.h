#ifndef _SceneParser_H_
#define _SceneParser_H_

#include "vectors.h"
#include <assert.h>

class Camera;
class Material;
class Object3D;
class Group;
class Sphere;

#define MAX_PARSER_TOKEN_LENGTH 100

// ====================================================================
// ====================================================================

class SceneParser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  SceneParser(const char *filename);
  ~SceneParser();

  // ACCESSORS
  Camera* getCamera() const { return camera; }
  Vec3f getBackgroundColor() const { return background_color; }
  int getNumMaterials() const { return num_materials; }
  Material* getMaterial(int i) const { 
    assert(i >= 0 && i < num_materials); 
    return materials[i]; }  
  Group* getGroup() const { return group; }

private:

  SceneParser() { assert(0); } // don't use

  // PARSING
  void parseFile();
  void parseOrthographicCamera();
  void parseBackground();
  void parseMaterials();
  Material* parseMaterial();

  Object3D* parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
  Group* parseGroup();
  Sphere* parseSphere();

  // HELPER FUNCTIONS
  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  Vec3f readVec3f();
  Vec2f readVec2f();
  float readFloat();
  int readInt();

  // ==============
  // REPRESENTATION
  FILE *file;
  Camera *camera;
  Vec3f background_color;
  int num_materials;
  Material **materials;
  Material *current_material;
  Group *group;
};

// ====================================================================
// ====================================================================

#endif
