#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() { calculate(); };
	~Sphere() {};

	OBJ_TYPE getType() {
		return SHAPE_SPHERE;
	}

	void draw();

	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();

	void drawNormalsFromFaces();
private:
};

#endif