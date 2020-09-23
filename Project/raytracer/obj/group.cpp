#include "stdafx.h"
#include "group.h"


Group::Group(int n)
{
	all = n;
	objList = new Object3D *[n];
    for (int i = 0; i < n; i++)
    {
        objList[i] = nullptr;
    }
}

Group::~Group()
{
	delete[] objList;
    if (itsboundingbox)
        delete itsboundingbox;
}

void Group::addObject(int index, Object3D * obj)
{
    // TODO5
    bool changeboundingbox = obj->getBoundingBox();
    if (objList[index])
    {
        objList[index] = obj;
        // update the whole bounding box according to list
        if (itsboundingbox) 
            delete itsboundingbox;
        itsboundingbox = nullptr;

        for (int i = 0; i < all; i++)
        {
            if (!objList[i] || !objList[i]->getBoundingBox()) continue;
            if (!itsboundingbox)
            {
                itsboundingbox = new BoundingBox(*(objList[i]->getBoundingBox()));
            }
            else
            {
                itsboundingbox->Extend(objList[i]->getBoundingBox());
            }
        }
    }
    else
    {
        objList[index] = obj;
        if (changeboundingbox)
        {
            if (!itsboundingbox)
            {
                itsboundingbox = new BoundingBox(*(obj->getBoundingBox()));
            }
            else
            {
                itsboundingbox->Extend(obj->getBoundingBox());
            }
        }
    }
	return;
}

bool Group::intersect(const Ray & r, Hit & h, float tmin)
{
    // TODO1
	int flag = false;
	for (int i = 0; i < all; i++)
	{
		if (objList[i]->intersect(r, h, tmin))
            flag = true;
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

void Group::insertIntoGrid(Grid * g, Matrix * m)
{
    for (int i = 0; i < all; i++)
    {
        objList[i]->insertIntoGrid(g, m);
    }
}
