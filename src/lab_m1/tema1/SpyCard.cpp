#include <cstdlib>
#include "lab_m1/tema1/SpyCard.h"

SpyCard::SpyCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 SpyCard::GetColor() const {
	// return glm::vec3(0, 0, 0); // black
	return glm::vec3(0.42f, 0.38f, 0.45f); // bc the background is black i can't see the card
}

CardType SpyCard::GetType() const {
	return CardType::Spy;
}

int SpyCard::GetPriority() const {
	return 1;
}

void SpyCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	// steal a card from the opponent hand and pass it to the players hand
	if (!opponent.GetHand().empty()) {
		int random = rand() % opponent.GetHand().size();
		Card* stolen = opponent.GetHand()[random];
		opponent.GetHand().erase(opponent.GetHand().begin() + random);
		owner.GetHand().push_back(stolen);
	}
}
