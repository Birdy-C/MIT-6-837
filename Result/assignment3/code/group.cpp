#include "group.h"





Group::Group(int n)
{
	all = n;
	objList = new Object3D *[n];
}

Group::~Group()
{
	delete[] objList;
}

void Group::addObject(int index, Object3D * obj)
{
	objList[index] = obj;
	return;
}

bool Group::intersect(const Ray & r, Hit & h, float tmin)
{
	int flag = false;
	for (int i = 0; i < all; i++)
	{
		if (objList[i]->intersect(r, h, tmin))		flag = true;
	}
	return flag;
}

void Group::paint(void)
{
	for (int i = 0; i < all; i++)
	{
		objList[i]->paint();
	}
}
