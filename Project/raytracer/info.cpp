#include "stdafx.h"
#include "info.h"

SceneParser *mainapp;
int theta_steps = 10;
int phi_steps = 5;
bool gouraud = false;
bool Blinn = true;// ʹ��Blinn-Torrance���Ǵ�ͳ��Phong
bool shade_back = true;

int max_bounces = 0;
float cutoff_weight = 0.01;
bool shadows = false;
bool useTransparentShadows = false;
