#include"GameHeaders.h"

// Constructor
Player::Player(EventHandler* EH, Timeline* T) {
	this->initTexture();
	this->initSprite();
	this->initPhysics();
	this->initNetworking();
	this->initTimeline(T);
	this->initEventHandling(EH);
	
	this->dash = false;

	zmq::context_t context{1};
    
    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
	socket.connect("tcp://localhost:5555");
	this->sock = sock;
}

// Destructor
Player::~Player() {
	delete this->anchor_tl;
	delete this->tlplayer;
	delete this->EH;
}

// Initializes Player texture from file.
void Player::initTexture() {
    if (!this->texture.loadFromFile("../src/Resources/bluebird-midflap.png"))
	{
		std::cout << "ERROR::PLAYER::Could not load the sprite texture." << "\n";
	}
}

// Initializes the Sprite object with a fixed size denoted by spriteSize.
void Player::initSprite()
{
    this->sprite.setTexture(this->texture);
	this->spriteSize = sf::IntRect(0, 0, 35, 20);

	this->sprite.setTextureRect(this->spriteSize);
	this->sprite.setScale(2.f, 2.f);
}

// Currently the physics system is a part of the player class as only the player is affected by gravity.
// This will be split off into a separate class soon.

// Initializes the Player physics system.
void Player::initPhysics() {
	this->dragFactor = 0.85f;
	this->gravity = 1.2f;
	this->velocityMin = 1.f;
	this->velocityMax = 15.f;
	this->velocity = sf::Vector2f(0,0);
}

// Initializes the Networking class
void Player::initNetworking() {
	this->NetID = "RegisterClient";
}

// Initializes Event Handling.
void Player::initEventHandling(EventHandler* EH) {
	this->EH = EH;
	this->EH->registerEvent("User_Input", std::bind(&Player::handleInputs, this));
	this->EH->registerEvent("Dash", std::bind(&Player::handleDash, this));
}

// Initializes Timelines.
void Player::initTimeline(Timeline* anchor) {
	this->tlplayer = new Timeline();
	tlplayer->setTicSize(50);
	this->ticsize = tlplayer->getTicSize();
	this->anchor_tl = anchor;

}

// Returns the sprite's position.
const sf::Vector2f Player::getPosition() const {
	return this->sprite.getPosition();
}

// Returns globalBounds for use in Collision Detection.
const sf::FloatRect Player::getGlobalBounds() const {
	return this->sprite.getGlobalBounds();
}

// Returns next position of sprite.
sf::FloatRect Player::getNextPosition() {
	return(this->nextPosition); 
}

// Returns the current Network ID.
std::string Player::getNetworkID() {
	return(this->NetID);
}


// Set network ID
void Player::setNetworkID(std::string new_ID) {
	this->NetID = new_ID;
	return;
}

// Sets sprite position to a specific point in the global coordinate system.
void Player::setPosition(const float x, const float y) {
	this->sprite.setPosition(x, y);
}

// Sets vertical velocity to zero.
void Player::resetVelocityY() {
	this->velocity.y = 0.f;
}

// Sets horizontal velocity to zero.
void Player::resetVelocityX() {
	this->velocity.x = 0.f;
}

// Handles Input Events.
void Player::handleInputs() {
	//std::cout<<"User Input Detected"<<std::endl;
}

// Handles the Dash event.
void Player::handleDash() {
	std::cout<<"Dash Event Detected"<<std::endl;
	this->velocity.x = 6 * this->velocity.x;
	this->dash = false;
}


// Applies the effect of gravity.
void Player::updatePhysics() {
	//Gravity
	this->velocity.y += this->gravity;
	if (std::abs(this->velocity.y) > this->velocityMax) {
		this->velocity.y = this->velocityMax * ((this->velocity.y < 0.f) ? -1.f : 1.f);
	}

	//Deceleration
	this->velocity *= this->dragFactor;

	//Limit deceleration
	if (std::abs(this->velocity.x) < this->velocityMin)
		this->velocity.x = 0.f;
	if (std::abs(this->velocity.y) < this->velocityMin)
		this->velocity.y = 0.f;

	this->sprite.move(this->velocity);
}

