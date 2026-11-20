#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <algorithm> // for swap

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

Card* Tema1::AddRandomCard() {
    int value = 1 + rand() % 3; // cards hp
    Lane lane = (Lane)(rand() % 3);// On witch lane is the temporary card: Melee, Ranged, Siege

    switch (rand() % 15) {
    case 0: case 1: case 2:
        return new DamageCard(value, lane);
    case 3: case 4:
        return new HealCard(value, lane);
    case 5:
        return new ShieldCard(value, lane);
    case 6:  return new FrostCard(value);
    case 7:  return new FogCard(value);
    case 8:  return new RainCard(value);
    case 9:  return new ScorchCard(value, lane);
    case 10: return new SpyCard(1, lane);
    case 11: return new WarHornCard(1, lane);
    case 12: return new MedicCard(1, lane);
    case 13: return new DecoyCard(1, lane);
    default: return new ClearWeatherCard(1, lane);
    }
}

void Tema1::UpdateRow(std::vector<Card*>& row, float y, float scale) {
    // define where every card should go
    int numberCardsHand = row.size();
    float distanceBetweenCards = 10.0f;
    float cardWidth = Card::WIDTH * scale;
    float rowWidth = numberCardsHand * cardWidth + (numberCardsHand - 1) * distanceBetweenCards;
    float firstCardX = LOGIC_W / 2 - 70 - rowWidth / 2 + cardWidth / 2; // LOGIC_W / 2 - 70 is the x of the card in the middle

    for (int i = 0; i < numberCardsHand; i++) {
        row[i]->SetTargetPos(glm::vec2(firstCardX + i * (cardWidth + distanceBetweenCards), y));
    }
}

void Tema1::ApplyEffects() {
    for (int priority = 0; priority < 8; priority++) { // to cycle between priorities
        for (int i = 0; i < player->GetBoard().size(); i++) { // cycle thru cards on the players board
            if (player->GetBoard()[i]->GetPriority() == priority && !player->GetBoard()[i]->IsDestroyed() && !player->GetBoard()[i]->IsCancelled()) { // if the card is the one i'm searching rn
                player->GetBoard()[i]->ApplyTurnEffect(*player, *opponent); // the card apply its effect
            }
        }

        for (int i = 0; i < opponent->GetBoard().size(); i++) { // cycle thru cards on the opponents board
            if (opponent->GetBoard()[i]->GetPriority() == priority && !opponent->GetBoard()[i]->IsDestroyed() && !opponent->GetBoard()[i]->IsCancelled()) {
                opponent->GetBoard()[i]->ApplyTurnEffect(*opponent, *player);
            }
        }
    }

    for (int i = 0; i < player->GetBoard().size(); i++) {
        player->GetBoard()[i]->AfterTurn(); // the permanent cards ignore it because of polymorphism (the base AfterTurn is empty)
        player->GetBoard()[i]->SetCancelled(false); // without this, the card stays cancelled forever
    }
    for (int i = 0; i < opponent->GetBoard().size(); i++) {
        opponent->GetBoard()[i]->AfterTurn(); // lowers the value by 1 of the temporary cards
        opponent->GetBoard()[i]->SetCancelled(false);
    }

    player->SetLastCardPlayed(nullptr);
    opponent->SetLastCardPlayed(nullptr);

    for (int i = player->GetBoard().size() - 1; i >= 0; i--) {
        if (player->GetBoard()[i]->IsDestroyed()) {
            player->GetUsedCards().push_back(player->GetBoard()[i]);
            player->GetBoard().erase(player->GetBoard().begin() + i);
        }
    }

    for (int i = opponent->GetBoard().size() - 1; i >= 0; i--) {
        if (opponent->GetBoard()[i]->IsDestroyed()) {
            opponent->GetUsedCards().push_back(opponent->GetBoard()[i]);
            opponent->GetBoard().erase(opponent->GetBoard().begin() + i);
        }
    }

    LayoutBoard(player, true);
    LayoutBoard(opponent, false);
    UpdateRow(player->GetHand(), 60, HAND_SCALE);
    UpdateRow(opponent->GetHand(), LOGIC_H - 60, HAND_SCALE);

    // the round ends when someone reaches 0 HP or nobody has cards left
    if (player->IsDone() || opponent->IsDone() ||
        (player->GetHand().empty() && opponent->GetHand().empty())) {

        // the one with less HP loses a crystal; on equal HP both lose one
        if (player->GetHp() <= opponent->GetHp()) {
            player->LoseCrystal();
        }
        if (opponent->GetHp() <= player->GetHp()) {
            opponent->LoseCrystal();
        }

        if (player->GetCrystals() == 0 || opponent->GetCrystals() == 0) {
            gameOver = true;
            // state = GameState::OverviewScreen;
        }
        else {
            GoToNextRound();
        }
    }
}

