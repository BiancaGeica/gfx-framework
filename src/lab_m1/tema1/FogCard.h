#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class FogCard : public TemporaryCard // without public TemporaryCard remains private and game couldn't treat a frostCard as a card
{
public:
	FogCard(int value); // Fog card is always on Ranged

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Player& owner, Player& opponent) override;
	bool CanBeOnLane(Lane newLane) const override;
};