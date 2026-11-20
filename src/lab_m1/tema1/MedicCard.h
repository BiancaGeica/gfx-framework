#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class MedicCard : public TemporaryCard
{
public:
	MedicCard(int value, Lane lane);

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Player& owner, Player& opponent) override;
};
