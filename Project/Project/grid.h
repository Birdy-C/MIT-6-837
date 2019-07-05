#pragma once
#include "object3d.h"
#include "boundingbox.h"

// store the information for the current ray and the current grid cell
struct MarchingInfo
{
	float tmin;
	int i, j, k;  // current grid cell
	int t_next_x, t_next_y, t_next_z; // the next values of intersection along each axis
	float d_tx, d_ty, d_tz; // the marching increments along each axis
	bool sign_x, sign_y, sign_z;
	void nextCell() {

	}
};


class Grid  :
	public Object3D
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
	float gethalfdiag() { 
		return halfdiag; 
	}
	void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const; 
private:

	void drawface(int i, int j, int k, int type);
	BoundingBox *boundingbox;
	bool *record;
	float halfdiag;

	bool *getRecord(int x, int y, int z);
};

