#include "Cone.h"
#include "Mesh.h"
#include <cmath>
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
    

    

void Cone::drawNormalsFromFaces(){
    // glColor3f(1.0f, .0f, .0f);
    // glBegin(GL_LINES);
    // for (Face &face : Faces) {
    //     Vertex *const *verts      = face.getVertices();
    //     glm::vec3      v0Pos      = verts[0]->getPos();
    //     glm::vec3      v1Pos      = verts[1]->getPos();
    //     glm::vec3      v2Pos      = verts[2]->getPos();
    //     glm::vec3      v1v0       = glm::normalize(v1Pos - v0Pos);
    //     glm::vec3      v2v0       = glm::normalize(v2Pos - v0Pos);
    //     glm::vec3      faceNormal = glm::normalize(glm::cross(v1v0, v2v0));

    //     for (int i = 0; i < 3; ++i) {
    //         const glm::vec3 &pos = (verts[i]->getPos());
    //         glVertex3f(pos.x, pos.y, pos.z);
    //         glVertex3f(pos.x + faceNormal.x * .1f, pos.y + faceNormal.y * .1f,
    //                    pos.z + faceNormal.z * .1f);
    //     }
    // }
    // glEnd();
}


void Cone::draw(){
    drawTriangleMeshFromFaces();
}

void Cone::drawNormal() {
    // drawNormalsFromFaces();
    // std::cout << "The drawnormal function is called !" << std::endl;
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

void Cone::calculate() {
    int vcount = (m_segmentsX + 1) * (m_segmentsY + 1);
    int fcount = m_segmentsX  * (m_segmentsY + 1) * 2;
    // Create a new mesh to store the cone
    Mesh* side = new Mesh(vcount, fcount);
    Mesh* bottom = new Mesh(m_segmentsX + 1, m_segmentsX + 1);
    this->clearGraphs();

    float stepAngle = 2.0f * glm::pi<float>() / m_segmentsX;  // 360 degrees divided by segmentsX
    float stepY = 1.0f / m_segmentsY;  // Height divided by segmentsY

    float height = 1.0f;  // Total height of the cone
    float radius = 0.5f;  // Base radius of the cone

    // Generate vertices for the side mesh, based on segmentY and segmentX
    for (int i = 0; i <= m_segmentsY; i++) {
        float rate = i * stepY;
        float y = -height / 2 + i * stepY * height;  // y-coordinate for each level
        float r = radius * (height / 2 - y) / height;  // Calculate radius for current height level

        for (int j = 0; j < m_segmentsX; j++) {
            float angle = j * stepAngle;
            float x = r * glm::cos(angle);  // x-coordinate
            float z = r * glm::sin(angle);  // z-coordinate

            glm::vec3 position(x, y, z);
            Vertex* v = new Vertex(position);

            // Check if the vertex is on the bottom (y == -height / 2)
            if (y == -height / 2) {
                // This vertex is on the bottom, assign the downward normal
                glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);
                Vertex* tempV = new Vertex(position);
                tempV->setNormal(bottomNormal);  // Assign the normal to the vertex
                bottom->addVertex(tempV);
            }
            if (y != height / 2){
                float n_y = float(radius / sqrt(radius*radius + height*height));
                // Calculate normal for this vertex
                float Nx = x;
                float Nz = z;
                float length = sqrt(Nx * Nx + Nz * Nz);  // Only normalize x and z
                Nx /= length;
                Nz /= length;
                glm::vec3 normal(Nx, n_y , Nz);
                // normal = glm::normalize(normal);  // Normalize the normal vector
                v->setNormal(normal);  // Store the normal directly in the vertex
            }

            
            side->addVertex(v);
        }
    }

    // Add the top vertex (apex of the cone)
    glm::vec3 topVertexPos(0.0f, height / 2, 0.0f);
    Vertex* topVertex = new Vertex(topVertexPos);
    side->addVertex(topVertex);
    
    Vertex** verts = side->getVertices();
    // Generate side faces using index-based method
    for (int i = 0; i < m_segmentsY; ++i) {
        for (int j = 0; j < m_segmentsX; ++j) {
            // Calculate indices for the current and next vertices
            int index1 = i * m_segmentsX + j;
            int index2 = index1 + 1;
            if (j == m_segmentsX - 1) index2 = i * m_segmentsX;  // Wrap around to first vertex

            int index3 = (i + 1) * m_segmentsX + j;
            int index4 = index3 + 1;
            if (j == m_segmentsX - 1) index4 = (i + 1) * m_segmentsX;  // Wrap around

            // Create two faces for each segment
            Face* f1 = new Face(verts[index1], verts[index3], verts[index4]);
            Face* f2 = new Face(verts[index4], verts[index2], verts[index1]);

            side->addFace(f1);
            side->addFace(f2);
        }
    }

    // Create the top face that connects the last ring to the top vertex
    for (int j = 0; j < m_segmentsX; ++j) {
        int index1 = m_segmentsY * m_segmentsX + j;  // Last ring of vertices
        int index2 = m_segmentsY * m_segmentsX + (j + 1) % m_segmentsX;  // Wrap around

        // Create a face that connects the top vertex to the last ring
        Face* f = new Face(verts[index1], topVertex, verts[index2]);
        side->addFace(f);
    }

    // Add bottom mesh
    glm::vec3 bottomVertexPos(0.0f, -height / 2, 0.0f);
    Vertex* bottomVertex = new Vertex(bottomVertexPos);
    bottomVertex->setNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    bottom->addVertex(bottomVertex);

    // Create the bottom face by connecting each segment to the center
    for (int j = 0; j < m_segmentsX; ++j) {
        int index1 = j;
        int index2 = (j + 1) % m_segmentsX;  // Wrap around

        Face* bottomFace = new Face(bottomVertex, verts[index2], verts[index1]);
        bottom->addFace(bottomFace);
    }

    // Add bottom and side meshes to the graphs
    this->graphs.push_back(side);
    this->graphs.push_back(bottom);

    // Calculate normals for shading
    // for (Mesh* g : this->graphs) {
    //     g->calculateVertexNormal();
    // }

    // Optional: Print total number of vertices and faces
    // int verticesSize = 0, facesSize = 0;
    // for (Mesh* g : this->graphs) {
    //     verticesSize += g->getVertices().size();
    //     facesSize += g->getFaces().size();
    // }
    // std::cout << "Vertices count: " << verticesSize << std::endl;
    // std::cout << "Faces count: " << facesSize << std::endl;
}