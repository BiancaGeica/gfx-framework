#include "lab_m1/tema1/ScorchCard.h"

ScorchCard::ScorchCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 ScorchCard::GetColor() const {
	return glm::vec3(0.62f, 0.40f, 0.75f); // purple
}

CardType ScorchCard::GetType() const {
	return CardType::Scorch;
}

int ScorchCard::GetPriority() const {
	return 0;
}

void ScorchCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	// cancels the opponent's card effect in the current turn if the opponent's card value is grater or equal with V 
	Card* lastCard = opponent.GetLastCardPlayed();

	if (lastCard != nullptr && lastCard->GetValue() >= value) {
		lastCard->SetCancelled(true);
	}
}
