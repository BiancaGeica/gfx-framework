#include "lab_m1/tema1/Card.h"

const float Card::WIDTH = 80.0f;
const float Card::HEIGHT = 120.0f;

Card::Card(int value, Lane lane) :
	value(value), // initializer list
	baseValue(value),
	lane(lane),
	pos(0.0f, 0.0f),
	targetPos(0.0f, 0.0f),
	faceUp(false),
	flipProgress(0.0f),
	cancelled(false)
{} // body

void Card::OnPlay(Game& game, Player& owner) {
	// For Spy, Medic and Decoy
}

void Card::AfterTurn() {
	// For TemporaryCard
}

bool Card::IsTemporary() const {
	return false;
}

bool Card::CanBeOnLane(Lane newLane) const {
	return lane == newLane;
}

bool Card::IsMouseOver(float x, float y) const {
	float xMin = pos.x - WIDTH / 2; // pos = (0,0) - the center of the card
	float xMax = pos.x + WIDTH / 2;
	float yMin = pos.y - HEIGHT / 2;
	float yMax = pos.y + HEIGHT / 2;
	
	if (xMin <= x && xMax >= x && yMin <= y && yMax >= y)
		return true;

	return false;
}

void Card::UpdateAnimation(float deltaTime) {
	const float SPEED = 10.0f;
	float t = glm::min(1.0f, SPEED * deltaTime); // t = the % of the distance to the target on each frame, ADDED MIN BECAUSE SOMETIMES IT GOES SOMEWHERE RANDOM
	pos += (targetPos - pos) * t; // targetPos - pos = distance left to the tsrget
}

void Card::ModifyValue(int delta) {
	value += delta;
}

void Card::ResetValue() {
	value = baseValue;
	cancelled = false;
}

bool Card::IsDestroyed() const {
	if (value <= 0) {
		return true;
	}

	return false;
}

// getters
int Card::GetValue() const {
	return value;
}

Lane Card::GetLane() const {
	return lane;
}

glm::vec2 Card::GetPos() const {
	return pos;
}

bool Card::IsFaceUp() const {
	return faceUp;
}

bool Card::IsCancelled() const {
	return cancelled;
}

// setters
void Card::SetPos(glm::vec2 newPos) {
	pos = newPos;
}

void Card::SetTargetPos(glm::vec2 newTarget) {
	targetPos = newTarget;
}

void Card::SetFaceUp(bool up) {
	faceUp = up;
}

void Card::SetCancelled(bool c) {
	cancelled = c;
}