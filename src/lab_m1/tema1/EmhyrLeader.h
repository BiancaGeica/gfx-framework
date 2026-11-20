#pragma once
#include "lab_m1/tema1/Leader.h"
#include "lab_m1/tema1/Card.h"

class EmhyrLeader : public Leader
{
public:
	std::string GetMeshName() const override;
	std::string GetName() const override;
	glm::vec3 GetColor() const override;

protected:
	void UseAbility(Player& owner, Player& opponent) override;
};

