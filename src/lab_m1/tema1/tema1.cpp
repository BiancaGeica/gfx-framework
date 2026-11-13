#include "lab_m1/tema1/tema1.h"

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

    Mesh* card = object2D::CreateRectangle("card", Card::WIDTH, Card::HEIGHT, glm::vec3(1, 1, 1), true);
    AddMeshToList(card);

    Mesh* boardOpponent = object2D::CreateRectangle("opponent_board", 900, 180, glm::vec3(0.3f, 0.1f, 0.1f), true);
    AddMeshToList(boardOpponent);

    Mesh* boardPlayer = object2D::CreateRectangle("player_board", 900, 180, glm::vec3(0.1f, 0.1f, 0.3f), true);
    AddMeshToList(boardPlayer);

    placeholderCard = new DamageCard(3, Lane::Melee);
    placeholderCard->SetPos(glm::vec2(LOGIC_W / 2, LOGIC_H / 2));

    Mesh* hpCard = object2D::CreateRectangle("card_hp", 10, 10, glm::vec3(1, 1, 1), true);
    AddMeshToList(hpCard);

    playerCards.push_back(new DamageCard(3, Lane::Melee));
    playerCards.push_back(new ShieldCard(4, Lane::Siege));
    playerCards.push_back(new HealCard(2, Lane::Ranged));
    playerCards.push_back(new FrostCard(2));
    playerCards.push_back(new ScorchCard(3, Lane::Melee));

    int numberCardsHand = playerCards.size();
    float distanceBetweenCards = 10.0f;
    float rowWidth = numberCardsHand * Card::WIDTH + (numberCardsHand - 1) * distanceBetweenCards;
    float firstCardX = LOGIC_W / 2 - 70 - rowWidth / 2 + Card::WIDTH / 2; // LOGIC_W / 2 - 70 is the x of the card in the middle

    for (int i = 0; i < numberCardsHand; i++) {
        playerCards[i]->SetTargetPos(glm::vec2(firstCardX + i * (Card::WIDTH + distanceBetweenCards), 100)); // SetTargetPos bc i want to point where the card should go
    }                                                                                                       // 80 because the card is 120px tall, so the middle is at 60 and i want a little gap down
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
    glm::vec2 cardPos = placeholderCard->GetPos();
    modelMatrix *= transform2D::Translate(cardPos.x, cardPos.y);
    // RenderMesh2D(meshes["card"], shaders["VertexColor"], modelMatrix); there is one more function for this that let me pass the color
    RenderMesh2D(meshes["card"], modelMatrix, placeholderCard->GetColor());

    int valueCard = placeholderCard->GetValue();
    for (int i = 0; i < valueCard; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(cardPos.x - Card::WIDTH / 2 + 10 + i * 14, cardPos.y - Card::HEIGHT / 2 + 10); // 4 pixels to the right of the previous square
        RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
    }

    for (int i = 0; i < playerCards.size(); i++) {
        Card* card = playerCards[i];
        card->UpdateAnimation(deltaTimeSeconds); // slide the cards on their place
        glm::vec2 positionCurrentCard = card->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);
        RenderMesh2D(meshes["card"], modelMatrix, card->GetColor());

        for (int i = 0; i < card->GetValue(); i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x - Card::WIDTH / 2 + 10 + i * 14, positionCurrentCard.y - Card::HEIGHT / 2 + 10); // 4 pixels to the right of the previous square
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }
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
