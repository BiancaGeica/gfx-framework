#pragma once
#include "lab_m1/tema1/Card.h" // I should include the inherited class

class TemporaryCard : public Card // to inherit a class : should be used
{
protected:
	TemporaryCard(int value, Lane lane);
	// virtual ~TemporaryCard() = default; Card's destructor also destroys this class, also default knows how to write only destructors, constructors without parameters and copies

public:
	void AfterTurn() override; // "virtual" = child classes could implement this function with their oun version
	bool IsTemporary() const override;
	bool CanBeOnLane(Lane newLane) const override;

	// getters and setters should't be rewritten by the child classes, they are inherited by everyone that inherit cards

};

