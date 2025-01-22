#include"GameHeaders.h"

// Constructor
Projectile::Projectile() {
    this->setRadius(5.f);
    this->setFillColor(sf::Color::Red);
    this->velocity = 20.f;
}

// Destructor
Projectile::~Projectile(){

}

// Set velocity
void Projectile::setVelocity(float vel) {
    this->velocity = vel;
}