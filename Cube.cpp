#include "Cube.h"
#include "Mesh.h"
#include <iostream>

// using namespace std;

static std::vector<Vertex> vertices;
static std::vector<Face>   Faces;


void Cube::drawTriangleMeshFromFaces() {
    // Get current mode
    GLint shadeModel;
    glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // Draw side face
    glBegin(GL_TRIANGLES);

    for (Mesh* g : this->graphs) {
        for (Face* face : g->getFaces()) {
            Vertex* const* verts = face->getVertices();
            for (int i = 0; i < 3; i++)
            {
                Vertex* v = verts[i];
                if (shadeModel == GL_SMOOTH)
                {
                    normalizeNormal(v->getNormals());
                }
                else if (shadeModel == GL_FLAT)
                {
                    normalizeNormal(face->getFaceNormal());
                }

                glm::vec3 pos = v->getPos();
                glVertex3f(pos.x, pos.y, pos.z);
            }
        }
    }
    glEnd();

}

void Cube::draw() {
    // Define transformation matrices (in draw, using glTranslatef and glRotatef)
    glPushMatrix();

    // Front face
    glTranslatef(0.0f, 0.0f, 0.5f);  // Move front face to z = 0.5
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    // Back face
    glTranslatef(0.0f, 0.0f, -0.5f);  // Move back face to z = -0.5
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);  // Rotate back face by 180 degrees along x-axis
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    // Right face
    glTranslatef(0.5f, 0.0f, 0.0f);  // Move right face to x = 0.5
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotate right face by 90 degrees along y-axis
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    // Left face
    glTranslatef(-0.5f, 0.0f, 0.0f);  // Move left face to x = -0.5
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);  // Rotate left face by -90 degrees along y-axis
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    // Bottom face
    glTranslatef(0.0f, -0.5f, 0.0f);  // Move bottom face to y = -0.5
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate bottom face by 90 degrees along x-axis
    drawTriangleMeshFromFaces();
    glPopMatrix();

    glPushMatrix();
    // Top face
    glTranslatef(0.0f, 0.5f, 0.0f);  // Move top face to y = 0.5
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Rotate top face by -90 degrees along x-axis
    drawTriangleMeshFromFaces();
    glPopMatrix();
}

void Cube::drawNormal() {
    glColor3f(1.0f, .0f, .0f);

    // Front face
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.5f);  // Move front face to z = 0.5
    drawNormalForSingleFace();       // Draw normals for the front face
    glPopMatrix();

    // Back face
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.5f);  // Move back face to z = -0.5
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);  // Rotate back face by 180 degrees along x-axis
    drawNormalForSingleFace();        // Draw normals for the back face
    glPopMatrix();

    // Right face
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);  // Move right face to x = 0.5
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotate right face by 90 degrees along y-axis
    drawNormalForSingleFace();        // Draw normals for the right face
    glPopMatrix();

    // Left face
    glPushMatrix();
    glTranslatef(-0.5f, 0.0f, 0.0f);  // Move left face to x = -0.5
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);  // Rotate left face by -90 degrees along y-axis
    drawNormalForSingleFace();         // Draw normals for the left face
    glPopMatrix();

    // Bottom face
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);  // Move bottom face to y = -0.5
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate bottom face by 90 degrees along x-axis
    drawNormalForSingleFace();        // Draw normals for the bottom face
    glPopMatrix();

    // Top face
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);  // Move top face to y = 0.5
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Rotate top face by -90 degrees along x-axis
    drawNormalForSingleFace();        // Draw normals for the top face
    glPopMatrix();
}

void Cube::drawNormalForSingleFace() {
    glBegin(GL_LINES);
    for (Mesh* g : this->graphs) {
        for (Vertex* v : g->getVertices()) {
            const glm::vec3& normal = v->getNormals();
            const glm::vec3& pos = (v->getPos());

            // Draw normal vector from the vertex position
            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + normal.x * 0.1f, pos.y + normal.y * 0.1f, pos.z + normal.z * 0.1f);
        }
    }
    glEnd();
}

void Cube::calculate() {
    int vcount = (m_segmentsX + 1) * (m_segmentsY + 1) * 6;
    int fcount = m_segmentsX * m_segmentsY * 12;
    Mesh* g = new Mesh(vcount, fcount);

    float stepX = 1.0f / m_segmentsX;
    float stepY = 1.0f / m_segmentsY;

    // Calculate all vertices
    for (int i = 0; i <= m_segmentsX; i++) {
        for (int j = 0; j <= m_segmentsY; j++) {
            float x = i * stepX - 0.5f;
            float y = j * stepY - 0.5f;
            float z = 0.0f;

            glm::vec3 position(x, y, z);
            Vertex* v = new Vertex(position);
            v->setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
            g->addVertex(v);
        }
    }

    // Calculate all faces
    for (int i = 0; i < m_segmentsX; i++) {
        for (int j = 0; j < m_segmentsY; j++) {
            int index1 = i * (m_segmentsY + 1) + j;
            int index2 = index1 + 1;
            int index3 = (i + 1) * (m_segmentsY + 1) + j;
            int index4 = index3 + 1;

            auto vertices = g->getVertices();
            g->addFace(new Face(vertices[index1], vertices[index3], vertices[index4]));
            g->addFace(new Face(vertices[index1], vertices[index4], vertices[index2]));
        }
    }

    // glm::mat4 Matrics[6];
    // Matrics[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
    // Matrics[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) *
    //         glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Matrics[2] = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f)) *
    //         glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Matrics[3] = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) *
    //         glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Matrics[4] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
    //         glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Matrics[5] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)) *
    //         glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    this->clearGraphs();
    // for (int i = 0; i < 6; i++) {
        this->graphs.push_back(g);
    // }
    // for (Mesh* g : this->graphs){
    //     g->calculateVertexNormal();
    // }
    // delete g;

}
