#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <iostream>

class Vertex;
class Edge;
class Face;

class Vertex {
private:
    glm::vec3 position;
    glm::vec3 normal;
    std::vector<Face*> faces;
public:
    Vertex(glm::vec3 pos) { position = pos; };
    ~Vertex() { };
    glm::vec3 getPos() { return position; };
    void setNormal(glm::vec3 norm) { normal = norm; };
    glm::vec3 getNormals() { return normal; };
    void addFace(Face* f) { faces.push_back(f); };
    std::vector<Face*> getFaces() { return faces; };
};

class Edge {
private:
    Vertex* src;
    Vertex* des;
    std::vector<Face*> faces;
public:
    Edge(Vertex* src, Vertex* des) { this->src = src; this->des = des; };
    ~Edge() { };
    Vertex* getSrc() { return src; };
    Vertex* getDes() { return des; };
    void addFace(Face* face) { faces.push_back(face); };
    std::vector<Face*>& getFaces() { return faces; };
};

class Face {
private:
    Vertex* vertices[3];;
    glm::vec3 faceNormal;
public:
    Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3) {
        vertices[0] = vertex1;
        vertices[1] = vertex2;
        vertices[2] = vertex3;
        glm::vec3 v1 = vertex2->getPos() - vertex1->getPos();
        glm::vec3 v2 = vertex3->getPos() - vertex2->getPos();
        faceNormal = glm::normalize(glm::cross(v1, v2));
    };
    ~Face() { };
    Vertex* const* getVertices() { return vertices; };
    glm::vec3 getFaceNormal() { return faceNormal; };
};

class Graph {
private:
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    std::vector<Face*> faces;
public:
    Graph() { };
    ~Graph() { clear(); };
    void addVertex(Vertex* v);
    void addEdge(Edge* e);
    void addFace(Face* f);
    std::vector<Vertex*>& getVertices();
    Vertex* getVertexAt(glm::vec3 pos);
    std::vector<Edge*>& getEdges();
    std::vector<Face*>& getFaces();
    void clear();
    void calculateVertexNormal();
    Graph* rotate(float angle_x, float angle_y, float angle_z);
    Graph* transform(glm::mat4 transformation);
    static Graph* union_graph(std::vector<Graph*>& graphs);
    static int convertVec3ToInt(glm::vec3 vec);
    // void drawTriangleMeshFromFaces();
    // void drawNormalsFromFaces();
};

#endif
