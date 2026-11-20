#include "lab_m1/tema1/EredinLeader.h"
#include "lab_m1/tema1/Card.h"

std::string EredinLeader::GetMeshName() const {
	return "leader_hexagon";
}

glm::vec3 EredinLeader::GetColor() const {
	return glm::vec3(0.6f, 0.9f, 1.0f); // icy blue
}

void EredinLeader::UseAbility(Player& owner, Player& opponent) {
	// doubles the value of all the player's melee cards
	for (int i = 0; i < owner.GetBoard().size(); i++) {
		Card* card = owner.GetBoard()[i];
		if (card->GetLane() == Lane::Melee) {
			card->ModifyValue(card->GetValue()); // value + value = double
		}
	}
}

std::string EredinLeader::GetName() const {
	return "EREDIN";
}
