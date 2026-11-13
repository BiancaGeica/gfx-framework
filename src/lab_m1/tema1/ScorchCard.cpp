#include "lab_m1/tema1/ScorchCard.h"

ScorchCard::ScorchCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 ScorchCard::GetColor() const {
	return glm::vec3(1, 0, 1); // purple
}

CardType ScorchCard::GetType() const {
	return CardType::Scorch;
}

int ScorchCard::GetPriority() const {
	return 0;
}

void ScorchCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	// TO-DO: when I implement game and player add this
	// cancels the opponent's card effect in the current turn if the opponent's card value is grater or equal with V 
}
