#include "lab_m1/tema1/tema1.h"
#include "lab_m1/tema1/Card.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, LOGIC_W, 0, LOGIC_H, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    Mesh* cardRed = object2D::CreateRectangle("card_red", Card::WIDTH, Card::HEIGHT, glm::vec3(1, 0, 0), true);
    AddMeshToList(cardRed);

    Mesh* boardOpponent = object2D::CreateRectangle("opponent_board", 900, 180, glm::vec3(0.3f, 0.1f, 0.1f), true);
    AddMeshToList(boardOpponent);

    Mesh* boardPlayer = object2D::CreateRectangle("player_board", 900, 180, glm::vec3(0.1f, 0.1f, 0.3f), true);
    AddMeshToList(boardPlayer);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    glDepthFunc(GL_LEQUAL); // without this my cards dissapear behind the board of each player
}


void Tema1::Update(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(LOGIC_W / 2 - 70, LOGIC_H / 2 + 90);
    RenderMesh2D(meshes["opponent_board"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(LOGIC_W / 2 - 70, LOGIC_H / 2 - 90);
    RenderMesh2D(meshes["player_board"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(LOGIC_W / 2, LOGIC_H / 2);
    RenderMesh2D(meshes["card_red"], shaders["VertexColor"], modelMatrix);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
