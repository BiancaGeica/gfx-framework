#include "lab_m1/tema1/EmhyrLeader.h"
#include "lab_m1/tema1/Card.h"
#include <cstdlib>

std::string EmhyrLeader::GetMeshName() const {
	return "leader_triangle";
}

glm::vec3 EmhyrLeader::GetColor() const {
	return glm::vec3(0.1f, 0.4f, 0.2f); // dark green
}

void EmhyrLeader::UseAbility(Player& owner, Player& opponent) {
	// reveals 3 random cards from the opponent's hand
	std::vector<Card*>& hand = opponent.GetHand();
	int revealed = 0;

	// pick random cards
	// if one is already revealed, try again
	// max 21 tries so it can't loop forever
	for (int i = 0; i < 21 && revealed < 3 && !hand.empty(); i++) { // i is the no of tries
		Card* card = hand[rand() % hand.size()];
		if (!card->IsFaceUp()) {
			card->SetFaceUp(true);
			card->StartFlip(revealed * 0.2f); // the flip of the opponent cards
			revealed++;
		}
	}
}

std::string EmhyrLeader::GetName() const {
	return "EMHYR";
}
