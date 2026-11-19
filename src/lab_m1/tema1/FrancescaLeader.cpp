#include "lab_m1/tema1/FrancescaLeader.h"
#include "lab_m1/tema1/Card.h"

std::string FrancescaLeader::GetMeshName() const {
	return "leader_diamond";
}

glm::vec3 FrancescaLeader::GetColor() const {
	return glm::vec3(0.6f, 0.4f, 0.2f); // brown
}

void FrancescaLeader::UseAbility(Player& owner, Player& opponent) {
	// destroys the opponent's strongest card on the melee lane
	Card* strongest = nullptr;

	for (int i = 0; i < opponent.GetBoard().size(); i++) {
		Card* card = opponent.GetBoard()[i];
		if (card->GetLane() == Lane::Melee) {
			if (strongest == nullptr || card->GetValue() > strongest->GetValue()) {
				strongest = card;
			}
		}
	}

	if (strongest != nullptr) { // the opponent have no melee cards
		strongest->ModifyValue(-strongest->GetValue()); // kill it
	}
}