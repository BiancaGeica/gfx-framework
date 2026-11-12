#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class ClearWeatherCard : public TemporaryCard
{
public:
	ClearWeatherCard(int value, Lane lane); // can be on any lane

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Game& game, Player& owner) override;
};

