// @author Birdy 2018/5/7
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
#include <algorithm>
//#include "ifs.h"
#include "scene_parser.h"
#include "image.h"
#include "matrix.h"
#include "camera.h" 
#include "material.h"
#include "object3d.h"
#include "group.h" 
#include "sphere.h"
#include "light.h"
#include "glCanvas.h"

Vec3f setcolor(int depth_min, int depth_max, float depth)
{
	float t = ((float)depth_max - depth) / (depth_max - depth_min);
	return Vec3f(t, t, t);
}

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;
bool gui = false;
SceneParser *mainapp;
GLCanvas canvas;



void render(void)
{
	Image img(width, height);
	Image depthimg(width, height);
	Image normalimg(width, height);
	//canvas.initialize(&mainapp,);// TODO add init
	//Ĭ�ϰ׵׺�ͼ
	img.SetAllPixels(mainapp->getBackgroundColor());
	depthimg.SetAllPixels(Vec3f(0, 0, 0));
	Group *itemAll = mainapp->getGroup();
	Camera *cameraAll = mainapp->getCamera();
	//IFS mainIFS(input_file);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Vec2f point((float)i / width, (float)j / height);
			//Vec2f point(0.5,0.5);
			Ray rayTemp = cameraAll->generateRay(point);
			Hit result;
			if (itemAll->intersect(rayTemp, result, cameraAll->getTMin()))
			{
				assert(NULL != result.getMaterial());
				Vec3f LightAll = mainapp->getAmbientLight(); // add Ambient Light Out the material

				for (int lighti = 0; lighti < mainapp->getNumLights(); lighti++)
				{
					Light *light = mainapp->getLight(lighti);
					Vec3f p, dir, col;
					float distance;
					light->getIllumination(p, dir, col, distance);
					if (shade_back)
						LightAll += abs(dir.Dot3(result.getNormal()))*col;
					else
						LightAll += max(dir.Dot3(result.getNormal()), (float)0)*col;
				}
				img.SetPixel(height - j - 1, width - i - 1, (result.getMaterial())->getDiffuseColor()*LightAll);
				depthimg.SetPixel(height - j - 1, width - i - 1, setcolor(depth_min, depth_max, result.getT()));
				normalimg.SetPixel(height - j - 1, width - i - 1, Vec3f(abs(result.getNormal().x()), abs(result.getNormal().y()), abs(result.getNormal().z())));
			}
		}
	}
	if (output_file)
		img.SaveTGA(output_file);
	if (depth_file)
		depthimg.SaveTGA(depth_file);
	if (normal_file)
		normalimg.SaveTGA(normal_file);

}

int main(int argc, char *argv[])
{
	// ========================================================
	// ========================================================
	// Some sample code you might like to use for parsing 
	// command line arguments 

	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")) {
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	FILE * file = fopen("scene3_01_cube_orthographic.txt", "r");
	//fprintf(stderr, "%s \n", GetLastError());
	if (gui)
	{
		mainapp = new SceneParser(input_file);
		canvas.initialize(mainapp, render);
		delete mainapp;
	}
	else
	{
		render();
	}
	return 0;
}