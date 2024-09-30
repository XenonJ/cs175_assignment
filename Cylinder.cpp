#include "Cylinder.h"
#include "Graph.h"
#include <iostream>


float radius = 0.5f;

void Cylinder::drawTriangleMeshFromFaces() {
    // Get current mode
    GLint shadeModel;
    glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // Draw side face
    glBegin(GL_TRIANGLES);

    for (Graph* g : this->graphs) {
        g->calculateVertexNormal();
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

void Cylinder::draw() {
    drawTriangleMeshFromFaces();
}

void Cylinder::drawNormal() {
    glColor3f(1.0f, .0f, .0f);

    glBegin(GL_LINES);
    for (Graph* g : this->graphs) {
        for (Vertex* v : g->getVertices()) {
            const glm::vec3& normal = v->getNormals();
            const glm::vec3& pos = (v->getPos());

            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + normal.x * .1f, pos.y + normal.y * .1f,
                pos.z + normal.z * .1f);
        }
    }
    glEnd();
}

void Cylinder::calculate() {
    //  Create a new graph to store the side shape
    Graph* side = new Graph();
    Graph* topFace = new Graph();
    Graph* bottomFace = new Graph();

    this->clearGraphs();

    float stepAngle = 360.0f / m_segmentsX;
    float stepY = 1.0f / m_segmentsY;

    this->vertices.clear();
    this->Faces.clear();


    for (int i = 0; i <= m_segmentsY; i++) {
        float y = i * stepY - radius;

        for (int j = 0; j < m_segmentsX; j++) {
            float angle = glm::radians(j * stepAngle);
            float x = radius * glm::cos(angle);
            float z = radius * glm::sin(angle);

            glm::vec3 position(x, y, z);
            // Calculate normal
            glm::vec3 normal = glm::vec3(x, 0.0f, z);

            // Build vertex with position and normal
            Vertex* v = new Vertex(position);

            side->addVertex(v);

            /* Add vertice on top and bottom to their graph */

            if (i == 0) // This point to the bottom
            {
                Vertex* vbottom = new Vertex(position);
                glm::vec3 bottomNormal = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
                bottomFace->addVertex(vbottom);
            }

            if (i == m_segmentsY) // This point to the top
            {
                Vertex* vtop = new Vertex(position);
                glm::vec3 topNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
                topFace->addVertex(vtop);
            }
        }
    }

    // Get the vertices list for calculating the face 
    std::vector<Vertex*> verts = side->getVertices();
    // Add all side face to side shape
    for (int i = 0; i < m_segmentsY; ++i) {
        for (int j = 0; j < m_segmentsX; ++j) {
            int index1 = i * m_segmentsX + j;
            int index2 = (j + 1) % m_segmentsX + i * m_segmentsX;
            int index3 = (i + 1) * m_segmentsX + j;
            int index4 = (i + 1) * m_segmentsX + ((j + 1) % m_segmentsX);

            Face* f1 = new Face(verts[index1], verts[index3], verts[index4]);

            verts[index1]->addFace(f1);
            verts[index3]->addFace(f1);
            verts[index4]->addFace(f1);

            Face* f2 = new Face(verts[index4], verts[index2], verts[index1]);

            verts[index4]->addFace(f2);
            verts[index2]->addFace(f2);
            verts[index1]->addFace(f2);

            side->addFace(f1);
            side->addFace(f2);
        }
    }

    /* --------------------- Add the top and bottom center points to the graph --------------------*/
    glm::vec3 topCenter(0, 1.0f - radius, 0);
    glm::vec3 bottomCenter(0, -radius, 0);

    int topCenterIndex = this->vertices.size();
    glm::vec3 topNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    Vertex* top = new Vertex(topCenter);
    topFace->addVertex(top);

    int bottomCenterIndex = this->vertices.size();
    glm::vec3 bottomNormal = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    Vertex* bottom = new Vertex(bottomCenter);
    bottomFace->addVertex(bottom);



    // Calculate and add the top and bottom face

    std::vector<Vertex*> topVertices = topFace->getVertices();
    std::vector<Vertex*> bottomVertices = bottomFace->getVertices();

    for (int i = 0; i < m_segmentsX; i++)
    {
        int index1 = i;
        int index2 = (i + 1) % (m_segmentsX);
        Face* f1 = new Face(topVertices[m_segmentsX], topVertices[index2], topVertices[index1]);
        topFace->addFace(f1);
    }

    for (int j = 0; j < m_segmentsX; j++)
    {
        int index1 = j;
        int index2 = (j + 1) % (m_segmentsX);
        Face* f2 = new Face(bottomVertices[m_segmentsX], bottomVertices[index1], bottomVertices[index2]);
        bottomFace->addFace(f2);
    }

    // Push the graph to STL for managing
    this->graphs.push_back(side);
    this->graphs.push_back(topFace);
    this->graphs.push_back(bottomFace);

    // Print total size
    int verticesSize = 0, facesSize = 0;
    for (Graph* g : this->graphs)
    {
        verticesSize += g->getVertices().size();
        facesSize += g->getFaces().size();
    }


    std::cout << "Vertices count: " << verticesSize - 2 * m_segmentsX << std::endl;
    std::cout << "Faces count: " << facesSize << std::endl;
}
