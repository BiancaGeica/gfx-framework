#pragma once // includes the file a single time

#include "utils/glm_utils.h" // glm::vec2, glm::vec3

class Game;
class Player; // declared before because Card is using them

enum class Lane {Melee, Ranged, Siege, DoesntMatter};
enum class CardType {Damage, Heal, Shield, // permanent classes
					 Spy, FrostWeather, FogWeather, RainWeather, ClearWeather, // Decoy = Momeala
					 WarHorn, Scorch, Decoy, Medic}; // temporary classes

class Card
{
public:
	Card(int value, Lane lane); // constructor (the only values that aren't known when the card is create), constructors cannot be virtuals
							    // the constructor receives the values that are different from one card to another
	virtual ~Card() = default; // virtual destructor, should be virtual to make sure the memory is freed, 
							  //default = message for compilator to automatically generate the standard destructor

	virtual glm::vec3 GetColor() const = 0; // virtual = every derivative class could make her own version of this function
											// glm::vec3 = the function returns a vector made of 3 elements (R, G, B)
											// const = a promise that the function only reads, without writing anything
											// = 0 => card doesn't have her own version so every single card is ~*~forced~*~ to write one
	virtual CardType GetType() const = 0;
	virtual int GetPriority() const = 0;
	virtual void ApplyTurnEffect(Game& game, Player& owner) = 0; // receives the game and the current player that has the card
																 // this one is not const because it should change things

	// Optionals, so I don't mark them with = 0 to force their implementation, I don't need them yet for every card
	virtual void OnPlay(Game& game, Player& owner); // only for Spy, Medic and Decoy
	virtual void AfterTurn();
	virtual bool IsTemporary() const; // const is used so I don't modify the object, just read it
	virtual bool CanBeOnLane(Lane newLane) const; // verify if the card could be placed on the specified lane

	// functions for all cards, DON'T OVERRIDE THEM or something
	bool IsMouseOver(float x, float y) const;
	void UpdateAnimation(float deltaTime);
	void ModifyValue(int delta);
	void ResetValue();
	bool IsDestroyed() const;

	// getters
	int GetValue() const;
	Lane GetLane() const;
	glm::vec2 GetPos() const;
	bool IsFaceUp() const;
	bool IsCancelled() const;

	// setters
	void SetPos(glm::vec2 newPos);
	void SetTargetPos(glm::vec2 newTarget);
	void SetFaceUp(bool up);
	void SetCancelled(bool c);

	static const float WIDTH; // static because I have a value for all, not for every single card
	static const float HEIGHT;

protected: // invisible outside, but visible in the derived classes
	int value;
	int baseValue;
	Lane lane;
	glm::vec2 pos;
	glm::vec2 targetPos;
	bool faceUp;
	float flipProgress;
	bool cancelled; // to see if the card is cancelled by Scorch in this turn
};