// Takes player input
bool Player::updateMovement() {
	bool hasmoved = false;
	this->dash = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
		this->dash = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) //Left
	{
		this->velocity.x = -5.f;
		hasmoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) //Right
	{
		this->velocity.x = 5.f;
		hasmoved = true;
	}
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) //Top
	{
		this->velocity.y = -5.f;
		hasmoved = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) //Down
	{
		this->velocity.y = 5.f;
		hasmoved = true;
	}

	if(this->dash && hasmoved) {
		json d;
		d["Event_Type"] = "Dash";
		ev D = make_tuple(this->anchor_tl->getTime_Absolute(), d);
		this->EH->raiseEvent(D);
		this->EH->handleEvent();
	}

	this->updatePosition();
	if(hasmoved) {
		json j;
		j["Event_Type"] = "User_Input";
		ev E = make_tuple(this->anchor_tl->getTime_Absolute(), j);
		this->EH->raiseEvent(E);
		this->EH->handleEvent();
	}
	return hasmoved;
}

// Update next position and move the sprite
void Player::updatePosition() {
	this->sprite.move(this->velocity);
	this->nextPosition = this->sprite.getGlobalBounds();
	this->nextPosition.left += this->velocity.x;
	this->nextPosition.top += this->velocity.y;
	return;
}

// Update the player timeline
void Player::updateTimeline() {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !this->tlplayer->Paused) {
			this->tlplayer->pause();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->tlplayer->Paused) { 
			this->tlplayer->unpause();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
		int currenttic = this->tlplayer->getTicSize();
		if (currenttic == this->ticsize) {
			this->tlplayer->setTicSize(this->ticsize/5);
		}
		else if (currenttic == this->ticsize/5) {
			this->tlplayer->setTicSize(2*this->ticsize);
		}
		else if (currenttic == 2*this->ticsize) {
			this->tlplayer->setTicSize(this->ticsize);
		}
	}
	return;
}

// Combined input update function for player. This is responsible for logging user inputs and reflecting them in mainClient, as well as sending them over
// the network to the server for synchronization.
void Player::updatefromInput() {
	//handle tic size changes
	this->updateTimeline();
	this->updateMovement();
	this->updatePhysics();
	//update the current time and wait for the remaining tic time to elapse for smoother updates.
	this->tlplayer->updateTime();
	std::this_thread::sleep_for(std::chrono::milliseconds(this->tlplayer->getTimeRemainingTic()));
	return;
}

void Player::updatefromNetwork(sf::Vector2f networkInput) {
	this->updateTimeline();
	this->velocity = networkInput;
	this->updatePosition();
	this->updatePhysics();
	this->tlplayer->updateTime();
	std::this_thread::sleep_for(std::chrono::milliseconds(this->tlplayer->getTimeRemainingTic()));
	return;
}

// Renders player to screen.
void Player::render(sf::RenderTarget & target) {
	target.draw(this->sprite);
}

// Parses string for replays.
sf::Vector2f Player::replayParser(std::string input) {
	sf::Vector2f output;
	std::string delimiter = ",";
	output.x = std::stof(input.substr(0, input.find(delimiter)));
	input.erase(0, input.find(delimiter) + delimiter.length());
	output.y = std::stof(input);
	return output;
}


// Returns player positions as string for replay handling
std::string Player::replayPos() {
	sf::Vector2f temp = this->getPosition();
	return (std::to_string(temp.x) + ',' + std::to_string(temp.y) + '\n');
}

// Moves player for replay handling
void Player::replayMove(std::string pos) {
	sf::Vector2f temp = this->replayParser(pos);
	this->setPosition(temp.x, temp.y);
}