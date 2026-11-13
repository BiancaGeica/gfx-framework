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

void FrostCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	for (int i = 0; i < opponent.GetBoard().size(); i++) {
		opponent.GetBoard()[i]->ModifyValue(-1);
	}
}