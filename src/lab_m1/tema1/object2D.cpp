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
