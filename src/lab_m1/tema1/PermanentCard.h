#pragma once
#include "lab_m1/tema1/Card.h"


class PermanentCard : public Card
{
protected: // in a class everything is private till I make it public, if I don't make the function public things like the constructor cannot be called
	PermanentCard(int value, Lane lane);
};
