#pragma once

#include <vector>
#include <ctime> // for rand

#include "components/simple_scene.h"
#include "lab_m1/tema1/Card.h"
#include "lab_m1/tema1/DamageCard.h"
#include "lab_m1/tema1/HealCard.h"
#include "lab_m1/tema1/ShieldCard.h"
#include "lab_m1/tema1/ScorchCard.h"
#include "lab_m1/tema1/FrostCard.h"
#include "lab_m1/tema1/ClearWeatherCard.h"
#include "lab_m1/tema1/Player.h"

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

        void UpdateRow(std::vector<Card*>& row, float y); // everytime I need to move cards I should copy-paste the same code to rearange the player cards

        void ApplyEffects();

     protected:
        glm::mat3 modelMatrix;
        static constexpr float LOGIC_W = 1280.0f; // constexpr is fixed at compile time so I don't write it again in cpp
        static constexpr float LOGIC_H = 720.0f; // logic because it's not the real size of the screen

        std::vector<Card*> playerCards;
        std::vector<Card*> opponentCards;

        std::vector<Card*> playerDeck;
        std::vector<Card*> opponentDeck;
        float timeSincePreviousCard = 0;

        // drag and drop
        Card* selectedCard = nullptr; // nullptr means that I don't hold any card

        std::vector<Card*> playerBoard; // the cards that are on the board
        std::vector<Card*> opponentBoard;

        // time to show the HP on the screen
        Player* player;
        Player* opponent;

        // timer for opponent
        bool opponentTurn = false;
        float opponentDelay = 0;

        bool gameOver = false;
    };
}   // namespace m1
