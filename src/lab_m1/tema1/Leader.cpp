#include "lab_m1/tema1/Leader.h"

void Leader::Activate(Player& owner, Player& opponent) {
	if (!didThings) {
		UseAbility(owner, opponent);
		didThings = true;
	}
}

bool Leader::IsUsed() const{
	return didThings;
}