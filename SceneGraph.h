#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include <FL/gl.h>
#include <FL/glu.h>
#include <vector>
#include "scene/SceneParser.h"

class SceneGraphNode {
private:
    glm::mat4 transformationMat;
    Shape* shape;
    SceneMaterial material;
public:
    SceneGraphNode(glm::mat4 mat, Shape* sp, SceneMaterial material) { this->transformationMat = mat; this->shape = sp; this->material = material; };
    ~SceneGraphNode() { delete this->shape; };
    void setTransformation(glm::mat4 mat) { this->transformationMat = mat; };
    void draw() { glMultMatrixf(glm::value_ptr(this->transformationMat)); this->shape->draw(); };
    SceneMaterial getMaterial() { return this->material; };
    void setSegments(int segX, int segY) { this->shape->setSegments(segX, segY); };
    void calculate() { this->shape->calculate(); };
};

class SceneGraph {
private:
    std::vector<SceneGraphNode*> list;
public:
    SceneGraph() { };
    ~SceneGraph() { clear(); };
    void addNode(SceneGraphNode* node) { this->list.push_back(node); };
    void clear() { for (auto node : this->list) { delete node; }; this->list.clear(); };
    std::vector<SceneGraphNode*>::iterator getIterator() { return this->list.begin(); };
    std::vector<SceneGraphNode*>::iterator getEnd() { return this->list.end(); };
    void calculate() { for (auto node : this->list) { node->calculate(); } };
};

#endif