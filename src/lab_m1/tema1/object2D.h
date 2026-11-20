#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    // Create square with given bottom left corner, length and color
    Mesh* CreateRectangle(const std::string& name, float width, float height, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, float size, glm::vec3 color, bool fill = false); // isosceles
    Mesh* CreatePolygon(const std::string& name, float radius, int sides, glm::vec3 color);
}
