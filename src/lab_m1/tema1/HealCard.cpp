#include "lab_m1/tema1/HealCard.h"

HealCard::HealCard(int value, Lane lane) : PermanentCard(value, lane){}

glm::vec3 HealCard::GetColor() const {
	return glm::vec3(0, 1, 0);
}

CardType HealCard::GetType() const {
	return CardType::Heal;
}

int HealCard::GetPriority() const {
	return 7; // the lower number has a higher priority
}

void HealCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	owner.Heal(value);
}

