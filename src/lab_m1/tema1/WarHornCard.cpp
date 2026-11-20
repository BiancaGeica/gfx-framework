#include "lab_m1/tema1/WarHornCard.h"

WarHornCard::WarHornCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 WarHornCard::GetColor() const {
	return glm::vec3(0.97f, 0.58f, 0.28f); // orange
}

CardType WarHornCard::GetType() const {
	return CardType::WarHorn;
}

int WarHornCard::GetPriority() const {
	return 4;
}

void WarHornCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	// doubles the value of the next card played by the same player
	owner.SetDoubleNextCard(true);
}

