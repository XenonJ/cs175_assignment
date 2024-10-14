#include "Mesh.h"
#include <iostream>

// TODO: same vertex should not be added?
void Mesh::addVertex(Vertex* v) {
    vertices[lastV] = v;
    lastV++;
}

// TODO: not using edges for now
void Mesh::addEdge(Edge* e) {

}

void Mesh::addFace(Face* f) {
    faces[lastF] = f;
    lastF++;
}

Vertex** Mesh::getVertices() {
    return vertices;
}

// TODO: improve speed
Vertex* Mesh::getVertexAt(glm::vec3 pos) {
    for (int i = 0; i < lastV; i++) {
        float epsilon = 1e-6f;
        if ((std::fabs(vertices[i]->getPos().x - pos.x) < epsilon) &&
           (std::fabs(vertices[i]->getPos().y - pos.y) < epsilon) &&
           (std::fabs(vertices[i]->getPos().z - pos.z) < epsilon) ) {
            return vertices[i];
        }
    }
    return nullptr;
}

// TODO: not using edges for now
Edge** Mesh::getEdges() {
    return edges;
}

Face** Mesh::getFaces() {
    return faces;
}

void Mesh::clear() {
    for (int i = 0; i < lastV; i++) {
        delete vertices[i];
    }
    for (int i = 0; i < lastF; i++) {
        delete faces[i];
    }
    delete vertices;
    delete edges;
    delete faces;
}

/*
void Mesh::calculateVertexNormal() {
    for (auto v : vertices) {
        glm::vec3 normal(0.0f, 0.0f, 0.0f);
        std::unordered_map<int, bool> mp;
        for (auto f : v->getFaces()) {
            if (!mp[convertVec3ToInt(f->getFaceNormal())]) {
                normal += f->getFaceNormal();
                mp[convertVec3ToInt(f->getFaceNormal())] = true;
            }
        }
        normal = glm::normalize(normal);
        v->setNormal(normal);
    }
}

// rotate the Mesh and return a new Mesh instance
Mesh* Mesh::rotate(float angle_x, float angle_y, float angle_z) {
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angle_x), glm::vec3(1, 0, 0));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(angle_y), glm::vec3(0, 1, 0));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(angle_z), glm::vec3(0, 0, 1));
    // orders: x -> y -> z
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    return transform(rotationMatrix);
}

Mesh* Mesh::transform(glm::mat4 transformation) {
    Mesh* ret = new Mesh();

    // store the relation between old and new vertex
    std::unordered_map<Vertex*, Vertex*> vertexMap;
    for (auto v : vertices) {
        glm::vec3 newPos = glm::vec3(transformation * glm::vec4(v->getPos(), 1.0f));
        glm::vec3 newNormal = glm::normalize(glm::vec3(transformation * glm::vec4(v->getNormals(), 0.0f)));
        Vertex* newVertex = new Vertex(newPos);
        newVertex->setNormal(newNormal);
        vertexMap[v] = newVertex;
        ret->addVertex(newVertex);
    }

    for (auto e : edges) {
        Edge* newEdge = new Edge(vertexMap[e->getSrc()], vertexMap[e->getDes()]);
        ret->addEdge(newEdge);
    }

    for (auto f : faces) {
        Face* newFace = new Face(vertexMap[f->getVertices()[0]], vertexMap[f->getVertices()[1]], vertexMap[f->getVertices()[2]]);
        ret->addFace(newFace);
        for (int i = 0; i < 3; i++) {
            vertexMap[f->getVertices()[i]]->addFace(newFace);
        }
    }

    return ret;
}

// union the given list of Mesh and return a new Mesh
Mesh* Mesh::union_graph(std::vector<Mesh*>& graphs) {
    Mesh* ret = new Mesh();
    std::unordered_map<int, Vertex*> vertexMap;

    for (auto g : graphs) {
        for (auto v : g->getVertices()) {
            if (vertexMap.find(convertVec3ToInt(v->getPos())) != vertexMap.end()) {
                continue;
            }
            else {
                Vertex* newVertex = new Vertex(v->getPos());
                ret->addVertex(newVertex);
                vertexMap[convertVec3ToInt(v->getPos())] = newVertex;
            }
        }
        for (auto e : g->getEdges()) {
            ret->addEdge(new Edge(vertexMap[convertVec3ToInt(e->getSrc()->getPos())], vertexMap[convertVec3ToInt(e->getDes()->getPos())]));
        }
        for (auto f : g->getFaces()) {
            Face* newFace = new Face(
                vertexMap[convertVec3ToInt(f->getVertices()[0]->getPos())],
                vertexMap[convertVec3ToInt(f->getVertices()[1]->getPos())],
                vertexMap[convertVec3ToInt(f->getVertices()[2]->getPos())]
            );
            ret->addFace(newFace);
            for (int i = 0; i < 3; i++) {
                vertexMap[convertVec3ToInt(f->getVertices()[i]->getPos())]->addFace(newFace);
            }
        }
    }
    ret->calculateVertexNormal();
    return ret;
}

int Mesh::convertVec3ToInt(glm::vec3 vec) {
    const float scale = 10000.0f;
    int scaledX = static_cast<int>(std::round(vec.x * scale));
    int scaledY = static_cast<int>(std::round(vec.y * scale));
    int scaledZ = static_cast<int>(std::round(vec.z * scale));

    return (scaledX * 100000000) + (scaledY * 10000) + scaledZ;
}
*/
