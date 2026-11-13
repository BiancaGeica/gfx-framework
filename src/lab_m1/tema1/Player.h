#pragma once
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

protected:
	int hp;
	int maxHp;
	int shield;
};

