#include "lab_m1/tema1/DecoyCard.h"

DecoyCard::DecoyCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 DecoyCard::GetColor() const {
	return glm::vec3(0.80f, 0.71f, 0.86f);
}

CardType DecoyCard::GetType() const {
	return CardType::Decoy;
}

int DecoyCard::GetPriority() const {
	return 0;
}

void DecoyCard::ApplyTurnEffect(Player& owner, Player& opponent) {
}
