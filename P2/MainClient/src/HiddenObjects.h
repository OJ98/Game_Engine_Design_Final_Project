#pragma once

class HiddenObjects : public GameObject{
private:
    sf::Vector2f position;
    // Denotes which type of Hidden Object (0 = Death, 1 = Spawn)
    int HOtype;
    HiddenObjects* closestSpawn;
public:
	HiddenObjects();
	virtual ~HiddenObjects();

    //Modifiers
    void setclosestSpawn(HiddenObjects*);
    void setHOType(int);

	//Functions	
	void performAction(Player*);
};