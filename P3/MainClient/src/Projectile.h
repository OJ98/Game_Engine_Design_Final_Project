#pragma once

class Projectile : public sf::CircleShape {
private:
    float velocity;

public:   
    Projectile();
    virtual ~Projectile();

    void setVelocity(float);
};