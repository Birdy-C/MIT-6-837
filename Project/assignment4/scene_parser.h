#ifndef _SceneParser_H_
#define _SceneParser_H_

#include "vectors.h"
#include <assert.h>

class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;

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
  Vec3f getAmbientLight() const { return ambient_light; }
  int getNumLights() const { return num_lights; }
  Light* getLight(int i) const { 
    assert(i >= 0 && i < num_lights); 
    return lights[i]; }  
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
  void parsePerspectiveCamera();
  void parseBackground();
  void parseLights();
  Light* parseDirectionalLight();
  Light* parsePointLight();
  void parseMaterials();
  Material* parsePhongMaterial();

  Object3D* parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
  Group* parseGroup();
  Sphere* parseSphere();
  Plane* parsePlane();
  Triangle* parseTriangle();
  Group* parseTriangleMesh();
  Transform* parseTransform();

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
  Vec3f ambient_light;
  int num_lights;
  Light **lights;
  int num_materials;
  Material **materials;
  Material *current_material;
  Group *group;
};

// ====================================================================
// ====================================================================

#endif
