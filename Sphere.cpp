#include "Sphere.h"
#include "Mesh.h"
#include <iostream>


static std::vector<Vertex> vertices;
static std::vector<Face>   Faces;

static float radius = 0.5f;

void Sphere::drawTriangleMeshFromFaces(){

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
    

    

void Sphere::drawNormalsFromFaces(){
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


void Sphere::draw(){
    drawTriangleMeshFromFaces();
}

void Sphere::drawNormal() {
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

void Sphere::calculate() {
    //  Create a new graph to store the sphere surface
    Mesh* sphere = new Mesh();

    this->clearGraphs();

    float stepLongitude = 360.0f / m_segmentsX;  // Longitude angle
    float stepLatitude = 180.0f / m_segmentsY;   // Latitude angle

    // Generate vertices from southern point to northern point
    for (int i = 1; i < m_segmentsY; i++) {
        float latAngle = glm::radians(-90.0f + i * stepLatitude); 
        float sinLat = glm::sin(latAngle);
        float cosLat = glm::cos(latAngle);

        for (int j = 0; j < 2; j++) {
            float lonAngle = glm::radians(j * stepLongitude); 
            float sinLon = glm::sin(lonAngle);
            float cosLon = glm::cos(lonAngle);

            // Transform Sphere coordinate to Cartesian coordinate
            float x = radius * cosLat * cosLon;
            float y = radius * sinLat;  
            float z = radius * cosLat * sinLon;

            glm::vec3 position(x, y, z);

            // Create vertex and add into graph
            Vertex* v = new Vertex(position);
            sphere->addVertex(v);
        }
    }

    glm::vec3 topPosition(0.0f, 0.5f, 0.0f);
    glm::vec3 bottomPosition(0.0f, -0.5f, 0.0f);
    Vertex* topv = new Vertex(topPosition);
    Vertex* bottomv = new Vertex(bottomPosition);

    int bottomIndex = sphere->getVertices().size();
    sphere->addVertex(bottomv);
    int topIndex = sphere->getVertices().size();
    sphere->addVertex(topv);

    // Build face of the sphere
    std::vector<Vertex*> verts = sphere->getVertices();

    for (int i = 0; i < m_segmentsY; i++)
    {
        // First triangle face
        if (i == 0)
        {
            Face* bottomFace = new Face(verts[bottomIndex], verts[0], verts[1]);
            sphere->addFace(bottomFace);
            continue;
        }
        // Last triangle face
        else if (i == m_segmentsY - 1)
        {
            Face* topFace = new Face(verts[topIndex], verts[2*i - 1], verts[2*i - 2]);
            sphere->addFace(topFace);
            break;
        }

        int leftdown = (i - 1) * 2;
        int rightdown = leftdown + 1;
        int leftup = i * 2;
        int rightup = leftup + 1;

        Face* f1 = new Face(verts[leftdown], verts[leftup], verts[rightup]);
        Face* f2 = new Face(verts[rightup], verts[rightdown], verts[leftdown]);

        sphere->addFace(f1);
        sphere->addFace(f2);

    }

    // Rotate to get other graphs
    std::vector<Mesh*> tempLatiList;

    for (int i = 0; i < m_segmentsX; i++)
    {
        Mesh* rotateSide = sphere->rotate(0.0f, i * stepLongitude, 0.0f);

        tempLatiList.push_back(rotateSide);


    }

    // Union all graph
    sphere = Mesh::union_graph(tempLatiList);

    this->graphs.push_back(sphere);
    for (Mesh* g : this->graphs){
        g->calculateVertexNormal();
    }

    // std::cout << "Vertices count: " << sphere->getVertices().size() << std::endl;
    // std::cout << "Faces count: " << sphere->getFaces().size() << std::endl;
}