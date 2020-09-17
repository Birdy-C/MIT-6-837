#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <assert.h>
#include "vectors.h"

// ====================================================================
// ====================================================================
// Simple image class

class Image {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Image(int w, int h) {
    width = w;
    height = h;
    data = new Vec3f[width*height]; }
  ~Image() {
    delete [] data; }

  // =========
  // ACCESSORS
  int Width() const { return width; }
  int Height() const { return height; }
  const Vec3f& GetPixel(int x, int y) const {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return data[y*width + x]; }
  
  // =========
  // MODIFIERS
  void SetAllPixels(const Vec3f &color) {
    for (int i = 0; i < width*height; i++) {
      data[i] = color; } }
  void SetPixel(int x, int y, const Vec3f &color) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    data[y*width + x] = color; }

  // ===========
  // LOAD & SAVE
  static Image* LoadPPM(const char *filename);
  void SavePPM(const char *filename) const; 
  static Image* LoadTGA(const char *filename);
  void SaveTGA(const char *filename) const; 
  
  // extension for image comparison
  static Image* Compare(Image* img1, Image* img2);
  
private:

  // ==============
  // REPRESENTATION
  int width;
  int height;
  Vec3f *data;

};

// ====================================================================
// ====================================================================

#endif
