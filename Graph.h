#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

class Vertex;
class Edge;
class Face;

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::size_t h1 = std::hash<float>()(v.x);
        std::size_t h2 = std::hash<float>()(v.y);
        std::size_t h3 = std::hash<float>()(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine the hash values
    }
};

struct Vec3Equal {
    bool operator()(const glm::vec3& v1, const glm::vec3& v2) const {
        return v1 == v2; // Use glm's operator==
    }
};

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
};

#endif
