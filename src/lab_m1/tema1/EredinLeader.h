#pragma once
#include "lab_m1/tema1/Leader.h"

class EredinLeader : public Leader
{
public:
	std::string GetMeshName() const override;
	std::string GetName() const override;
	glm::vec3 GetColor() const override;

protected:
	void UseAbility(Player& owner, Player& opponent) override;
};