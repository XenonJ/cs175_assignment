#include "Cube.h"
#include "Graph.h"
#include <iostream>

// using namespace std;

std::vector<Vertex> vertices;
std::vector<Face>   Faces;

void drawTriangleMeshFromFaces() {
    glBegin(GL_TRIANGLES);
    for (Face &face : Faces) {
        Vertex *const *verts = face.getVertices();

        for (int i = 0; i < 3; ++i) {
            const glm::vec3 &pos = verts[i]->getPos();
            glVertex3f(pos.x, pos.y, pos.z);
        }
    }
    glEnd();
}

void drawNormalsFromFaces() {

    glColor3f(1.0f, .0f, .0f);
    glBegin(GL_LINES);
    for (Face &face : Faces) {
        Vertex *const *verts      = face.getVertices();
        glm::vec3      v0Pos      = verts[0]->getPos();
        glm::vec3      v1Pos      = verts[1]->getPos();
        glm::vec3      v2Pos      = verts[2]->getPos();
        glm::vec3      v1v0       = glm::normalize(v1Pos - v0Pos);
        glm::vec3      v2v0       = glm::normalize(v2Pos - v0Pos);
        glm::vec3      faceNormal = glm::normalize(glm::cross(v1v0, v2v0));

        for (int i = 0; i < 3; ++i) {
            const glm::vec3 &pos = (verts[i]->getPos());
            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + faceNormal.x * .1f, pos.y + faceNormal.y * .1f,
                       pos.z + faceNormal.z * .1f);
        }
    }
    glEnd();
}

void Cube::draw() {
    // std::cout << "This function was called!" << std::endl;
    // printf("This function was called!");
    // calculate();
    // printf("%\n", vertices.size());
    // printf("%\n", Faces.size());

    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.5f);

    drawTriangleMeshFromFaces();

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);   // Move to Y=1
    glRotatef(-90, 1.0f, 0.0f, 0.0f); // rotate by X -90
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);  // Move to Z=-1
    glRotatef(-90, 0.0f, 1.0f, 0.0f); // rotate by Y -90
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);  // Move to X=1
    glRotatef(90, 0.0f, 1.0f, 0.0f); // rotate by Y 90
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f); // Move to Z=-1
    glRotatef(90, 1.0f, 0.0f, 0.0f); // rotate by X 90
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);   // Move to Y=1
    glTranslatef(0.0f, 0.0f, -1.0f);  // Move to Z=-1
    glRotatef(180, 1.0f, 0.0f, 0.0f); // rotate by X 180
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPopMatrix();
}

void Cube::drawNormal() {

    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.5f);

    drawNormalsFromFaces();

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);   // Move to Y=1
    glRotatef(-90, 1.0f, 0.0f, 0.0f); // rotate by X -90
    drawNormalsFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);  // Move to Z=-1
    glRotatef(-90, 0.0f, 1.0f, 0.0f); // rotate by Y -90
    drawNormalsFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);  // Move to X=1
    glRotatef(90, 0.0f, 1.0f, 0.0f); // rotate by Y 90
    drawNormalsFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f); // Move to Z=-1
    glRotatef(90, 1.0f, 0.0f, 0.0f); // rotate by X 90
    drawNormalsFromFaces();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);   // Move to Y=1
    glTranslatef(0.0f, 0.0f, -1.0f);  // Move to Z=-1
    glRotatef(180, 1.0f, 0.0f, 0.0f); // rotate by X 180
    drawNormalsFromFaces();
    glPopMatrix();

    glPopMatrix();
}

void Cube::calculate() {

    // for (int n = 0; n < 6; n++){

    // graphs[n].clear();
    float stepX = 1.0f / m_segmentsX;
    float stepY = 1.0f / m_segmentsY;

    vertices.clear();
    Faces.clear();

    // Calculate all vertices
    for (int i = 0; i <= m_segmentsX; i++) {
        for (int j = 0; j <= m_segmentsY; j++) {
            float x = i * stepX;
            float y = j * stepY;
            float z = 0.0f;

            // Vertex vertex = new Vertex(glm::vec3(x, y, z));
            vertices.emplace_back(glm::vec3(x, y, z));
        }
    }

    // Calculate all faces
    for (int i = 0; i < m_segmentsX; i++) {
        for (int j = 0; j < m_segmentsY; j++) {
            int index1 = i * (m_segmentsY + 1) + j;
            int index2 = index1 + 1;
            int index3 = (i + 1) * (m_segmentsY + 1) + j;
            int index4 = index3 + 1;

            Faces.emplace_back(&vertices[index1], &vertices[index3],
                               &vertices[index4]);
            Faces.emplace_back(&vertices[index1], &vertices[index4],
                               &vertices[index2]);
        }

        std::cout << "Vertices count: " << vertices.size() << std::endl;
        std::cout << "Faces count: " << Faces.size() << std::endl;
    }
}