void Tema1::GoToNextRound() {
    // clean the board at the end
    for (int i = 0; i < player->GetBoard().size(); i++) {
        player->GetUsedCards().push_back(player->GetBoard()[i]);
    }
    player->GetBoard().clear();

    for (int i = 0; i < opponent->GetBoard().size(); i++) {
        opponent->GetUsedCards().push_back(opponent->GetBoard()[i]);
    }
    opponent->GetBoard().clear();

    // reset the cards
    player->SetLastCardPlayed(nullptr);
    opponent->SetLastCardPlayed(nullptr);

    // reset HP
    player->ResetHp();
    opponent->ResetHp();

    // one more card for each player, dealt with the same animation
    maxCards++;
}

float Tema1::TextWidth(std::string text, float scale) {
    // every letter is equally wide (~29 px for size 48)
    return text.size() * 29.0f * scale;
}

float Tema1::LaneY(Lane lane, bool isPlayer) {
    // Melee = 0
    // Ranged = 1
    // Siege = 2 
    float offset = 40 + (int)lane * 80;

    if (isPlayer) {
        return LOGIC_H / 2 - offset;
    }
    return LOGIC_H / 2 + offset;
}

void Tema1::LayoutBoard(Player* p, bool isPlayer) {
    for (int l = 0; l < 3; l++) {
        std::vector<Card*> row;
        for (int i = 0; i < p->GetBoard().size(); i++) {
            if (p->GetBoard()[i]->GetLane() == (Lane)l) {
                row.push_back(p->GetBoard()[i]);
            }
        }
        UpdateRow(row, LaneY((Lane)l, isPlayer));
    }
}

void Tema1::UseDecoy(Player* p, Card* decoy, int target) {
    Card* returned = p->GetBoard()[target];
    decoy->SetLane(returned->GetLane());
    p->GetBoard()[target] = decoy;
    returned->ResetValue();
    returned->SetFaceUp(false);
    p->GetHand().push_back(returned);
}

