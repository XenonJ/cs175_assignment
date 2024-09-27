#include "Graph.h"

void Graph::addVertex(Vertex* v) {
    vertices.push_back(v);
}

void Graph::addEdge(Edge* e) {
    edges.push_back(e);
}

void Graph::addFace(Face* f) {
    faces.push_back(f);
    Vertex* const* vertices = f->getVertices();
    for (int i = 0; i < 3; i++) {
        vertices[i]->addFace(f);
    }
}

std::vector<Vertex*>& Graph::getVertices() {
    return vertices;
}

Vertex* Graph::getVertexAt(glm::vec3 pos) {
    for (auto v : vertices) {
        if (v->getPos() == pos) {
            return v;
        }
    }
}

std::vector<Edge*>& Graph::getEdges() {
    return edges;
}

std::vector<Face*>& Graph::getFaces() {
    return faces;
}

void Graph::clear() {
    for (auto v : vertices) {
        delete v;
    }
    for (auto e : edges) {
        delete e;
    }
    for (auto f : faces) {
        delete f;
    }
    vertices.clear();
    edges.clear();
    faces.clear();
}

void Graph::calculateVertexNormal() {
    for (auto v : vertices) {
        glm::vec3 normal(0.0f, 0.0f, 0.0f);
        for (auto f : v->getFaces()) {
            normal += f->getFaceNormal();
        }
        normal /= v->getFaces().size();
        v->setNormal(normal);
    }
}

Graph Graph::rotate(float angle_x, float angle_y, float angle_z) {

}

Graph Graph::union_graph(std::vector<Graph> graphs) {

}
