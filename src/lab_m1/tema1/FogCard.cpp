#include "lab_m1/tema1/FogCard.h"

FogCard::FogCard(int value) : TemporaryCard(value, Lane::Ranged) {}

glm::vec3 FogCard::GetColor() const {
	return glm::vec3(0.7f, 0.7f, 0.7f); // light gray
}

CardType FogCard::GetType() const {
	return CardType::FogWeather;
}

int FogCard::GetPriority() const {
	return 3;
}

void FogCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	for (int i = 0; i < opponent.GetBoard().size(); i++) {
		if (opponent.GetBoard()[i]->GetLane() == Lane::Ranged) {
			opponent.GetBoard()[i]->ModifyValue(-1);
		}
	}
}

bool FogCard::CanBeOnLane(Lane newLane) const {
	return newLane == lane; // weather goes only on its own lane
}