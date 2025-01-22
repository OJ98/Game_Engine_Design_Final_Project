#pragma once

class Platform : public GameObject{
private:
	sf::IntRect currentFrame;
	int movementPattern;
	// Physics
	float velocity;
	float vert_vel;
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
	void setVertVelocity(float);
	void setMoveDistance(float);
	void setMovementPattern(int);

	//Accessors
	std::string getID();

	//Functions	
	void updatePhysics();
	void updateMovement();
	void update();
	void render(sf::RenderTarget& target);
};