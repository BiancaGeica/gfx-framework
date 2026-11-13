#include "lab_m1/tema1/DamageCard.h"

DamageCard::DamageCard(int value, Lane lane) : PermanentCard(value, lane) {}

glm::vec3 DamageCard::GetColor() const {
	return glm::vec3(1, 0, 0);
}

CardType DamageCard::GetType() const {
	return CardType::Damage;
}

int DamageCard::GetPriority() const {
	return 6; // the lower number has a higher priority
}

void DamageCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	opponent.TakeDamage(value);
}
