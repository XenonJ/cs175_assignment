#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	OBJ_TYPE getType() {
		return SHAPE_CYLINDER;
	}
	
	std::vector<Vertex> vertices;
	std::vector<Face>   Faces;

	void draw();

	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();


private:
};

#endif