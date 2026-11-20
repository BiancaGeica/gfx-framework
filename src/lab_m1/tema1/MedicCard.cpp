#include <cstdlib>
#include "lab_m1/tema1/MedicCard.h"

MedicCard::MedicCard(int value, Lane lane) : TemporaryCard(value, lane) {}

glm::vec3 MedicCard::GetColor() const {
	return glm::vec3(1.0f, 0.5f, 0.8f);
}

CardType MedicCard::GetType() const {
	return CardType::Medic;
}

int MedicCard::GetPriority() const {
	return 1;
}

void MedicCard::ApplyTurnEffect(Player& owner, Player& opponent) {
	if (!owner.GetUsedCards().empty()) {
		int random = rand() % owner.GetUsedCards().size();
		Card* revived = owner.GetUsedCards()[random];
		owner.GetUsedCards().erase(owner.GetUsedCards().begin() + random);
		revived->ResetValue();
		revived->SetFaceUp(false);
		owner.GetHand().push_back(revived);
	}
}
