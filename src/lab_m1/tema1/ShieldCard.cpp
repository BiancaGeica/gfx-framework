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

void ShieldCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	owner.AddShield(value);
}

