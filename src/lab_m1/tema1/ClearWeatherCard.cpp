#include "lab_m1/tema1/ClearWeatherCard.h"

ClearWeatherCard::ClearWeatherCard(int value, Lane lane) : TemporaryCard(value, lane){}

glm::vec3 ClearWeatherCard::GetColor() const {
	return glm::vec3(1, 1, 0); // yellow
}

CardType ClearWeatherCard::GetType() const {
	return CardType::ClearWeather;
}

int ClearWeatherCard::GetPriority() const {
	return 2;
}

void ClearWeatherCard::ApplyTurnEffect(Game& game, Player& owner) {
	// TO-DO: when I implement game and player add this
	// remove all weather effects for both players
}
