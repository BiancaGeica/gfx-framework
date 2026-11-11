#include "lab_m1/tema1/TemporaryCard.h"

TemporaryCard::TemporaryCard(int value, Lane lane) : Card(value, lane) {
}

void TemporaryCard::AfterTurn() { // :Card() is only for constructors, not this
	ModifyValue(-1);
}

bool TemporaryCard::IsTemporary() const{
	return true;
}

bool TemporaryCard::CanBeOnLane(Lane newLane) const{
	return true; // temporary cards can go on any line, so the value is true anytime
}
