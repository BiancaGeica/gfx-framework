#include "lab_m1/tema1/RainCard.h"

RainCard::RainCard(int value) : TemporaryCard(value, Lane::Siege) {}

glm::vec3 RainCard::GetColor() const {
	return glm::vec3(0.13f, 0.71f, 0.66f); // light blue
}

CardType RainCard::GetType() const {
	return CardType::RainWeather;
}

int RainCard::GetPriority() const {
	return 3;
}

void RainCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	for (int i = 0; i < opponent.GetBoard().size(); i++) {
		if (opponent.GetBoard()[i]->GetLane() == Lane::Siege) {
			opponent.GetBoard()[i]->ModifyValue(-1);
		}
	}
}

bool RainCard::CanBeOnLane(Lane newLane) const {
	return newLane == lane; // weather goes only on its own lane
}