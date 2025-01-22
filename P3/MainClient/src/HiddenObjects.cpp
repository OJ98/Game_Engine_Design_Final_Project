#include"GameHeaders.h"

//Constructor
HiddenObjects::HiddenObjects() {
    // Set size to default value (10,10)
    this->setSize(sf::Vector2f(10,10));
    // Set default value to offscreen to avoid unexpected behaviours
    this->setPosition(-100, -100);
    // Set default value to spawn (0)
    this->HOtype = 0;
    // Set closestSpawn value to NULL by default
    this->closestSpawn = NULL;
}

//Destructor
HiddenObjects::~HiddenObjects() {
}

//Set the value of the closest spawn
void HiddenObjects::setclosestSpawn(HiddenObjects* cs) {
    this->closestSpawn = cs;
    return;
}

//Set the value of the HOType
void HiddenObjects::setHOType(int i) {
    this->HOtype = i;
    return;
}

//Set the behaviour to be executed upon collision
//For now, it will simply check whether the hiddenobject is in death mode and teleport the player object to the spawn point recorded.
void HiddenObjects::performAction(Player* P) {
    if(this->HOtype == 1) {
        sf::Vector2f pos = this->closestSpawn->getPosition();
        //std::cout<<"DEATH REPORTED!"<<std::endl;
        P->setPosition(pos.x, pos.y);
    }
    return;
}