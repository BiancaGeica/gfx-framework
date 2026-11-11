#include "lab_m1/tema1/ShieldCard.h"

ShieldCard::ShieldCard(int value, Lane lane) : PermanentCard(value, lane) {}

glm::vec3 ShieldCard::GetColor() const {
	return glm::vec3(0, 0, 1);
}

CardType ShieldCard::GetType() const {
	return CardType::Shield;
}

int ShieldCard::GetPriority() const {
	return 5; // the lower number has a higher priority
}

void ShieldCard::ApplyTurnEffect(Game& game, Player& owner) {
	// TO-DO: when I implement Game and Player add owner.AddShield(value);
}

