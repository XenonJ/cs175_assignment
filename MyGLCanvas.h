#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <time.h>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Special1.h"

#include "Camera.h"
#include "scene/SceneParser.h"


class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	int wireframe;
	int smooth;
	int fill;
	int normal;
	int segmentsX, segmentsY;
    float scale;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
    Special1 *special1;
	Shape* shape;

	Camera* camera;
	SceneParser* parser;

	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void renderShape(OBJ_TYPE type);
	void setSegments();
	void loadSceneFile(const char* filenamePath);
	void setShape(OBJ_TYPE type);
	void resetScene();

private:
	void draw();
	void drawScene();
	void drawObject(OBJ_TYPE type);
	void drawAxis();
	void drawWireframe(SceneNode* node, bool wireframe);

	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);
	void setLight(const SceneLightData &light);
	void applyMaterial(const SceneMaterial &material);
};

#endif // !MYGLCANVAS_H