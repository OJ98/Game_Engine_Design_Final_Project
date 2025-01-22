#include"GameHeaders.h"

// Constructor
Platform::Platform() {
    this->initPlatform();
}
// Destructor
Platform::~Platform(){
}

// Initializes the Platform class with default values
void Platform::initPlatform() {
	this->setSize(sf::Vector2f(10,10));
    this->setFillColor(sf::Color::White);
    this->setPosition(0, 0);
	this->move_distance = 0.f;
    this->velocity = 0.f;
	this->vert_vel = 0.f;
	this->count_moves = 0;
	this->movementPattern = 0;
}

// Resets Platform velocity for smooth movement.
void Platform::resetVelocity() {
	if(this->velocity == 1.f) {
		this->velocity = -1.f;
	}
	else if(this->velocity == -1.f) {
		this->velocity = 1.f;
	}
	else
		this->velocity = 0.f;
}

// Sets Platform velocity
void Platform::setVelocity(float new_velocity) {
	this->velocity = new_velocity;
}

// Set Vertical velocity
void Platform::setVertVelocity(float new_velocity) {
	this->vert_vel = new_velocity;
}

// Sets the movement pattern
void Platform::setMovementPattern(int input) {
	this->movementPattern = input;
	return;
}

// Sets maximum movement allowed for Platform
void Platform::setMoveDistance(float new_moveDistance) {
	this->move_distance = new_moveDistance;
}

// Moves Platform Object
void Platform::update() {
	switch(this->movementPattern) {
		case 0:
			if(this->move_distance > this->count_moves) {
				this->setPosition(this->getPosition().x + this->velocity, this->getPosition().y);
				this->count_moves++;
			}
			else {
				this->resetVelocity();
				this->count_moves = 0;
			}
			break;
		case 1:
			if(this->move_distance > this->count_moves) {
				this->setPosition(this->getPosition().x, this->getPosition().y + this->velocity);
				this->count_moves++;
			}
			else {
				this->resetVelocity();
				this->count_moves = 0;
			}
			break;
		case 2:
			if(this->move_distance > this->count_moves) {
				this->setPosition(this->getPosition().x + this->velocity, this->getPosition().y + this->vert_vel);
				this->count_moves++;
			}
			else {
				this->resetVelocity();
				this->setPosition(this->getPosition().x, this->getPosition().y + this->vert_vel);
				this->count_moves = 0;
			}
			break;
	}

}

// Modifies platform ID
std::string Platform::getID() {
	return guid;
}

// Renders Platform to window
void Platform::render(sf::RenderTarget& target) {
	target.draw(*this);
}