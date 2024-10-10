#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shape.h>
#include <FL/gl.h>
#include <FL/glu.h>
#include <vector>

class SceneNode {
private:
    glm::mat4 transformationMat;
    Shape* shape;
public:
    SceneNode(glm::mat4 mat, Shape* sp) { this->transformationMat = mat; this->shape = sp; };
    ~SceneNode() { delete this->shape; };
    void setTransformation(glm::mat4 mat) { this->transformationMat = mat; };
    void draw() { glMultMatrixf(glm::value_ptr(this->transformationMat)); this->shape->draw(); };
};

class SceneGraph {
private:
    std::vector<SceneNode*> list;
public:
    SceneGraph() { };
    ~SceneGraph() { clear(); };
    void addNode(SceneNode* node) { this->list.push_back(node); };
    void draw() { for (auto node : this->list) { node->draw(); } };
    void clear() { for (auto node : this->list) { delete node; } };
};

#endif