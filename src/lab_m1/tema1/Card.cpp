#include "lab_m1/tema1/Card.h"

const float Card::WIDTH = 50.0f;
const float Card::HEIGHT = 75.0f;

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

void Card::OnPlay(Player& owner, Player& opponent) {
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

bool Card::IsMouseOver(float x, float y, float scale) const {
	float xMin = pos.x - WIDTH * scale / 2; // pos = (0,0) = the center of the card
	float xMax = pos.x + WIDTH * scale / 2;
	float yMin = pos.y - HEIGHT * scale / 2;
	float yMax = pos.y + HEIGHT * scale / 2;
	
	if (xMin <= x && xMax >= x && yMin <= y && yMax >= y)
		return true;

	return false;
}

void Card::UpdateAnimation(float deltaTime) {
	const float SPEED = 10.0f;
	float t = glm::min(1.0f, SPEED * deltaTime); // t = the % of the distance to the target on each frame, ADDED MIN BECAUSE SOMETIMES IT GOES SOMEWHERE RANDOM
	pos += (targetPos - pos) * t; // targetPos - pos = distance left to the tsrget

	// for emhyr action
	// the flip advances from 0 to 1 in 0.3 seconds (negative = still waiting)
	flipProgress = glm::min(1.0f, flipProgress + deltaTime / 0.3f);
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

std::string Card::GetLabel() const {
	switch (GetType()) {
	case CardType::Damage: return "DMG";
	case CardType::Heal: return "HEAL";
	case CardType::Shield: return "SHLD";
	case CardType::Spy: return "SPY";
	case CardType::FrostWeather: return "FRST";
	case CardType::FogWeather: return "FOG";
	case CardType::RainWeather: return "RAIN";
	case CardType::ClearWeather: return "CLR";
	case CardType::WarHorn: return "HORN";
	case CardType::Scorch: return "SCRH";
	case CardType::Decoy: return "DCOY";
	case CardType::Medic: return "MED";
	}
	return "";
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

void Card::StartFlip(float delay) {
	flipProgress = -delay / 0.3f;
}

float Card::GetFlipProgress() const {
	return flipProgress;
}

void Card::SetLane(Lane newLane) {
	lane = newLane;
}