void Tema1::StartGame() {
    // throw away everything from the previous game
    if (player != nullptr) {
        for (int i = 0; i < player->GetHand().size(); i++) {
            delete player->GetHand()[i];
        }
        for (int i = 0; i < player->GetBoard().size(); i++) {
            delete player->GetBoard()[i];
        }
        for (int i = 0; i < player->GetUsedCards().size(); i++) {
            delete player->GetUsedCards()[i];
        }
        delete player;
    }
    if (opponent != nullptr) {
        for (int i = 0; i < opponent->GetHand().size(); i++) {
            delete opponent->GetHand()[i];
        }
        for (int i = 0; i < opponent->GetBoard().size(); i++) {
            delete opponent->GetBoard()[i];
        }
        for (int i = 0; i < opponent->GetUsedCards().size(); i++) {
            delete opponent->GetUsedCards()[i];
        }
        delete opponent;
    }
    for (int i = 0; i < playerDeck.size(); i++) {
        delete playerDeck[i];
    }
    for (int i = 0; i < opponentDeck.size(); i++) {
        delete opponentDeck[i];
    }
    playerDeck.clear();
    opponentDeck.clear();

    for (int i = 0; i < leaders.size(); i++) {
        delete leaders[i];
    }
    leaders.clear();
    delete opponentLeader;

    // reset all the counters
    usedCards = 0;
    maxCards = 10;
    timeSincePreviousCard = 0;
    selectedCard = nullptr;
    opponentTurn = false;
    opponentDelay = 0;
    gameOver = false;
    gameOverTimer = 0;
    flippingCard = nullptr;
    flipTimer = 0;
    selectedLeader = -1;
    playerLeader = nullptr;
    opponentLeader = nullptr;

    // ~*~ new leaders ~*~
    leaders.push_back(new FoltestLeader());
    leaders.push_back(new EmhyrLeader());
    leaders.push_back(new EredinLeader());
    leaders.push_back(new FrancescaLeader());

    // new decks
    for (int i = 0; i < 12; i++) {
        playerDeck.push_back(AddRandomCard());
        opponentDeck.push_back(AddRandomCard());
    }
    for (int i = 0; i < playerDeck.size(); i++) {
        playerDeck[i]->SetPos(glm::vec2(LOGIC_W - 120 - 10, LOGIC_H / 2 - 90 - 10));
    }
    for (int i = 0; i < opponentDeck.size(); i++) {
        opponentDeck[i]->SetPos(glm::vec2(LOGIC_W - 120 - 10, LOGIC_H / 2 + 90 + 10));
    }

    // new players
    player = new Player(20);
    opponent = new Player(20);
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, LOGIC_W, 0, LOGIC_H, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    // for the welcome screen
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, LOGIC_W, LOGIC_H); // window->props.selfDir = where the program is
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 48);

    AddMeshToList(object2D::CreateRectangle("button", 240, 80, glm::vec3(1, 1, 1), true));

    // the shapes of the leaders
    AddMeshToList(object2D::CreatePolygon("leader_circle", 50, 40, glm::vec3(1, 1, 1))); // the color is just a placeholder
    AddMeshToList(object2D::CreateTriangle("leader_triangle", 100, glm::vec3(1, 1, 1), true));
    AddMeshToList(object2D::CreatePolygon("leader_hexagon", 50, 6, glm::vec3(1, 1, 1)));
    AddMeshToList(object2D::CreatePolygon("leader_diamond", 50, 4, glm::vec3(1, 1, 1)));

    // I moved them in StartGame bc init can't be called everytime I want to play again so I made a new function
   /* leaders.push_back(new FoltestLeader());
    leaders.push_back(new EmhyrLeader());
    leaders.push_back(new EredinLeader());
    leaders.push_back(new FrancescaLeader());*/

    srand(time(0)); // for random

    Mesh* card = object2D::CreateRectangle("card", Card::WIDTH, Card::HEIGHT, glm::vec3(1, 1, 1), true);
    AddMeshToList(card);

    Mesh* boardOpponent = object2D::CreateRectangle("opponent_board", 900, 180, glm::vec3(0.3f, 0.1f, 0.1f), true);
    AddMeshToList(boardOpponent);

    Mesh* boardPlayer = object2D::CreateRectangle("player_board", 900, 180, glm::vec3(0.1f, 0.1f, 0.3f), true);
    AddMeshToList(boardPlayer);

    Mesh* hpCard = object2D::CreateRectangle("card_hp", 6, 6, glm::vec3(1, 1, 1), true);
    AddMeshToList(hpCard);

    Mesh* hpPlayer = object2D::CreateRectangle("player_hp", 10, 10, glm::vec3(0.18f, 0.54f, 0.34f), true);
    AddMeshToList(hpPlayer);

    //// the cards that the player receives from the deck in the begining
    ///*playerDeck.push_back(new DamageCard(3, Lane::Melee));
    //playerDeck.push_back(new SpyCard(1, Lane::Melee));
    //playerDeck.push_back(new HealCard(2, Lane::Ranged));
    //playerDeck.push_back(new FrostCard(2));
    //playerDeck.push_back(new ScorchCard(3, Lane::Melee));
    //playerDeck.push_back(new DamageCard(1, Lane::Siege));
    //playerDeck.push_back(new HealCard(3, Lane::Melee));
    //playerDeck.push_back(new ShieldCard(2, Lane::Ranged));
    //playerDeck.push_back(new ClearWeatherCard(1, Lane::Melee));
    //playerDeck.push_back(new WarHornCard(1, Lane::Melee));*/

    //for (int i = 0; i < 12; i++) {
    //    playerDeck.push_back(AddRandomCard());
    //    opponentDeck.push_back(AddRandomCard());
    //}

    //// the cards that the opponent receives from the deck in the begining
    ///*opponentDeck.push_back(new DamageCard(3, Lane::Melee));
    //opponentDeck.push_back(new SpyCard(1, Lane::Melee));
    //opponentDeck.push_back(new HealCard(2, Lane::Ranged));
    //opponentDeck.push_back(new FrostCard(2));
    //opponentDeck.push_back(new ScorchCard(3, Lane::Melee));
    //opponentDeck.push_back(new DamageCard(1, Lane::Siege));
    //opponentDeck.push_back(new HealCard(3, Lane::Melee));
    //opponentDeck.push_back(new ShieldCard(2, Lane::Ranged));
    //opponentDeck.push_back(new ClearWeatherCard(1, Lane::Melee));
    //opponentDeck.push_back(new WarHornCard(1, Lane::Melee));*/

    //for (int i = playerDeck.size() - 1; i > 0; i--) { // to shuffle the deck
    //    int j = rand() % (i + 1);
    //    std::swap(playerDeck[i], playerDeck[j]);
    //}

    //for (int i = opponentDeck.size() - 1; i > 0; i--) { // to shuffle the deck
    //    int j = rand() % (i + 1);
    //    std::swap(opponentDeck[i], opponentDeck[j]);
    //}

    //for (int i = 0; i < playerDeck.size(); i++) {
    //    playerDeck[i]->SetPos(glm::vec2(LOGIC_W - 120 - 10, LOGIC_H / 2 - 90 - 10));
    //}

    //for (int i = 0; i < opponentDeck.size(); i++) {
    //    opponentDeck[i]->SetPos(glm::vec2(LOGIC_W - 120 - 10, LOGIC_H / 2 + 90 + 10));
    //}

    //player = new Player(20);
    //opponent = new Player(20);

    // triangle for temporary cards
    AddMeshToList(object2D::CreateTriangle("symbol_triangle", 18, glm::vec3(0, 0, 0), true));

    // square for permanent cards
    AddMeshToList(object2D::CreateRectangle("symbol_square", 16, 16, glm::vec3(0, 0, 0), true));

    AddMeshToList(object2D::CreateRectangle("crystal", 14, 14, glm::vec3(0.6f, 0.9f, 1.0f), true));

    AddMeshToList(object2D::CreateRectangle("lane", 900, 76, glm::vec3(1, 1, 1), true)); // the gap between lanes

    StartGame(); // the new function to restart the game in the final screen bc i can't call init again
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    //glDepthFunc(GL_LEQUAL); // without this my cards dissapear behind the board of each player
    glDisable(GL_DEPTH_TEST); // I CANT WRITE START OVER MY BUTTON WITHOUT THIS SOMEHOW
}


