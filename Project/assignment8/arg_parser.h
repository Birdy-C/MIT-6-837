#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// =========================================================================
// =========================================================================
// A simple class to organize all of the command line arguments

class ArgParser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  ArgParser(int argc, char *argv[]) {
    DefaultValues();
    ParseArguments(argc,argv); }
  ~ArgParser() {}

private:
  ArgParser() { assert(0); } // don't use this constructor

  // HELPER FUNCTIONS
  void DefaultValues() {
    input_file = NULL;
    output_file = NULL;
    output_bezier_file = NULL;
    output_bspline_file = NULL;
    gui = 0;
    revolution_tessellation = 10;
    curve_tessellation = 10;
    patch_tessellation = 10;
  }

  void ParseArguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i],"-input")) {
	i++; assert (i < argc); 
	input_file = argv[i];
      } else if (!strcmp(argv[i],"-output")) {
	i++; assert (i < argc); 
	output_file = argv[i];
      } else if (!strcmp(argv[i],"-output_bezier")) {
	i++; assert (i < argc); 
	output_bezier_file = argv[i];
      } else if (!strcmp(argv[i],"-output_bspline")) {
	i++; assert (i < argc); 
	output_bspline_file = argv[i];
      } else if (!strcmp(argv[i],"-gui")) {
	gui = 1;
      } else if (!strcmp(argv[i],"-revolution_tessellation")) {
	i++; assert (i < argc); 
	revolution_tessellation = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-curve_tessellation")) {
	i++; assert (i < argc); 
	curve_tessellation = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-patch_tessellation")) {
	i++; assert (i < argc); 
	patch_tessellation = atoi(argv[i]);
      } else {
	printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
	assert(0);
      }
    }
  }

public:
  // ==============
  // REPRESENTATION
  // all public! (no need for accessors)

  char *input_file;
  char *output_file;
  char *output_bezier_file;
  char *output_bspline_file;
  int gui;
  int revolution_tessellation;
  int curve_tessellation;
  int patch_tessellation;
};

// =========================================================================
// =========================================================================

#endif
