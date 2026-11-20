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

void Player::LoseCrystal() {
	crystals--;
}

void Player::ResetHp() {
	hp = maxHp;
	shield = 0;
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

bool Player::GetDoubleNextCard() const {
	return doubleNextCard;
}

std::vector<Card*>& Player::GetHand() {
	return hand;
}

int Player::GetCrystals() const {
	return crystals;
}

//setters

void Player::SetLastCardPlayed(Card* card) {
	lastCardPlayed = card;
}

void Player::SetDoubleNextCard(bool something) {
	doubleNextCard = something;
}

std::vector<Card*>& Player::GetUsedCards() {
	return usedCards;
}
