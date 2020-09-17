#pragma once
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "matrix.h"
#include "vectors.h"
#include "image.h"


//a field to store n, the number of transformations
//an array of matrices representing the n transformations
//an array of the corresponding probabilities for choosing a transformation
//a constructor that creates an IFS
//an input method that reads the IFS description
//a render method that takes as input an image instance, a number of points and a number of iterations
//a destructor that frees the memory of the various arrays(using delete)

class IFS
{
	int num;
	std::vector <Matrix>  transformation;
	std::vector <float>  p;

public:
	IFS();
	IFS(char *file);
	void readFile(char *file);
	void render(Image &img, int pointNum,int iter);

};