#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <glm/glm.hpp>

class Vertex {
private:
    glm::vec3 position;
    glm::vec3 normal;
public:
    Vertex(glm::vec3 pos) : position(pos) { };
    ~Vertex() { };
    glm::vec3 getPos() const { return position; };
    void setNormal(glm::vec3 norm) { normal = norm; };
    const glm::vec3 getNormals() const { return normal; };
};

class Edge {
private:
    Vertex* src;
    Vertex* des;
    std::vector<Face*> faces;
public:
    Edge(Vertex* src, Vertex* des) : src(src), des(des) { };
    ~Edge() { };
    Vertex* getSrc() const { return src; };
    Vertex* getDes() const { return des; };
    void addFace(Face* face) { faces.push_back(face); };
    const std::vector<Face*>& getFaces() const { return faces; };
};

class Face {
private:
    Vertex* vertices[3];;
    glm::vec3 faceNormal;
public:
    Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3) 
        : vertices({vertex1, vertex2, vertex3}) {
        // TODO: calculate faceNormal
    };
    ~Face() { };
    const Vertex* const* getVertices() const { 
        return vertices; 
    };
    glm::vec3 getFaceNormal() const { return faceNormal; };
};

class Graph {
private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;
public:
    Graph() { };
    ~Graph() { };
    void addVertex(Vertex v) { };
    void addEdge(Edge e) { };
    void addFace(Face f) { };
    void getVertices() { };
    void getEdges() { };
    void getFaces() { };
    void clear() { };
};

#endif