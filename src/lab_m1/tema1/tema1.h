#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/tema1/Card.h"
#include "lab_m1/tema1/DamageCard.h"
#include "lab_m1/tema1/HealCard.h"
#include "lab_m1/tema1/ShieldCard.h"
#include "lab_m1/tema1/ScorchCard.h"
#include "lab_m1/tema1/FrostCard.h"
#include "lab_m1/tema1/ClearWeatherCard.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        glm::mat3 modelMatrix;
        static constexpr float LOGIC_W = 1280.0f; // constexpr is fixed at compile time so I don't write it again in cpp
        static constexpr float LOGIC_H = 720.0f; // logic because it's not the real size of the screen

        Card* placeholderCard;

        std::vector<Card*> playerCards;

        std::vector<Card*> playerDeck;
        float timeSincePreviousCard = 0; 
    };
}   // namespace m1
