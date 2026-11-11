#pragma once
#include "lab_m1/tema1/PermanentCard.h"

class HealCard : public PermanentCard
{
public:
	HealCard(int value, Lane lane);

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Game& game, Player& owner) override;
};

