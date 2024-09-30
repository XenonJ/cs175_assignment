#ifndef SPECIAL1_H
#define SPECIAL1_H

#include "Shape.h"

class Special1 : public Shape {
public:
	Special1() {};
	~Special1() {};

	OBJ_TYPE getType() {
		return SHAPE_SPECIAL1;
	}

	void draw();

	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();

	void drawNormalsFromFaces();


private:
};

#endif