void Tema1::Update(float deltaTimeSeconds)
{
    // welcome screen
    if (state == GameState::WelcomeScreen) {
        textRenderer->RenderText("WELCOME TO GWENT!", LOGIC_W / 2 - TextWidth("WELCOME TO GWENT!", 1.0f) / 2, 250, 1.0f, glm::vec3(0.3f, 0.8f, 0.5f));
        textRenderer->RenderText("THE", LOGIC_W / 2 - TextWidth("THE", 0.5f) / 2, 320, 0.5f, glm::vec3(0.85f, 0.65f, 0.2f));
        textRenderer->RenderText("WITCHER CARD GAME", LOGIC_W / 2 - TextWidth("WITCHER CARD GAME", 0.5f) / 2, 350, 0.5f, glm::vec3(0.85f, 0.65f, 0.2f));

        // button is centered at y = 260 from the bottom witch means 460 from the top
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(LOGIC_W / 2, 260);
        RenderMesh2D(meshes["button"], modelMatrix, glm::vec3(0.2f, 0.5f, 0.3f));

        textRenderer->RenderText("START", LOGIC_W / 2 - TextWidth("START", 0.8f) / 2, 446, 0.8f, glm::vec3(1, 1, 1));
        return; // don't draw the game!!!!!!!!!!
    }

    // leader select screen
    if (state == GameState::LeaderSelectScreen) {
        textRenderer->RenderText("CHOOSE YOUR LEADER", LOGIC_W / 2 - TextWidth("CHOOSE YOUR LEADER", 1.0f) / 2, 150, 1.0f, glm::vec3(0.85f, 0.65f, 0.2f));

        for (int i = 0; i < leaders.size(); i++) {
            float x = LOGIC_W / 2 + (i - 1.5f) * 220;
            std::string mesh = leaders[i]->GetMeshName();

            if (i == selectedLeader) { // highlight
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(x, LOGIC_H / 2);
                modelMatrix *= transform2D::Scale(1.15f, 1.15f);
                RenderMesh2D(meshes[mesh], modelMatrix, glm::vec3(0.2f, 0.4f, 1.0f));
            }

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, LOGIC_H / 2);
            RenderMesh2D(meshes[mesh], modelMatrix, leaders[i]->GetColor());
        }
        return;
    }

    // final screen
    if (state == GameState::OverviewScreen) {
        std::string message = "YOU LOST";
        glm::vec3 color = glm::vec3(0.9f, 0.3f, 0.3f); // red

        if (player->GetCrystals() == opponent->GetCrystals()) {
            message = "DRAW";
            color = glm::vec3(0.85f, 0.65f, 0.2f); // gold
        }
        else if (player->GetCrystals() > opponent->GetCrystals()) {
            message = "YOU WON!";
            color = glm::vec3(0.3f, 0.8f, 0.5f); // green
        }

        textRenderer->RenderText(message, LOGIC_W / 2 - TextWidth(message, 1.5f) / 2, 250, 1.5f, color);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(LOGIC_W / 2, 260);
        RenderMesh2D(meshes["button"], modelMatrix, glm::vec3(0.2f, 0.5f, 0.3f));
        textRenderer->RenderText("PLAY AGAIN", LOGIC_W / 2 - TextWidth("PLAY AGAIN", 0.6f) / 2, 450, 0.6f, glm::vec3(1, 1, 1));
        return;
    }

    // the leaders on the in game screen
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(60, 270);
    RenderMesh2D(meshes[playerLeader->GetMeshName()], modelMatrix,
        playerLeader->IsUsed() ? glm::vec3(0.4f, 0.4f, 0.4f) : playerLeader->GetColor());

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(60, LOGIC_H - 270);
    RenderMesh2D(meshes[opponentLeader->GetMeshName()], modelMatrix,
        opponentLeader->IsUsed() ? glm::vec3(0.4f, 0.4f, 0.4f) : opponentLeader->GetColor());
    
    glm::vec3 playerBoardColor = glm::vec3(0.1f, 0.1f, 0.3f); // used to change the color of the boards to show who won
    glm::vec3 opponentBoardColor = glm::vec3(0.3f, 0.1f, 0.1f);

    if (gameOver) {
        if (player->GetCrystals() == opponent->GetCrystals()) { // draw
            playerBoardColor = glm::vec3(1, 1, 0);
            opponentBoardColor = glm::vec3(1, 1, 0);
        }
        else if (player->GetCrystals() > opponent->GetCrystals()) { // player won
            playerBoardColor = glm::vec3(0, 1, 0);
            opponentBoardColor = glm::vec3(1, 0, 0);
        }
        else {
            playerBoardColor = glm::vec3(1, 0, 0); // opponent won
            opponentBoardColor = glm::vec3(0, 1, 0);
        }
    }
    
    // the board
    for (int l = 0; l < 3; l++) {
        Lane lane = (Lane)l;

        // opponent's lanes
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(LOGIC_W / 2 - 70, LaneY(lane, false));
        RenderMesh2D(meshes["lane"], modelMatrix, opponentBoardColor);

        // players lanes
        glm::vec3 laneColor = playerBoardColor;
        if (selectedCard != nullptr && selectedCard->CanBeOnLane(lane)) {
            laneColor = glm::vec3(0.2f, 0.45f, 0.3f); // highlight
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(LOGIC_W / 2 - 70, LaneY(lane, true));
        RenderMesh2D(meshes["lane"], modelMatrix, laneColor);
    }

    // the players deck
    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120, LOGIC_H / 2 - 90);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.2f, 0.2f, 0.3f));


    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120 - 5, LOGIC_H / 2 - 90 - 5);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.54f, 0.17f, 0.88f));

    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120 - 10, LOGIC_H / 2 - 90 - 10);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.55f, 0.0f, 0.55f));

    // the opponents deck
    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120, LOGIC_H / 2 + 90);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.2f, 0.2f, 0.3f));

    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120 - 5, LOGIC_H / 2 + 90 + 5);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.54f, 0.17f, 0.88f));

    modelMatrix = glm::mat3(1);
    modelMatrix = transform2D::Translate(LOGIC_W - 120 - 10, LOGIC_H / 2 + 90 + 10);
    modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
    RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.55f, 0.0f, 0.55f));

    // players cards that are on the board
    for (int i = 0; i < player->GetBoard().size(); i++) {
        Card* card = player->GetBoard()[i];

        card->UpdateAnimation(deltaTimeSeconds); // slide the cards on their place

        glm::vec2 positionCurrentCard = card->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);
        RenderMesh2D(meshes["card"], modelMatrix, card->GetColor());

        // render the symbol: triangle for temporary card and square for permanent
        if (card->IsTemporary())
            RenderMesh2D(meshes["symbol_triangle"], modelMatrix, glm::vec3(0, 0, 0));
        else
            RenderMesh2D(meshes["symbol_square"], modelMatrix, glm::vec3(0, 0, 0));

        for (int i = 0; i < card->GetValue(); i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x - Card::WIDTH / 2 + 6 + i * 8, positionCurrentCard.y - Card::HEIGHT / 2 + 6); // 4 pixels to the right of the previous square
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }

    // players cards
    for (int i = 0; i < player->GetHand().size(); i++) {
        Card* card = player->GetHand()[i];

        if (card == selectedCard) {
            continue; // this warning is a drama queen because it could never be NULL
        }

        card->UpdateAnimation(deltaTimeSeconds); // slide the cards on their place

        glm::vec2 positionCurrentCard = card->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);
        modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
        RenderMesh2D(meshes["card"], modelMatrix, card->GetColor());

        // render the symbol: triangle for temporary card and square for permanent
        if (card->IsTemporary())
            RenderMesh2D(meshes["symbol_triangle"], modelMatrix, glm::vec3(0, 0, 0));
        else
            RenderMesh2D(meshes["symbol_square"], modelMatrix, glm::vec3(0, 0, 0));

        for (int i = 0; i < card->GetValue(); i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x + (-Card::WIDTH / 2 + 6 + i * 8) * HAND_SCALE, positionCurrentCard.y + (-Card::HEIGHT / 2 + 6) * HAND_SCALE); // 4 pixels to the right of the previous square
            modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }

    // opponents cards
    for (int i = 0; i < opponent->GetHand().size(); i++) {
        Card* card = opponent->GetHand()[i];
        card->UpdateAnimation(deltaTimeSeconds); // slide the cards on their place
        glm::vec2 positionCurrentCard = card->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);

        float scaleX = 1;
        bool showFace = card->IsFaceUp();

        // to reveal the opponent cards when Emhyr is activated
        if (card->IsFaceUp() && card->GetFlipProgress() < 1) {
            float t = glm::max(0.0f, card->GetFlipProgress());
            scaleX = abs(1 - 2 * t);
            showFace = t >= 0.5f;
        }

        modelMatrix *= transform2D::Scale(scaleX * HAND_SCALE, HAND_SCALE);

        if (!showFace) { // hidden: only the back
            RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.55f, 0.0f, 0.55f));
            continue;
        }

        // revealed by Emhyr: show the card like the ones on the board
        RenderMesh2D(meshes["card"], modelMatrix, card->GetColor());

        if (card->IsTemporary())
            RenderMesh2D(meshes["symbol_triangle"], modelMatrix, glm::vec3(0, 0, 0));
        else
            RenderMesh2D(meshes["symbol_square"], modelMatrix, glm::vec3(0, 0, 0));

        if (card->GetFlipProgress() < 1) continue; // the squares don't shrink, so hide them while flipping

        for (int j = 0; j < card->GetValue(); j++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x + (-Card::WIDTH / 2 + 6 + j * 8) * HAND_SCALE, positionCurrentCard.y + (-Card::HEIGHT / 2 + 6) * HAND_SCALE);
            modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }

    // cards flying out of deck animation
    timeSincePreviousCard += deltaTimeSeconds;
    if (timeSincePreviousCard > 0.4f && usedCards < maxCards) { // can't decide between 0.3 and 0.4 //COME BACK LATERRR
        usedCards++;
        timeSincePreviousCard = 0;
        player->GetHand().push_back(playerDeck.back()); // takes the card that is on top
        playerDeck.pop_back(); // removes the card from the deck

        opponent->GetHand().push_back(opponentDeck.back());
        opponentDeck.pop_back();

        UpdateRow(player->GetHand(), 60, HAND_SCALE);
        UpdateRow(opponent->GetHand(), LOGIC_H - 60, HAND_SCALE);
    }

    // draw the selected card because it went under other things when being hoovered over the screen and that's not cute
    if (selectedCard != nullptr) {
        glm::vec2 positionCurrentCard = selectedCard->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);
        modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
        RenderMesh2D(meshes["card"], modelMatrix, selectedCard->GetColor());

        // render the symbol: triangle for temporary card and square for permanent
        if (selectedCard->IsTemporary())
            RenderMesh2D(meshes["symbol_triangle"], modelMatrix, glm::vec3(0, 0, 0));
        else
            RenderMesh2D(meshes["symbol_square"], modelMatrix, glm::vec3(0, 0, 0));

        for (int i = 0; i < selectedCard->GetValue(); i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x + (-Card::WIDTH / 2 + 6 + i * 8) * HAND_SCALE, positionCurrentCard.y + (-Card::HEIGHT / 2 + 6) * HAND_SCALE); // 4 pixels to the right of the previous square
            modelMatrix *= transform2D::Scale(HAND_SCALE, HAND_SCALE);
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }

    // draw the HP for both players
    for (int i = 0; i < player->GetHp(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + (i % 5) * 14, 60 + (i / 5) * 14);
        RenderMesh2D(meshes["player_hp"], modelMatrix, glm::vec3(0.18f, 0.54f, 0.34f));
    }

    for (int i = 0; i < opponent->GetHp(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + (i % 5) * 14, LOGIC_H - 60 - (i / 5) * 14);
        RenderMesh2D(meshes["player_hp"], modelMatrix, glm::vec3(0.18f, 0.54f, 0.34f));
    }

    // draw shield points for both players
    for (int i = 0; i < player->GetShield(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + (i % 5) * 14, 60 + 5 * 14 + (i / 5) * 14);
        RenderMesh2D(meshes["player_hp"], modelMatrix, glm::vec3(0.0f, 0.80f, 0.82f));
    }

    for (int i = 0; i < opponent->GetShield(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + (i % 5) * 14, LOGIC_H - 60 - 5 * 14 - (i / 5) * 14);
        RenderMesh2D(meshes["player_hp"], modelMatrix, glm::vec3(0.0f, 0.80f, 0.82f));
    }

    for (int i = 0; i < player->GetCrystals(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + i * 20, 30);
        modelMatrix *= transform2D::Rotate(glm::radians(45.0f)); // 45 degrees to look like diamonds
        RenderMesh2D(meshes["crystal"], modelMatrix, glm::vec3(0.6f, 0.9f, 1.0f));
    }

    for (int i = 0; i < opponent->GetCrystals(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + i * 20, LOGIC_H - 30);
        modelMatrix *= transform2D::Rotate(glm::radians(45.0f));
        RenderMesh2D(meshes["crystal"], modelMatrix, glm::vec3(0.6f, 0.9f, 1.0f));
    }

    // opponent cards that are on the board
    for (int i = 0; i < opponent->GetBoard().size(); i++) {
        Card* card = opponent->GetBoard()[i];

        card->UpdateAnimation(deltaTimeSeconds); // slide the cards on their place

        glm::vec2 positionCurrentCard = card->GetPos();

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(positionCurrentCard.x, positionCurrentCard.y);

        // for the animation itself, (like that pirouette), the card shrink horizontally, change its face and goes back to normal
        float scaleX = 1;
        bool showFace = card->IsFaceUp();

        if (card == flippingCard) { // this card is turning over right now
            float t = (flipTimer - 0.5f) / 0.3f; // starts after 0.5s, lasts 0.3s
            t = glm::clamp(t, 0.0f, 1.0f);       // stays 0 while flying, 1 at the end
            scaleX = abs(1 - 2 * t);                         // the shrink then expanding back
            showFace = t >= 0.5f;                            // the moment when the face appears
        }

        modelMatrix *= transform2D::Scale(scaleX, 1);

        //if (!card->IsFaceUp()) { // face down: only the back of the card
        if (!showFace) {
            RenderMesh2D(meshes["card"], modelMatrix, glm::vec3(0.55f, 0.0f, 0.55f));
            continue;
        }

        RenderMesh2D(meshes["card"], modelMatrix, card->GetColor());

        // render the symbol: triangle for temporary card and square for permanent
        if (card->IsTemporary())
            RenderMesh2D(meshes["symbol_triangle"], modelMatrix, glm::vec3(0, 0, 0));
        else
            RenderMesh2D(meshes["symbol_square"], modelMatrix, glm::vec3(0, 0, 0));

        if (card == flippingCard) {
            continue; // the squares don't shrink with the card, so they should stay hided durring the flip
        }

        for (int i = 0; i < card->GetValue(); i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(positionCurrentCard.x - Card::WIDTH / 2 + 6 + i * 8, positionCurrentCard.y - Card::HEIGHT / 2 + 6); // 4 pixels to the right of the previous square
            
            RenderMesh2D(meshes["card_hp"], modelMatrix, glm::vec3(1, 1, 1));
        }
    }

    // if there are no cards left in the player hand, but the opponent still has cards, skip the player
    if (!opponentTurn && !gameOver && usedCards == maxCards && player->GetHand().empty()) {
        opponentTurn = true;
        opponentDelay = 0;
    }

    // show final screen after a little delay
    if (gameOver) {
        gameOverTimer += deltaTimeSeconds;
        if (gameOverTimer > 2.0f) {
            state = GameState::OverviewScreen;
        }
    }

    // make the opponent put down cards with delay
    if (opponentTurn && flippingCard == nullptr) {
        opponentDelay += deltaTimeSeconds;
        if (opponentDelay > 1.0f) {
            // the opponent uses its leader at a random moment: 1 in 4 chance each turn
            if (!opponentLeader->IsUsed() && rand() % 4 == 0) {
                opponentLeader->Activate(*opponent, *player); // swapped: for it, the opponent is me
            }

            if (!opponent->GetHand().empty()) { // could be 0, then rand would go kapoot
                int random = rand() % opponent->GetHand().size();
                if (opponent->GetHand()[random]->GetType() == CardType::Decoy && !opponent->GetBoard().empty()) {
                    UseDecoy(opponent, opponent->GetHand()[random], rand() % opponent->GetBoard().size());
                }
                else {
                    opponent->GetBoard().push_back(opponent->GetHand()[random]);
                }
                opponent->SetLastCardPlayed(opponent->GetHand()[random]); // for Scorch, it saves the last played card

                if (opponent->GetDoubleNextCard()) {
                    opponent->GetHand()[random]->ModifyValue(opponent->GetHand()[random]->GetValue());
                    opponent->SetDoubleNextCard(false);
                }

                flippingCard = opponent->GetHand()[random]; // for the turning effect when arriving on the table
                flipTimer = 0;

                opponent->GetHand().erase(opponent->GetHand().begin() + random);

                UpdateRow(opponent->GetHand(), LOGIC_H - 60, HAND_SCALE);
                LayoutBoard(opponent, false);
            }
            else { // because there are no other cards left there is nothing left to flip and the turn should end
                ApplyEffects();
                opponentTurn = false;
            }

        }
    }

    if (flippingCard != nullptr) {
        flipTimer += deltaTimeSeconds;
        if (flipTimer > 0.8f) {
            flippingCard->SetFaceUp(true);
            flippingCard = nullptr;
            ApplyEffects(); // make the cards do things
            opponentTurn = false;
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
    glm::ivec2 resolution = window->GetResolution();

    if (selectedCard != nullptr) {
        float x = mouseX * LOGIC_W / resolution.x;
        float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;

        selectedCard->SetPos(glm::vec2(x, y));
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    //welcome screen
    if (state == GameState::WelcomeScreen) {
        glm::ivec2 resolution = window->GetResolution();
        float x = mouseX * LOGIC_W / resolution.x;
        float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;

        // the button is 240 x 80, centered on the screen
        if (x >= LOGIC_W / 2 - 120 && x <= LOGIC_W / 2 + 120 && y >= 260 - 40 && y <= 260 + 40) {
            state = GameState::LeaderSelectScreen;
        }
        return;
    }

    // leader select screen
    if (state == GameState::LeaderSelectScreen) {
        glm::ivec2 resolution = window->GetResolution();
        float x = mouseX * LOGIC_W / resolution.x;
        float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;

        for (int i = 0; i < leaders.size(); i++) {
            float leaderX = LOGIC_W / 2 + (i - 1.5f) * 220;

            // check if the player clicked inside the leader
            if (glm::distance(glm::vec2(x, y), glm::vec2(leaderX, LOGIC_H / 2)) <= 50) {
                if (i == selectedLeader) { // second click on the same leader: start the game
                    playerLeader = leaders[i];

                    int r = rand() % 4; // the opponent gets a random leader
                    if (r == 0) opponentLeader = new FoltestLeader();
                    else if (r == 1) opponentLeader = new EmhyrLeader();
                    else if (r == 2) opponentLeader = new EredinLeader();
                    else opponentLeader = new FrancescaLeader();

                    state = GameState::GameScreen;
                }
                else {
                    selectedLeader = i;
                }
            }
        }
        return;
    }

    // final screen
    if (state == GameState::OverviewScreen) {
        glm::ivec2 resolution = window->GetResolution();
        float x = mouseX * LOGIC_W / resolution.x;
        float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;

        if (x >= LOGIC_W / 2 - 120 && x <= LOGIC_W / 2 + 120 && y >= 260 - 40 && y <= 260 + 40) {
            StartGame();
            state = GameState::LeaderSelectScreen;
        }
        return;
    }
    
    // Add mouse button press event
    if (gameOver) {
        return;
    }

    if (opponentTurn) return;

    // cout << "Pixel x = " << mouseX << " Pixel y = " << mouseY << "\n";
    glm::ivec2 resolution = window->GetResolution();

    // because the mouse coordinates are in pixels and the cards in logical coordinates, i should convert the mouse ones
    float x = mouseX * LOGIC_W / resolution.x;
    float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;
    // cout << "Logical x = " << x << " Logical y = " << y << "\n";

    // logic when a card is selected
    for (int i = 0; i < player->GetHand().size(); i++) {
        if (player->GetHand()[i]->IsMouseOver(x, y, HAND_SCALE)) {
            selectedCard = player->GetHand()[i];
            // cout << "picked " << selectedCard->GetPos().x << "\n";
        }
    }

    // use leaders ability
    if (glm::distance(glm::vec2(x, y), glm::vec2(60, 270)) <= 50) {
        playerLeader->Activate(*player, *opponent);
        return;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    glm::ivec2 resolution = window->GetResolution();
    float x = mouseX * LOGIC_W / resolution.x;
    float y = (resolution.y - mouseY) * LOGIC_H / resolution.y;

    if (selectedCard != nullptr && selectedCard->GetType() == CardType::Decoy) {
        for (int t = 0; t < player->GetBoard().size(); t++) {
            if (player->GetBoard()[t]->IsMouseOver(x, y)) {
                for (int i = 0; i < player->GetHand().size(); i++) {
                    if (player->GetHand()[i] == selectedCard) {
                        player->GetHand().erase(player->GetHand().begin() + i);
                        break;
                    }
                }
                UseDecoy(player, selectedCard, t);
                player->SetLastCardPlayed(selectedCard);
                UpdateRow(player->GetHand(), 60, HAND_SCALE);
                LayoutBoard(player, true);
                opponentTurn = true;
                opponentDelay = 0;
                break;
            }
        }
        selectedCard = nullptr;
        return;
    }

    // move the card from the players inventory to the board
    bool foundLane = false;
    Lane droppedLane = Lane::Melee;
    for (int l = 0; l < 3; l++) {
        if (abs(y - LaneY((Lane)l, true)) <= 40) {
            droppedLane = (Lane)l;
            foundLane = true;
        }
    }

    bool onBoard = x >= LOGIC_W / 2 - 70 - 450 && x <= LOGIC_W / 2 - 70 + 450;

    if (onBoard && foundLane && selectedCard != nullptr && selectedCard->CanBeOnLane(droppedLane)) {
        {
            selectedCard->SetLane(droppedLane);
            for (int i = 0; i < player->GetHand().size(); i++) {
                if (player->GetHand()[i] == selectedCard) {
                    player->GetHand().erase(player->GetHand().begin() + i); // can't use pop_back() bc the choosen card isn't necessary the last one from hand
                    player->GetBoard().push_back(selectedCard); // add the card to the board
                    player->SetLastCardPlayed(selectedCard);

                    // when war horn is on the board, the next card should double its value
                    if (player->GetDoubleNextCard()) {
                        selectedCard->ModifyValue(selectedCard->GetValue());
                        player->SetDoubleNextCard(false); // after the card gets doubled I should mark that
                    }
                    
                    break;
                }
            }
            UpdateRow(player->GetHand(), 60, HAND_SCALE);
            LayoutBoard(player, true);

            opponentTurn = true;
            opponentDelay = 0;
        }
        // cout << "on board\n";
    }

    selectedCard = nullptr; // without this the card gets stuck on the mouse
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
