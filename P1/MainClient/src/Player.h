#pragma once

class Player : public GameObject{
private:
	//Rendering
	sf::Sprite sprite;
	sf::Texture texture;
	sf::IntRect spriteSize;

	//Physics
	sf::Vector2f velocity;
	sf::FloatRect nextPosition;
	float velocityMin;
	float acceleration;
	float dragFactor;
	float gravity;
	float velocityMax;
	zmq::socket_ref sock;
	bool dash;

	//Networking
	std::string NetID;

	//Event Handling
	EventHandler* EH;

	//Time Management;
	Timeline* tlplayer;
	Timeline* anchor_tl;
	int64_t ticsize;
	
	//Initialization
	void initTexture();
	void initSprite();
	void initPhysics();
	void initTimeline(Timeline*);
	void initNetworking();
	void initEventHandling(EventHandler*);

public:
	Player(EventHandler*, Timeline*);
	virtual ~Player();

	//Accessors
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getNextPosition();
	std::string getNetworkID();

	//Modifiers
	void setPosition(const float x, const float y);
	void setNetworkID(std::string);
	void resetVelocityY();
	void resetVelocityX();

	//Handlers
	void handleInputs();
	void handleDash();

	//Functions	
	void updatePhysics();
	bool updateMovement();
	void updatePosition();
	void updateTimeline();
	void updatefromInput();
	void updatefromNetwork(sf::Vector2f);
	void render(sf::RenderTarget& target);

	//Helper Functions
	sf::Vector2f replayParser(std::string);
	std::string replayPos();
	void replayMove(std::string);
};