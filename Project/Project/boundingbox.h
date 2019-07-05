#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "vectors.h"
#include "ray.h"
#include <assert.h>

#define min2(a,b) (((a)<(b))?(a):(b))
#define max2(a,b) (((a)>(b))?(a):(b))

// ====================================================================
// ====================================================================

class BoundingBox {

public:
	float intersect(const Ray * ray, float tmin);

	// CONSTRUCTOR & DESTRUCTOR
	//BoundingBox(Vec3f _min, Vec3f _max) {
	//  Set(_min,_max); }
	BoundingBox(BoundingBox *m) {
		Set(m);
	}

	BoundingBox(Vec3f vec1, Vec3f vec2) {
		Set(vec1, vec2);
	}

	BoundingBox(Vec3f vec1, Vec3f vec2, Vec3f vec3) {
		Set(vec1, vec2);
		Extend(vec3);
	}

	BoundingBox(Vec3f vec1, Vec3f vec2, Vec3f vec3, Vec3f vec4) {
		Set(vec1, vec2);
		Extend(vec3);
		Extend(vec4);
	}


	~BoundingBox() {}

	// ACCESSORS
	void Get(Vec3f &_min, Vec3f &_max) const {
		_min = min;
		_max = max;
	}
	Vec3f getMin() const { return min; }
	Vec3f getMax() const { return max; }

	// MODIFIERS
	void Set(BoundingBox *bb) {
		assert(bb != NULL);
		min = bb->min;
		max = bb->max;
	}
	//void Set(Vec3f _min, Vec3f _max) {
	//  assert (min.x() <= max.x() &&
	   //   min.y() <= max.y() &&
	   //   min.z() <= max.z());
	//  min = _min;
	//  max = _max; }

	void Set(Vec3f v1, Vec3f v2) {
		min = Vec3f(min2(v1.x(), v2.x()),
			min2(v1.y(), v2.y()),
			min2(v1.z(), v2.z()));
		max = Vec3f(max2(v1.x(), v2.x()),
			max2(v1.y(), v2.y()),
			max2(v1.z(), v2.z()));
	}

	void Extend(const Vec3f v) {
		min = Vec3f(min2(min.x(), v.x()),
			min2(min.y(), v.y()),
			min2(min.z(), v.z()));
		max = Vec3f(max2(max.x(), v.x()),
			max2(max.y(), v.y()),
			max2(max.z(), v.z()));
	}
	void Extend(BoundingBox *bb) {
		//assert (bb != NULL);
		if (NULL == bb)
			return;
		Extend(bb->min);
		Extend(bb->max);
	}

	// DEBUGGING 
	void Print() const {
		printf("%f %f %f  -> %f %f %f\n", min.x(), min.y(), min.z(),
			max.x(), max.y(), max.z());
	}
	void paint() const;

private:

	BoundingBox() { assert(0); } // don't use this constructor

	// REPRESENTATION
	Vec3f min;
	Vec3f max;

};

// ====================================================================
// ====================================================================

#endif
