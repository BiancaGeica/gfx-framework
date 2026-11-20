#include "lab_m1/tema1/FoltestLeader.h"
#include "lab_m1/tema1/Card.h"

std::string FoltestLeader::GetMeshName() const {
	return "leader_circle";
}

glm::vec3 FoltestLeader::GetColor() const {
	return glm::vec3(1, 0.85f, 0.3f); // gold
}

void FoltestLeader::UseAbility(Player& owner, Player& opponent) {
	// clears all weather on both boards, like Clear Weather
	for (int i = 0; i < owner.GetBoard().size(); i++) { // for every card on the player board
		Card* card = owner.GetBoard()[i];
		if (card->GetType() == CardType::FrostWeather || card->GetType() == CardType::FogWeather || card->GetType() == CardType::RainWeather) { // check if it is FrostWeather
			card->ModifyValue(-card->GetValue()); // kill it
		}
	}

	for (int i = 0; i < opponent.GetBoard().size(); i++) {
		Card* card = opponent.GetBoard()[i];
		if (card->GetType() == CardType::FrostWeather || card->GetType() == CardType::FogWeather || card->GetType() == CardType::RainWeather) { // check if it is FrostWeather
			card->ModifyValue(-card->GetValue()); // kill it
		}
	}
}

std::string FoltestLeader::GetName() const {
	return "FOLTEST";
}
