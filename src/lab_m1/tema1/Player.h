#pragma once
#include <vector>

class Card; // can't include card.h because it already includes player

class Player
{
public:
	Player(int maxHp);

	void TakeDamage(int amount);
	void Heal(int amount);
	void AddShield(int amount);

	// getters
	int GetHp() const;
	int GetMaxHp() const;
	int GetShield() const;
	bool IsDone() const;
	bool GetDoubleNextCard() const;

	std::vector<Card*>& GetBoard(); //& for the real vector, not just a copy
	Card* GetLastCardPlayed() const;

	// setters
	void SetLastCardPlayed(Card* card);
	void SetDoubleNextCard(bool something);

protected:
	int hp;
	int maxHp;
	int shield;

	std::vector<Card*> board;
	Card* lastCardPlayed = nullptr;

	// for War horn
	bool doubleNextCard = false;
};

