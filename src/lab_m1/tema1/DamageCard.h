#pragma once
#include "lab_m1/tema1/PermanentCard.h"

// red, lowers the oponnent's life by V

class DamageCard : public PermanentCard
{
public:
	DamageCard(int value, Lane lane);

	glm::vec3 GetColor() const override;
	CardType GetType() const override;
	int GetPriority() const override;
	void ApplyTurnEffect(Player& owner, Player& opponent) override;
};

