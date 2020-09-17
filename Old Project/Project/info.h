#pragma once
#include "scene_parser.h"
#include "glCanvas.h"
#include "grid.h"
extern SceneParser *mainapp;
extern GLCanvas canvas;
extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
extern bool Blinn;
extern int max_bounces;
extern float cutoff_weight;
extern bool shadows;
extern bool shade_back;
extern bool useTransparentShadows;
//extern class Grid;

