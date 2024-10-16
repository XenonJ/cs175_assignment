#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
	Cube() { calculate(); };
	~Cube() {};

	OBJ_TYPE getType() {
		return SHAPE_CUBE;
	}
	
	std::vector<Vertex> vertices;
	std::vector<Face>   Faces;

	void draw();


	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();
	
	void drawNormalForSingleFace();

private:
	
};

#endif