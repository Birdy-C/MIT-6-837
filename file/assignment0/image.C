#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "image.h"

// ====================================================================
// ====================================================================
// some helper functions for save & load

unsigned char ReadByte(FILE *file) {  
  unsigned char b;
  int success = fread((void*)&b,sizeof(unsigned char),1,file);
  assert (success == 1);
  return b;
}

void WriteByte(FILE *file, unsigned char b) {
  int success = fwrite((void*)&b,sizeof(unsigned char),1,file);
  assert (success == 1);
}

unsigned char ClampColorComponent(float c) {
  int tmp = int (c*255);
  if (tmp < 0) tmp = 0;
  if (tmp > 255) tmp = 255;
  return (unsigned char)tmp;
}

// ====================================================================
// ====================================================================
// Save and Load data type 2 Targa (.tga) files
// (uncompressed, unmapped RGB images)

void Image::SaveTGA(const char *filename) const {
  assert(filename != NULL);
  // must end in .tga
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".tga"));
  FILE *file = fopen(filename,"wb");
  // misc header information
  for (int i = 0; i < 18; i++) {
    unsigned char tmp;
    if (i == 2) WriteByte(file,2);
    else if (i == 12) WriteByte(file,width%256);
    else if (i == 13) WriteByte(file,width/256);
    else if (i == 14) WriteByte(file,height%256);
    else if (i == 15) WriteByte(file,height/256);
    else if (i == 16) WriteByte(file,24);
    else if (i == 17) WriteByte(file,32);
    else WriteByte(file,0);
  }
  // the data
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Vec3f v = GetPixel(x,y);
      // note reversed order: b, g, r
      WriteByte(file,ClampColorComponent(v.b()));
      WriteByte(file,ClampColorComponent(v.g()));
      WriteByte(file,ClampColorComponent(v.r()));
    }
  }
  fclose(file);
}

Image* Image::LoadTGA(const char *filename) {
  assert(filename != NULL);
  // must end in .tga
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".tga"));
  FILE *file = fopen(filename,"rb");
  // misc header information
  int width = 0;
  int height = 0;
  for (int i = 0; i < 18; i++) {
    unsigned char tmp;
    tmp = ReadByte(file);
    if (i == 2) assert(tmp == 2);
    else if (i == 12) width += tmp;
    else if (i == 13) width += 256*tmp;
    else if (i == 14) height += tmp;
    else if (i == 15) height += 256*tmp;
    else if (i == 16) assert(tmp == 24);
    else if (i == 17) assert(tmp == 32);
    else assert(tmp == 0);
  }
  // the data
  Image *answer = new Image(width,height);
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      unsigned char r,g,b;
      // note reversed order: b, g, r
      b = ReadByte(file);
      g = ReadByte(file);
      r = ReadByte(file);
      Vec3f color(r/255.0,g/255.0,b/255.0);
      answer->SetPixel(x,y,color);
    }
  }
  fclose(file);
  return answer;
}

// ====================================================================
// ====================================================================
// Save and Load PPM image files using magic number 'P6' 
// and having one comment line

void Image::SavePPM(const char *filename) const {
  assert(filename != NULL);
  // must end in .ppm
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".ppm"));
  FILE *file = fopen(filename, "w");
  // misc header information
  assert(file != NULL);
  fprintf (file, "P6\n");
  fprintf (file, "# Creator: Image::SavePPM()\n");
  fprintf (file, "%d %d\n", width,height);
  fprintf (file, "255\n");
  // the data
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x=0; x<width; x++) {
      Vec3f v = GetPixel(x,y);
      fputc (ClampColorComponent(v.r()),file);
      fputc (ClampColorComponent(v.g()),file);
      fputc (ClampColorComponent(v.b()),file);
    }
  }
  fclose(file);
}

Image* Image::LoadPPM(const char *filename) {
  assert(filename != NULL);
  // must end in .ppm
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".ppm"));
  FILE *file = fopen(filename,"rb");
  // misc header information
  int width = 0;
  int height = 0;  
  char tmp[100];
  fgets(tmp,100,file); 
  assert (strstr(tmp,"P6"));
  fgets(tmp,100,file); 
  assert (tmp[0] == '#');
  fgets(tmp,100,file); 
  sscanf(tmp,"%d %d",&width,&height);
  fgets(tmp,100,file); 
  assert (strstr(tmp,"255"));
  // the data
  Image *answer = new Image(width,height);
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      unsigned char r,g,b;
      r = fgetc(file);
      g = fgetc(file);
      b = fgetc(file);
      Vec3f color(r/255.0,g/255.0,b/255.0);
      answer->SetPixel(x,y,color);
    }
  }
  fclose(file);
  return answer;
}

// ====================================================================
// ====================================================================

Image* Image::Compare(Image* img1, Image* img2) {
  assert (img1->Width() == img2->Width());
  assert (img1->Height() == img2->Height());
  
  Image* img3 = new Image(img1->Width(), img1->Height());
  
  
  for (int x = 0; x < img1->Width(); x++) {
    for (int y = 0; y < img1->Height(); y++) {
      Vec3f color1 = img1->GetPixel(x, y);
      Vec3f color2 = img2->GetPixel(x, y);
      Vec3f color3 = Vec3f(fabs(color1.r() - color2.r()),
			   fabs(color1.g() - color2.g()),
			   fabs(color1.b() - color2.b()));
      img3->SetPixel(x, y, color3);
    }
  }
  
  return img3;
}
