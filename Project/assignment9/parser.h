#ifndef _PARSER_H_
#define _PARSER_H_

#include <assert.h>
#include "vectors.h"

#define MAX_PARSER_TOKEN_LENGTH 100

class System;
class Generator;
class Integrator;
class ForceField;

// ====================================================================
// ====================================================================

// parse the particle system file

class Parser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Parser(const char *file);
  ~Parser();

  // ACCESSORS
  int getNumSystems() { return num_systems; }
  System* getSystem(int i) {
    assert (i >= 0 && i < num_systems);
    return systems[i]; }

private:
  // don't use this constructor
  Parser() { assert(0); } 

  // HELPER FUNCTIONS
  System* ParseSystem();
  Generator* ParseGenerator();
  Integrator* ParseIntegrator();
  ForceField* ParseForceField();

  // UTILITY FUNCTIONS
  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  Vec3f readVec3f();
  Vec2f readVec2f();
  float readFloat();
  int readInt();

  // REPRESENTATION
  int num_systems;
  System **systems;
  FILE *file;
};

// ====================================================================
// ====================================================================


#endif
