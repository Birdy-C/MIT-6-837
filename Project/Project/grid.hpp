#pragma once
#include "object3d.h"
#include "boundingbox.h"

struct MarchingInfo
{

};


class Grid 
{
public:

	Grid(BoundingBox *bb, int nx, int ny, int nz);
	~Grid();

	void setRecord(int x, int y, int z);
	Vec3f center(int x, int y, int z);
	void calculatebox();
	bool intersect(const Ray &r, Hit &h, float tmin);
	void paint(void);
	const int nx, ny, nz;
	BoundingBox* getBoundingBox();
private:
	void drawface(int i, int j, int k, int type);
	BoundingBox *boundingbox;
	bool *record;

	bool *getRecord(int x, int y, int z);
	void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
};

