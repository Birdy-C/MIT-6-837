#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>

#include "ifs.h"

int main(int argc, char *argv[])
{
	// ========================================================
	// ========================================================
	// Some sample code you might like to use for parsing 
	// command line arguments and the input IFS files

	// sample command line:
	// ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga

	char *input_file = NULL;
	int num_points = 10000;
	int num_iters = 10;
	int size = 100;
	char *output_file = NULL;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-points")) {
			i++; assert(i < argc);
			num_points = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-iters")) {
			i++; assert(i < argc);
			num_iters = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			size = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	// ========================================================
	// ========================================================
	// Some sample code you might like to use for
	// parsing the IFS transformation files

	Image img(size, size);
	//Ĭ�ϰ׵׺�ͼ
	img.SetAllPixels(Vec3f(255, 255, 255));
	IFS mainIFS(input_file);

	mainIFS.render(img, num_points, num_iters);
	img.SaveTGA(output_file);
	// ========================================================
	// ========================================================
}