#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class ScorchCard : public TemporaryCard
{
public:
	ScorchCard(int value, Lane lane);

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Game& game, Player& owner) override;
};

