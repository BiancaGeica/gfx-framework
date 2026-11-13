#include "lab_m1/tema1/Player.h"
#include "utils/glm_utils.h"

Player::Player(int maxHp) :
	maxHp(maxHp),
	hp(maxHp),
	shield(0)
{}

void Player::TakeDamage(int amount) {
	int minimum = glm::min(shield, amount);
	shield -= minimum;
	amount -= minimum;

	hp = ((hp - amount) < 0) ? 0 : (hp - amount);
}

void Player::Heal(int amount) {
	hp = ((amount + hp) <= maxHp) ? (amount + hp) : maxHp;
}

void Player::AddShield(int amount) {
	shield += amount;
}

// getters
int Player::GetHp() const {
	return hp;
}

int Player::GetMaxHp() const {
	return maxHp;
}

int Player::GetShield() const {
	return shield;
}

bool Player::IsDone() const {
	return (hp <= 0);
}

std::vector<Card*>& Player::GetBoard() {
	return board;
}

Card* Player::GetLastCardPlayed() const {
	return lastCardPlayed;
}

void Player::SetLastCardPlayed(Card* card) {
	lastCardPlayed = card;
}