#ifndef _OBJECT_3D_VECTOR_H_
#define _OBJECT_3D_VECTOR_H_

#include "assert.h"
class Object3D;

typedef Object3D* Object3DPtr;

// ====================================================================
// ====================================================================

// Store an collection of Object3D.  The total number to be stored is
// not known initially.  Resize as necessary.

// ====================================================================
// ====================================================================

class Object3DVector {

public:

  // CONSTRUCTOR
  Object3DVector() {
    num_objects = 0;
    size = 10;
    objects = new (Object3D*)[size];
    for (int i = 0; i < size; i++)
      objects[i] = NULL;
  }

  // DESTRUCTOR
  ~Object3DVector() {
    // don't delete the objects, just the array to store the pointers
    delete [] objects;
  }

  // ACCESSORS
  int getNumObjects() { return num_objects; }
  Object3D* getObject(int i) { 
    assert (i >= 0 && i < num_objects);
    assert (objects[i] != NULL);
    return objects[i];
  }

  // MODIFIERS
  void addObject(Object3D *o) {
    assert (o != NULL);
    if (size == num_objects) {
      // double the size of the array and copy the pointers
      int new_size = size * 2;
      Object3D **new_objects = new (Object3D*)[new_size];
      int i;
      for (i = 0; i < size; i++) {
	new_objects[i] = objects[i];
      }
      for (i = size; i < 2*size; i++) {
	new_objects[i] = NULL;
      }
      delete [] objects;
      objects = new_objects;
      size = new_size;
    }
    objects[num_objects] = o;
    num_objects++;
  }

private:

  // REPRESENTATION
  Object3D **objects;
  int size;
  int num_objects;

};

// ====================================================================
// ====================================================================

#endif
