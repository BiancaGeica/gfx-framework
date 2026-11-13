#pragma once
#include "lab_m1/tema1/TemporaryCard.h"

class FrostCard : public TemporaryCard // without public TemporaryCard remains private and game couldn't treat a frostCard as a card
{
public:
	// FrostCard(int value, Lane lane);
	FrostCard(int value); // Frost card is always on Melee

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Player& owner, Player& opponent) override;
};

