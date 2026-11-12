#include "lab_m1/tema1/FrostCard.h"

FrostCard::FrostCard(int value) : TemporaryCard(value, Lane::Melee){}

glm::vec3 FrostCard::GetColor() const {
	return glm::vec3(1, 1, 1);
}

CardType FrostCard::GetType() const {
	return CardType::FrostWeather;
}

int FrostCard::GetPriority() const {
	return 3;
}

void FrostCard::ApplyTurnEffect(Game& game, Player& owner) {
	// TO-DO: when I implement game and player add this
	// lower the value of all the opponent's cards by 1
}