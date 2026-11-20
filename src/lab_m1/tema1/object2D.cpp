#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2D::CreateRectangle(
    const std::string& name,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-width / 2, -height / 2, 0), color),  // bottom-left
        VertexFormat(glm::vec3(width / 2, -height / 2, 0), color),  // bottom-right
        VertexFormat(glm::vec3(width / 2,  height / 2, 0), color),  // top-right
        VertexFormat(glm::vec3(-width / 2,  height / 2, 0), color)   // top-left
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateTriangle(const std::string& name, float size, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-size / 2, -size / 2, 0), color), // bottom-left
        VertexFormat(glm::vec3(size / 2, -size / 2, 0), color), // bottom-right
        VertexFormat(glm::vec3(0, size / 2, 0), color) // top-middle
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    
    triangle->InitFromData(vertices, indices);
    return triangle;
}


Mesh* object2D::CreatePolygon(const std::string& name, float radius, int sides, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // vertex 0 is the center
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));

    // the points on the circle, at equal angles
    for (int i = 0; i < sides; i++) {
        float angle = 2 * glm::pi<float>() * i / sides;
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), radius * sin(angle), 0), color));
    }

    // one triangle per side: center, current point, next point
    for (int i = 1; i <= sides; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i % sides + 1); // the last triangle goes back to point 1
    }

    Mesh* polygon = new Mesh(name);
    polygon->InitFromData(vertices, indices);
    return polygon;
}