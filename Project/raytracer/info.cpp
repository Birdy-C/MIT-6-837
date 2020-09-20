#include "stdafx.h"
#include "info.h"

SceneParser *mainapp;
int theta_steps = 10;
int phi_steps = 5;
bool gouraud = false;
bool Blinn = true;// 使用Blinn-Torrance还是传统的Phong
bool shade_back = true;

int max_bounces = 0;
float cutoff_weight = 0.01;
bool shadows = false;
bool useTransparentShadows = false;
