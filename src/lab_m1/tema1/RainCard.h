#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class RainCard : public TemporaryCard
{
public:
	RainCard(int value); // Rain card is always on Siege

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Player& owner, Player& opponent) override;
	bool CanBeOnLane(Lane newLane) const override;
};