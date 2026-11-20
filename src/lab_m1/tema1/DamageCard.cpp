#include "lab_m1/tema1/DamageCard.h"

DamageCard::DamageCard(int value, Lane lane) : PermanentCard(value, lane) {}

glm::vec3 DamageCard::GetColor() const {
	return glm::vec3(0.93f, 0.36f, 0.36f);
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
