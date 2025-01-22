#pragma once

class Platform : public GameObject{
private:
	sf::IntRect currentFrame;
	int movementPattern;
	// Physics
	float velocity;
	float move_distance;
	float count_moves;
	// Core
	void initPlatform();

public:
	// Constructors & Destructors
	Platform();
	virtual ~Platform();

	//Modifiers
	void resetVelocity();
	void setVelocity(float);
	void setMoveDistance(float);
	void setMovementPattern(int);

	//Functions	
	void updatePhysics();
	void updateMovement();
	void update();
	void render(sf::RenderTarget& target);
};