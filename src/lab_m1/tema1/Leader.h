#pragma once
#include <string>
#include "utils/glm_utils.h"
#include "lab_m1/tema1/Player.h"

class Leader
{
public:
	virtual ~Leader() = default;

	virtual std::string GetMeshName() const = 0; // get witch shape to draw
	virtual glm::vec3 GetColor() const = 0;
	virtual std::string GetName() const = 0;

	void Activate(Player& owner, Player& opponent);
	bool IsUsed() const;

protected:
	virtual void UseAbility(Player& owner, Player& opponent) = 0; // I should know what every leader do
	bool didThings = false;
};

