#include "Cone.h"
#include "Mesh.h"
#include <iostream>


static std::vector<Vertex> vertices;
static std::vector<Face>   Faces;

static float radius = 0.5f;

void Cone::drawTriangleMeshFromFaces(){

    // Get current mode
    GLint shadeModel;
    glGetIntegerv(GL_SHADE_MODEL, &shadeModel);

    // Draw side face
    glBegin(GL_TRIANGLES);

    for (Mesh* g : this->graphs){
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
                // glNormal3f(normal.x, normal.y, normal.z);
            }
        }
    glEnd();
    
}
    

    

void Cone::drawNormalsFromFaces(){
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


void Cone::draw(){
    drawTriangleMeshFromFaces();
}

void Cone::drawNormal() {
    // drawNormalsFromFaces();
    glColor3f(1.0f, .0f, .0f);

    glBegin(GL_LINES);
    for (Mesh* g : this->graphs){
        for (Vertex *v : g->getVertices()){
            const glm::vec3 &normal = v->getNormals();
            const glm::vec3 &pos = (v->getPos());
            
            glVertex3f(pos.x, pos.y, pos.z);
            glVertex3f(pos.x + normal.x * .1f, pos.y + normal.y * .1f,
                        pos.z + normal.z * .1f);
        }
    }
    glEnd();
}

void Cone::calculate() {
    //  Create a new graph to store the side shape
    Mesh* side = new Mesh();
    Mesh* bottom = new Mesh();

    this->clearGraphs();

    float stepAngle = 360.0f / m_segmentsX; 
    float stepY = 1.0f / m_segmentsY;      



    // Create side vertices
    
    float angle = glm::radians(stepAngle);  
    float x1 = radius * glm::cos(0);
    float z1 = radius * glm::sin(0); 

    float x2 = radius * glm::cos(angle);
    float z2 = radius * glm::sin(angle); 

    for (int i = m_segmentsY; i > 0; i--)
    {
        float rate = i * stepY;
        // left bottom
        float x1 = rate * radius * glm::cos(0);
        float z1 = rate * radius * glm::sin(0); 
        // Right bottom
        float x2 = rate * radius * glm::cos(angle);
        float z2 = rate * radius * glm::sin(angle);

        float y = radius - i * stepY;
        glm::vec3 position1(x1, y, z1);
        glm::vec3 position2(x2, y, z2);

        // Build vertex with position
        Vertex* v1 = new Vertex(position1);
        Vertex* v2 = new Vertex(position2);

        side->addVertex(v1);
        side->addVertex(v2);

        if (i == m_segmentsY)
        {
            // Add bottom vertex(y = -0.5)
            glm::vec3 bottomVertexPos(0.0f, -radius, 0.0f);
            Vertex* bottomVertex = new Vertex(bottomVertexPos);
            bottom->addVertex(bottomVertex);
            bottom->addVertex(v1);
            bottom->addVertex(v2);
            Face* bottomFace = new Face(bottomVertex, v1, v2);
            bottom->addFace(bottomFace);
        }

        if (i == 1)
        {
            // Add top vertex(y = 0.5)
            glm::vec3 topVertexPos(0.0f, radius, 0.0f);
            Vertex* topVertex = new Vertex(topVertexPos);
            side->addVertex(topVertex);
        }
    }

    // Build the side face with vertices
    std::vector<Vertex*> tempVerts = side->getVertices();
    for (int j = 0; j < m_segmentsY; j++)
    {
        int index1 = j * 2;
        int index2 = index1 + 1;
        int index3 = (j + 1) * 2;
        int index4 = index3 + 1;

        Face* f1 = new Face(tempVerts[index1], tempVerts[index3], tempVerts[index4]);
        Face* f2 = new Face(tempVerts[index4], tempVerts[index2], tempVerts[index1]);

        side->addFace(f1);
        side->addFace(f2);

        if (index4 == 2 * m_segmentsY - 1)
        {
            Face* f3 = new Face(tempVerts[index4 + 1], tempVerts[index4], tempVerts[index3]);
            side->addFace(f3);
            break;
        }


    }

    // Rotate to get other graphs
    std::vector<Mesh*> tempSideList;
    std::vector<Mesh*> tempBottomList;

    for (int i = 0; i < m_segmentsX; i++)
    {
        Mesh* rotateSide = side->rotate(0.0f, i * stepAngle, 0.0f);
        Mesh* rotateButtom = bottom->rotate(0.0f, i * stepAngle, 0.0f);

        tempSideList.push_back(rotateSide);
        tempBottomList.push_back(rotateButtom);

    }

    // Union all graph
    side = Mesh::union_graph(tempSideList);
    bottom = Mesh::union_graph(tempBottomList);

    this->graphs.push_back(side);
    this->graphs.push_back(bottom);
    
    // Print total size
    int verticesSize = 0, facesSize = 0;
    for (Mesh* g : this->graphs)
    {   
        verticesSize += g->getVertices().size();
        facesSize += g->getFaces().size();
    }
    for (Mesh* g : this->graphs){
        g->calculateVertexNormal();
    }


    // std::cout << "Vertices count: " << verticesSize - m_segmentsX << std::endl;
    // std::cout << "Faces count: " << facesSize << std::endl;
}