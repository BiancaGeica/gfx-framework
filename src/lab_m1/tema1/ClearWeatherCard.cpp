#include "lab_m1/tema1/ClearWeatherCard.h"

ClearWeatherCard::ClearWeatherCard(int value, Lane lane) : TemporaryCard(value, lane){}

glm::vec3 ClearWeatherCard::GetColor() const {
	return glm::vec3(0.89f, 0.91f, 0.35f); // yellow
}

CardType ClearWeatherCard::GetType() const {
	return CardType::ClearWeather;
}

int ClearWeatherCard::GetPriority() const {
	return 2;
}

void ClearWeatherCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	// remove all weather effects for both players
	for (int i = 0; i < owner.GetBoard().size(); i++) { // for every card on the player board
		Card* card = owner.GetBoard()[i];
		if (card->GetType() == CardType::FrostWeather || card->GetType() == CardType::FogWeather ||card->GetType() == CardType::RainWeather) { // check if it is FrostWeather
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
