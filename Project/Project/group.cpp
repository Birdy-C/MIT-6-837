#include "group.h"


void Group::calculatebox()
{
	// for group, when initialized there is still no infomation stored in the class
	// so I implement the calculation in add Object
	//boundingbox = NULL;
	return;
}

Group::Group(int n)
{
	calculatebox();
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


	// Attention: Don't f overlap the setted obj
	if (NULL == boundingbox)
	{
		boundingbox = new BoundingBox(obj->getBoundingBox());
	}
	else
	{
		boundingbox->Extend(obj->getBoundingBox());
	}
	return;
}

bool Group::intersect(const Ray & r, Hit & h, float tmin)
{
	bool flag = false;
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


void Group::insertIntoGrid(Grid *g, Matrix *m)
{
	for (int i = 0; i < all; i++)
	{
		objList[i]->insertIntoGrid(g, m);
	}

}