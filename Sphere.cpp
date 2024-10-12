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
    // Create a new graph to store the sphere surface
    Mesh* sphere = new Mesh();

    this->clearGraphs();

    float stepLongitude = 2.0f * glm::pi<float>() / m_segmentsX;  // Longitude angle step (radians)
    float stepLatitude = glm::pi<float>() / m_segmentsY;  // Latitude angle step (radians)

    // Store all vertices in the sphere
    std::vector<Vertex*> tempVerts;

    // Generate vertices from southern point to northern point
    for (int i = 1; i < m_segmentsY; i++) {  // Skipping poles (i = 0 for south, i = m_segmentsY for north)
        float latAngle = -glm::half_pi<float>() + i * stepLatitude;  // Latitude angle from -π/2 to π/2
        float sinLat = glm::sin(latAngle);
        float cosLat = glm::cos(latAngle);

        for (int j = 0; j < m_segmentsX; j++) {  // Full rotation in longitude
            float lonAngle = j * stepLongitude;  // Longitude angle
            float sinLon = glm::sin(lonAngle);
            float cosLon = glm::cos(lonAngle);

            // Transform Sphere coordinate to Cartesian coordinate
            float x = radius * cosLat * cosLon;
            float y = radius * sinLat;  
            float z = radius * cosLat * sinLon;

            glm::vec3 position(x, y, z);

            // Create vertex
            Vertex* v = new Vertex(position);

            // Set the normal to be the normalized position vector (direction from the origin)
            glm::vec3 normal = glm::normalize(position);
            v->setNormal(normal);  // Set normal for this vertex

            // Add vertex to the mesh and store in temporary list
            sphere->addVertex(v);
            tempVerts.push_back(v);  // Store vertex in tempVerts
        }
    }

    // Add poles (north and south vertices)
    glm::vec3 topPosition(0.0f, radius, 0.0f);      // North pole
    glm::vec3 bottomPosition(0.0f, -radius, 0.0f);  // South pole
    Vertex* topv = new Vertex(topPosition);
    Vertex* bottomv = new Vertex(bottomPosition);

    // Set normals for poles
    topv->setNormal(glm::vec3(0.0f, 1.0f, 0.0f));     // North pole normal points upwards
    bottomv->setNormal(glm::vec3(0.0f, -1.0f, 0.0f)); // South pole normal points downwards

    // Add poles to the mesh
    sphere->addVertex(bottomv);  // South pole
    sphere->addVertex(topv);     // North pole

    int bottomIndex = tempVerts.size();  // Index for south pole
    int topIndex = bottomIndex + 1;      // Index for north pole

    // Build faces for the sphere
    for (int i = 0; i < m_segmentsY - 1; i++) {
        for (int j = 0; j < m_segmentsX; j++) {
            int nextJ = (j + 1) % m_segmentsX;  // Wrap around in longitude

            // First row: connect bottom (south pole)
            if (i == 0) {
                Face* bottomFace = new Face(sphere->getVertices()[bottomIndex], tempVerts[j], tempVerts[nextJ]);
                sphere->addFace(bottomFace);
            }
            // Last row: connect top (north pole)
            else if (i == m_segmentsY - 2) {
                int baseIndex = (i) * m_segmentsX;
                int baseIndex1 = (i - 1) * m_segmentsX;
                int baseIndex2 = i * m_segmentsX;

                Face* f1 = new Face(tempVerts[baseIndex1 + j], tempVerts[baseIndex2 + j], tempVerts[baseIndex2 + nextJ]);
                Face* f2 = new Face(tempVerts[baseIndex2 + nextJ], tempVerts[baseIndex1 + nextJ], tempVerts[baseIndex1 + j]);

                sphere->addFace(f1);
                sphere->addFace(f2);
                Face* topFace = new Face(sphere->getVertices()[topIndex], tempVerts[baseIndex + nextJ], tempVerts[baseIndex + j]);
                sphere->addFace(topFace);
            }
            // Middle rows: connect latitude strips
            else {
                int baseIndex1 = (i - 1) * m_segmentsX;
                int baseIndex2 = i * m_segmentsX;

                Face* f1 = new Face(tempVerts[baseIndex1 + j], tempVerts[baseIndex2 + j], tempVerts[baseIndex2 + nextJ]);
                Face* f2 = new Face(tempVerts[baseIndex2 + nextJ], tempVerts[baseIndex1 + nextJ], tempVerts[baseIndex1 + j]);

                sphere->addFace(f1);
                sphere->addFace(f2);
            }
        }
    }

    // Add mesh to graphs
    this->graphs.push_back(sphere);

    // Optional: Output vertex and face counts
    std::cout << "Vertices count: " << sphere->getVertices().size() << std::endl;
    std::cout << "Faces count: " << sphere->getFaces().size() << std::endl;
}