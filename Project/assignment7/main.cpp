// @author Birdy 2018/5/7
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
#include <algorithm>
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
#include "info.h"
#include "raytracer.h"
#include "raytracing_stats.h"
#include "film.h"
#include "sampler.h"

Vec3f setcolor(int depth_min, int depth_max, float depth)
{
	float t = ((float)depth_max - depth) / (depth_max - depth_min);
	return Vec3f(t, t, t);
}

Grid *maingrid = nullptr;
char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool gui = false;
GLCanvas canvas;
bool tracer_status = false;
int nx = 1, ny = 1, nz = 1;
char *sample_file = NULL;
int sample_zoom = 20;
int num_samples = 1;
Sampler *sampler = NULL;

void render(void)
{
    if (tracer_status)
        RayTracingStats::Initialize(width, height, mainapp->getGroup()->getBoundingBox(), nx, ny, nz);

	Image img(width, height);
	Image depthimg(width, height);
	Image normalimg(width, height);

	img.SetAllPixels(mainapp->getBackgroundColor());
	depthimg.SetAllPixels(Vec3f(0, 0, 0));

	Camera *cameraAll = mainapp->getCamera();
	RayTracer raytracer(maingrid);
    Film film(width, height, num_samples);

    RandomSampler rnd;
	for (int i = 0; i < width; i++)
	{
        for (int j = 0; j < height; j++)
        {
            for (int u = 0; u < num_samples; u++)
            {
                Vec2f offset = sampler->getSamplePosition(u);
                Vec2f point((float)(i + offset.x()) / width, (float)(j + offset.y()) / height);
                Ray rayTemp = cameraAll->generateRay(point);
                Hit hit_result;
                Vec3f Color = raytracer.traceRay(rayTemp, cameraAll->getTMin(), 0, 0, 1, hit_result);

                film.setSample(i, j, u, offset, Color);
                img.SetPixel(height - j - 1, i, Color);
                depthimg.SetPixel(height - j - 1, i, setcolor(depth_min, depth_max, hit_result.getT()));
                normalimg.SetPixel(height - j - 1, i, Vec3f(abs(hit_result.getNormal().x()), abs(hit_result.getNormal().y()), abs(hit_result.getNormal().z())));
            }
        }
	}
	if (output_file)
		img.SaveTGA(output_file);
	if (depth_file)
		depthimg.SaveTGA(depth_file);
	if (normal_file)
		normalimg.SaveTGA(normal_file);
    if (tracer_status)
        RayTracingStats::PrintStatistics();
    if (sample_file)
        film.renderSamples(sample_file, sample_zoom);
}

void traceRay(float x, float y)
{

	Camera *cameraAll = mainapp->getCamera();
	RayTracer raytracer(maingrid);

	Vec2f point(y, 1 - x);
	Ray rayTemp = cameraAll->generateRay(point);
	Hit hit_result;
	Vec3f Color = raytracer.traceRay(rayTemp, cameraAll->getTMin(), 0, cutoff_weight, 0, hit_result);

}

int main(int argc, char *argv[])
{
	// ========================================================
	// ========================================================
	// Some sample code you might like to use for parsing 
	// command line arguments 

	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
    bool usegrid = false;

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
			shade_back = false;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			theta_steps = atof(argv[i]);
			i++; assert(i < argc);
			phi_steps = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-shadows")) {
			shadows = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++; assert(i < argc);
			max_bounces = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++; assert(i < argc);
			cutoff_weight = atof(argv[i]);
		}
        else if (!strcmp(argv[i], "-grid")) {
            usegrid = true;
            i++; assert(i < argc);
            nx = atof(argv[i]);
            i++; assert(i < argc);
            ny = atof(argv[i]);
            i++; assert(i < argc);
            nz = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-visualize_grid")) {
            visualize_grid = true;
        }
        else if (!strcmp(argv[i], "-stats")) {
            tracer_status = true;
        }
        else if (!strcmp(argv[i], "-render_samples")) {
            i++; assert(i < argc);
            sample_file = argv[i];
            i++; assert(i < argc);
            sample_zoom = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-random_samples")) {
            i++; assert(i < argc);
            num_samples = atof(argv[i]);
            sampler = new RandomSampler(); 
        }
        else if (!strcmp(argv[i], "-uniform_samples")) {
            i++; assert(i < argc);
            num_samples = atof(argv[i]);
            sampler = new UniformSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-jittered_samples")) {
            i++; assert(i < argc);
            num_samples = atof(argv[i]);
            sampler = new JitteredSampler(num_samples);
        }
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	mainapp = new SceneParser(input_file);
    if (usegrid)
    {
        maingrid = new Grid(mainapp->getGroup()->getBoundingBox(), nx, ny, nz);
        mainapp->getGroup()->insertIntoGrid(maingrid, nullptr);
    }
	if (gui)
	{
		canvas.initialize(mainapp, render, traceRay, maingrid, visualize_grid);
		delete mainapp;
	}
	else
	{
		render();
	}
    if(maingrid)
        delete maingrid;
    delete mainapp;
	return 0;
}