#include "Special1.h"
#include "Mesh.h"
#include <iostream>


static std::vector<Vertex> vertices;
static std::vector<Face>   Faces;

static float radius = 0.5f;

void Special1::drawTriangleMeshFromFaces(){

    // Get current mode
    GLint shadeModel;
    glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // Draw side face
    glBegin(GL_TRIANGLES);

    for (Mesh* g : this->graphs){
        for (Face* face : *g->getFaceIterator()) {
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
                // glNormal3f(normal.x, normal.y, normal.z);
            }
        }
    glEnd();
    
}
    

    

void Special1::drawNormalsFromFaces(){
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


void Special1::draw(){
    drawTriangleMeshFromFaces();
}

void Special1::drawNormal() {
    // drawNormalsFromFaces();
    glColor3f(1.0f, .0f, .0f);

    glBegin(GL_LINES);
    for (Mesh* g : this->graphs){
        for (Vertex *v : *g->getVertexIterator()){
            const glm::vec3 &normal = v->getNormals();
            const glm::vec3 &pos = (v->getPos());
            
            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + normal.x * .1f, pos.y + normal.y * .1f,
                        pos.z + normal.z * .1f);
        }
    }
    glEnd();
}

void Special1::calculate() {
    int vcount = (m_segmentsY + 1) * (m_segmentsX + 1);
    int fcount = m_segmentsX * m_segmentsY * 2;
   // Create a new graph to store the heart surface
    Mesh* heart = new Mesh(vcount, fcount);
    Mesh* frontHeart= new Mesh(m_segmentsX + 1, m_segmentsX * 2);
    Mesh* backHeart = new Mesh(m_segmentsX + 1, m_segmentsX * 2);

    this->clearGraphs();

    float stepAngleX = 360.0f / m_segmentsX;  // Step along X axis
    float stepZ = 2.0f / m_segmentsY;         // Step along Z axis (heart height)

    // Generate heart shape in the XY plane and extrude along the Z axis
    for (int i = 0; i <= m_segmentsY; ++i) {
        float z = -1.0f + i * stepZ;  // Height of the current section (normalized)

        for (int j = 0; j < m_segmentsX; ++j) {
            float t = glm::radians(j * stepAngleX);
            float x = 16.0f * glm::pow(glm::sin(t), 3);
            float y = 13.0f * glm::cos(t) - 5.0f * glm::cos(2 * t) - 2.0f * glm::cos(3 * t) - glm::cos(4 * t);

            glm::vec3 position(x * 0.05f, y * 0.05f, z * radius);  // Scale down and add z-axis offset

            Vertex* v = new Vertex(position);
            heart->addVertex(v);

            // Back(z = -1)
            if (i == 0)
            {
                backHeart->addVertex(new Vertex(v->getPos()));
            }
            // Front(z = 1)
            else if (i == m_segmentsY)
            {
                frontHeart->addVertex(new Vertex(v->getPos()));
            }
            
        }
    }

    glm::vec3 topPosition(0.0f, 0.0f, 0.7f);
    glm::vec3 bottomPosition(0.0f, 0.0f, -0.7f);
    Vertex* topv = new Vertex(topPosition);
    Vertex* bottomv = new Vertex(bottomPosition);

    int backIndex = m_segmentsX;
    int frontIndex = m_segmentsX;

    backHeart->addVertex(bottomv);
    frontHeart->addVertex(topv);

    // Now connect vertices to form faces (triangles) between each layer
    Vertex** verts = heart->getVertices();
    Vertex** backs = backHeart->getVertices();
   Vertex** fronts = frontHeart->getVertices();

    for (int i = 0; i < m_segmentsY; ++i) {
        for (int j = 0; j < m_segmentsX; ++j) {
            int current = i * m_segmentsX + j;
            int next = current + m_segmentsX;
            int nextWrap = (j + 1) % m_segmentsX;

            int currentNext = i * m_segmentsX + nextWrap;
            int nextNext = currentNext + m_segmentsX;

            // Create two faces (triangles) between adjacent vertices
            Face* f1 = new Face(verts[current], verts[next], verts[currentNext]);
            Face* f2 = new Face(verts[currentNext], verts[next], verts[nextNext]);

            heart->addFace(f1);
            heart->addFace(f2);

            // Back(z = -1)
            if (i == 0)
            {
                Face* f3 = new Face(backs[backIndex], verts[current], verts[currentNext]);
                backHeart->addFace(f3);
            }
            // Front(z = 1)
            else if (i == m_segmentsY - 1)
            {
                Face* f4 = new Face(verts[nextNext], verts[next], fronts[frontIndex]);
                backHeart->addFace(f4);
            }
        }
    }

    this->graphs.push_back(heart);
    this->graphs.push_back(backHeart);
    this->graphs.push_back(frontHeart);
    // for (Mesh* g : this->graphs){
    //     g->calculateVertexNormal();
    // }


    // std::cout << "Vertices count: " << heart->getVertices().size() << std::endl;
    // std::cout << "Faces count: " << heart->getFaces().size() << std::endl;
}