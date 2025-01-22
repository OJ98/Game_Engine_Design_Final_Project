#include"GameHeaders.h"

// Constructor
Game::Game(Timeline* t, ScriptManager* sm, v8::Isolate *isolate, v8::Local<v8::Context> context) {
	this->main_tl = t;
	this->sm = sm;
	this->is = isolate;
	this->context = context;
	this->victory_cond = false;
	this->initWindow();
	this->initPlatforms();
	this->initHiddenObjects();
	this->initEvents();
	this->initPlayer();
	this->initText();
	this->initReplays();

	// Without parameters, these calls are made to the default context
    this->sm->addScript("hello_world", "../src/scripts/hello_world.js");
	this->sm->addScript("raise_event","../src/scripts/raise_event.js");
}

// Destructor
Game::~Game() {
	delete this->player;
    
	delete this->platform_base;
	delete this->platform_moving1;
	delete this->platform_moving2;

	delete this->spawnZone;
	delete this->deathZone;

	delete this->EH;

	delete this->sm;

	for (auto j = this->moving_platform_pointers.begin(); j != this->moving_platform_pointers.end(); ++j) {
		delete (*j);
	}
	for (auto j = this->network_player_pointers.begin(); j != this->network_player_pointers.end(); ++j) {
		delete (*j);
	}

	delete this->RH;
}

// Initializes window.
void Game::initWindow() {
	this->window.create(sf::VideoMode(800, 600), "SFML_Platformer", sf::Style::Resize | sf::Style::Titlebar);
	this->window.setFramerateLimit(60);
	this->resize_behaviour_showmore = false;
	this->view = window.getDefaultView();
	this->viewCenter = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

// Initializes Player object.
void Game::initPlayer() {
	this->player = new Player(this->EH, this->main_tl);
	sf::Vector2f pos = this->spawnZone->getPosition();
	this->player->setPosition(pos.x, pos.y);
	this->client_count = 0;
	for (int i = 0; i < 4; i++) {
		auto tempplayer = new Player(this->EH, this->main_tl);
		tempplayer->setPosition(pos.x, pos.y);
		this->network_player_pointers.push_back(tempplayer);
	}
}

// Initializes Platforms.
void Game::initPlatforms() {
	this->platform_base = new Platform();

	this->platform_base->setVelocity(0.f);
	this->platform_base->setSize(sf::Vector2f(90000,100));
	this->platform_base->setFillColor(sf::Color::Blue);
	this->platform_base->setPosition(1, 500);
	this->platform_base->setMoveDistance(400.f);

	this->platform_moving1 = new Platform();
	this->platform_moving1->setVelocity(1.f);
	this->platform_moving1->setSize(sf::Vector2f(100,10));
	this->platform_moving1->setFillColor(sf::Color::Red);
	this->platform_moving1->setPosition(50, 300);
	this->platform_moving1->setMoveDistance(400.f);
	this->platform_moving1->setMovementPattern(0);
	this->moving_platform_pointers.push_back(this->platform_moving1);

	this->platform_moving2 = new Platform();
	this->platform_moving2->setVelocity(1.f);
	this->platform_moving2->setSize(sf::Vector2f(100,10));
	this->platform_moving2->setFillColor(sf::Color::Green);
	this->platform_moving2->setPosition(600, 200);
	this->platform_moving2->setMoveDistance(100.f);
	this->platform_moving2->setMovementPattern(1);
	this->moving_platform_pointers.push_back(this->platform_moving2);

}

// Initialize Hidden Objects (Spawn and Death Zones)
void Game::initHiddenObjects() {
	this->spawnZone = new HiddenObjects();
	this->spawnZone->setPosition(10, 400);

	this->deathZone = new HiddenObjects();
	this->deathZone->setPosition(500, 500);
	//Set this hidden object as the death zone
	this->deathZone->setHOType(1);
	this->deathZone->setclosestSpawn(this->spawnZone);
}

// Initialize Event Management System.
void Game::initEvents() {
	ev_queue EQ;
	this->EH = new EventHandler(EQ, this->main_tl);
	this->EH->registerEvent("Death", std::bind(&Game::handleDeath, this));
	this->EH->registerEvent("Spawn", std::bind(&Game::handleSpawn, this));
	this->EH->registerEvent("Collision", std::bind(&Game::handleCollision, this));
	this->EH->registerEvent("Respawn_Script", std::bind(&Game::handleRespawn_Script, this));
	this->EH->exposeToV8(this->is, this->context);
}

// Initialize the Replay Handling System.
void Game::initReplays() {
	this->RH = new ReplayHandler(this->EH, this->player);
	this->EH->registerEvent("Replay_Start", std::bind(&Game::handleReplayStart, this));
	this->EH->registerEvent("Replay_End", std::bind(&Game::handleReplayEnd, this));
	this->EH->registerEvent("Replay_Start", std::bind(&Game::handleReplayNetwork, this));
}

// Initializes the Display text system
void Game::initText() {
	if (!this->display_font.loadFromFile("../src/Resources/times.ttf")) {
    	std::cout<<"Error loading fonts"<<std::endl;
	}
	// select the font
	this->display_text.setFont(this->display_font); // font is a sf::Font
	// set the string to display
	this->display_text.setString("");
	// set the character size
	this->display_text.setCharacterSize(100); // in pixels, not points!
	// set the color
	this->display_text.setFillColor(sf::Color::Red);
	// set the text style
	this->display_text.setStyle(sf::Text::Bold);
	//center text
	sf::FloatRect textRect = this->display_text.getLocalBounds();
	this->display_text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
	sf::Vector2u size = window.getSize();
	this->display_text.setPosition(sf::Vector2f(size.x/4.0f,size.y/4.0f));
}

// Update View based on player movement
void Game::updateView() {
	float window_width = this->window.getSize().x / 2.f;
	if (this->player->getPosition().x > this->viewCenter.x + window_width)
    {
        this->viewCenter.x  = this->viewCenter.x + window_width;
    }
    if  (this->player->getPosition().x < this->viewCenter.x - window_width)
    {
        this->viewCenter.x = this->viewCenter.x - window_width;
    }
	if (this->viewCenter.x == 0) {
		this->viewCenter.x = window_width;
	}
    this->view.setCenter(this->viewCenter);
	this->window.setView(this->view);
}

// Calls Player Input Update function.
void Game::updatePlayer() {
	this->updateCollision();
	this->player->updatefromInput();
}

// Calls Player Network Update function for all network controlled players.
void Game::updateNetwork() {
	json Request;
	json Reply;
	
	// initialize the zmq context with a single IO thread
    zmq::context_t context{1};
	// construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
	socket.connect("tcp://localhost:5555");

	// Register Clients
	if(this->player->getNetworkID() == "RegisterClient") {
		Request["clientID"] = "RegisterClient";
		Request["positionX"] = std::to_string(this->player->getPosition().x);
		Request["positionY"] = std::to_string(this->player->getPosition().y);
		// send the reply to the server
    	socket.send(zmq::buffer(Request.dump()), zmq::send_flags::none);
		// wait for available clientID in reply
		zmq::message_t reply_dump{};
    	zmq::recv_result_t status = socket.recv(reply_dump, zmq::recv_flags::none);
		if(status.has_value() != true) {
        	std::cout<<"Response not received."<<std::endl;
    	}
		Reply = json::parse(reply_dump.to_string());
		for (auto it = Reply.begin(); it != Reply.end(); ++it) {
			if(it.key() == "newID") {
				this->player->setNetworkID(it.value());
				continue;
			}
			else {
				auto tempPos = this->parseRequest(it.value());
				bool flag = true;
				for (auto j = this->network_player_pointers.begin(); j != this->network_player_pointers.end(); ++j ) {
					if((*j)->getNetworkID() == "RegisterClient" && (*j)->getNetworkID() != this->player->getNetworkID() && flag) {
						(*j)->setNetworkID(it.key());
						(*j)->setPosition(tempPos.x, tempPos.y);
						flag = false;
					}
				}
			}
		}
	}
	else {
		Request["clientID"] = this->player->getNetworkID();
		Request["positionX"] = std::to_string(this->player->getPosition().x);
		Request["positionY"] = std::to_string(this->player->getPosition().y);
		// send the reply to the server
    	socket.send(zmq::buffer(Request.dump()), zmq::send_flags::none);
		// wait for available clientID in reply
		zmq::message_t reply_dump{};
    	zmq::recv_result_t status = socket.recv(reply_dump, zmq::recv_flags::none);
		if(status.has_value() != true) {
        	std::cout<<"Response not received."<<std::endl;
    	}
		Reply = json::parse(reply_dump.to_string());
		this->client_count = Reply["client_count"].get<int>()-1;
	}
	
}

// Calls spawn handler function
void Game::handleSpawn() {
	std::cout<<"Spawn Event Detected."<<std::endl;
}

// Calls death handler function.
void Game::handleDeath() {
	std::cout<<"Death Event Detected."<<std::endl;
	this->deathZone->performAction(this->player);
	this->sm->runOne("raise_event", false);
}

// Calls collision handler function.
void Game::handleCollision() {
	//std::cout<<"Collision Event Detected."<<std::endl;
}

// Calls replay start handler function.
void Game::handleReplayStart() {
	std::cout<<"Replay Start Event Detected."<<std::endl;
	this->RH->startRecordingReplay();
}

// Calls replay end handler function.
void Game::handleReplayEnd() {
	std::cout<<"Replay Play Event Detected."<<std::endl;
	this->RH->endRecordingReplay();
}

// Calls replay network handler function.
void Game::handleReplayNetwork() {
	json n;
	n["Event_Type"] = "ReplayEvent";
	n["Event_Client"] = this->player->getNetworkID();
	n["Event_Time"] = std::to_string(this->main_tl->getTime_Absolute());
	std::cout<<"Replay Event Sent to Server."<<std::endl;
	this->EH->raiseNetworkEvent(n);
}

// Calls Respawn Handler function
void Game::handleRespawn_Script() {
	std::cout<<"Respawn Event handled by script"<<std::endl;
}

// Calls Platform Update function.
void Game::updatePlatforms() {
    this->updateCollision();
	this->platform_base->update();
	for (auto it = begin (this->moving_platform_pointers); it != end (this->moving_platform_pointers); ++it) {
		(*it)->update();
	}
}

// Handles Collision given bounds.
void Game::resolvePlatformCollision(sf::FloatRect playerGlobalBounds, sf::FloatRect platformGlobalBounds) {
	
	//player_bottom -> platform_top collision
	if(playerGlobalBounds.top > platformGlobalBounds.top 
	&&playerGlobalBounds.top + playerGlobalBounds.height  > platformGlobalBounds.top + platformGlobalBounds.height
	&&playerGlobalBounds.left < platformGlobalBounds.left + platformGlobalBounds.width  
	&&playerGlobalBounds.left + playerGlobalBounds.width > playerGlobalBounds.left) {
		this->player->resetVelocityY();
		this->player->setPosition(playerGlobalBounds.left, platformGlobalBounds.top + platformGlobalBounds.height);
		//std::cout<<"player_top -> platform_bottom Collision\n";
	}
	//player_top -> platform_bottom collision
	else if(playerGlobalBounds.top < platformGlobalBounds.top 
	&&playerGlobalBounds.top + playerGlobalBounds.height  < platformGlobalBounds.top + platformGlobalBounds.height
	&&playerGlobalBounds.left < platformGlobalBounds.left + platformGlobalBounds.width  
	&&playerGlobalBounds.left + playerGlobalBounds.width > playerGlobalBounds.left) {
		this->player->resetVelocityY(); 
		this->player->setPosition(playerGlobalBounds.left, platformGlobalBounds.top - playerGlobalBounds.height);
		//std::cout<<"player_bottom -> platform_top Collision\n";
	}
	//right collision
	else if(playerGlobalBounds.left < platformGlobalBounds.left 
	&&playerGlobalBounds.left + playerGlobalBounds.width  < platformGlobalBounds.left + platformGlobalBounds.width
	&&playerGlobalBounds.top < platformGlobalBounds.top + platformGlobalBounds.height  
	&&playerGlobalBounds.top + playerGlobalBounds.height > playerGlobalBounds.top) {
		this->player->resetVelocityX();
		this->player->setPosition(platformGlobalBounds.left - playerGlobalBounds.width, playerGlobalBounds.top);
		//std::cout<<"Right Collision\n";
	}
	//left collision
	else if(playerGlobalBounds.left > platformGlobalBounds.left 
	&&playerGlobalBounds.left + playerGlobalBounds.width  > platformGlobalBounds.left + platformGlobalBounds.width
	&&playerGlobalBounds.top < platformGlobalBounds.top + platformGlobalBounds.height  
	&&playerGlobalBounds.top + playerGlobalBounds.height > playerGlobalBounds.top) {
		this->player->resetVelocityX();
		this->player->setPosition(platformGlobalBounds.left + platformGlobalBounds.width, playerGlobalBounds.top);
		//std::cout<<"Left Collision\n";
	}
}

// Handles screen collision, scrolling and calls function to handle platform collisions
void Game::updateCollision() {
	json c;
	c["Event_Type"] = "Collision";
	//Collision left of screen
	if(this->player->getPosition().x < 0.f) {
		this->player->setPosition(0.f, this->player->getPosition().y);
		c["Collision_Type"] = "Left";
	}
	//Collision top of screen
	if(this->player->getPosition().y < 0.f) {
		this->player->setPosition(this->player->getPosition().x, 0.f);
		c["Collision_Type"] = "Top";
	}
	//Collision bottom of screen
	if (this->player->getPosition().y + this->player->getGlobalBounds().height > this->window.getSize().y) {
		this->player->resetVelocityY();
		this->player->setPosition(
			this->player->getPosition().x,
			this->window.getSize().y - this->player->getGlobalBounds().height
		);
		c["Collision_Type"] = "Bottom";
	}
		
	
	sf::FloatRect playerGlobalBounds = this->player->getGlobalBounds();

	//Collision base platform
	if (this->platform_base->getGlobalBounds().intersects(this->player->getNextPosition())) {
		resolvePlatformCollision(playerGlobalBounds, this->platform_base->getGlobalBounds());
		c["Collision_Type"] = "Platform-Stationary";
	}

	//Collision moving platforms
	for (auto it = begin (this->moving_platform_pointers); it != end (this->moving_platform_pointers); ++it) {
    	if ((*it)->getGlobalBounds().intersects(playerGlobalBounds)) {
			resolvePlatformCollision(playerGlobalBounds, (*it)->getGlobalBounds());
			c["Collision_Type"] = "Platform-Moving";
		}
	}
	ev C = make_tuple(this->main_tl->getTime_Absolute(), c);
	this->EH->raiseEvent(C);
	
	//Collision death zone
	if (this->deathZone->getGlobalBounds().intersects(this->player->getNextPosition())) {
		json d;
		d["Event_Type"] = "Death";
		ev D = make_tuple(this->main_tl->getTime_Absolute(), d);
		this->EH->raiseEvent(D);
		json s;
		s["Event_Type"] = "Spawn";
		ev S = make_tuple(this->main_tl->getTime_Absolute(), s);
		this->EH->raiseEvent(S);
	}
}

// Combined Update function checks for movement, collision and resize events.
void Game::update() {
	if(this->EH->ScriptEvent["Raised"]) {
		this->EH->ScriptEvent["Raised"] = false;
		json o;
		o["Event_Type"] = this->EH->ScriptEvent["Event_Type"];
		ev O = make_tuple(this->main_tl->getTime_Absolute(), o);
		this->EH->raiseEvent(O);
	}
	//Polling window events
	while (this->window.pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed) {
			this->window.close();
		}
		else if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::Escape) {
			this->window.close();
		}
		else if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::LShift)
			this->resize_behaviour_showmore = !this->resize_behaviour_showmore;
		else if (event.type == sf::Event::Resized && this->resize_behaviour_showmore == true) {
        	// update the view to the new size of the window
        	sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        	window.setView(sf::View(visibleArea));
    	}
		else if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::R) {
			// raise a replay-start recording event.
			json r;
			r["Event_Type"] = "Replay_Start";
			ev R = make_tuple(this->main_tl->getTime_Absolute(), r);
			this->EH->raiseEvent(R);
		}
		else if (this->event.type == sf::Event::KeyPressed && this->event.key.code == sf::Keyboard::P) {
			// raise a replay-end recording event.
			json p;
			p["Event_Type"] = "Replay_End";
			ev P = make_tuple(this->main_tl->getTime_Absolute(), p);
			this->EH->raiseEvent(P);
		}
	}

	//Move platforms
	this->updatePlatforms();
	
	if(this->window.hasFocus()) {
		//Update player movement and update from network.
		std::mutex m;
		std::condition_variable cv;
		ThreadMaker t1(0, std::bind(&Game::updatePlayer, this), NULL, &m, &cv);
		ThreadMaker t2(1, std::bind(&Game::updateNetwork, this), &t1, &m, &cv);
		std::thread network_thread(&ThreadMaker::runWrapper, &t1);
		std::thread player_thread(&ThreadMaker::runWrapper, &t2);
		network_thread.join();
		player_thread.join();
	}
	
	else {
		//Update from network.
		this->updateNetwork();
	}
	
	json e;
	e["Event_Type"] = "Loop_End";
	ev E = make_tuple(this->main_tl->getTime_Absolute(), e);
	this->EH->raiseEvent(E);

	// Handle all events.
	while(!this->EH->isEmpty()) {
		this->EH->handleEvent();
	}

	// Victory Condition.
	if(this->victory_cond) {
		this->victory();
	}

	//Update view
	this->updateView();
}

// Render player
void Game::renderPlayer() {
	this->player->render(this->window);
}

// Render network entities
void Game::renderNetwork() {
	for (int j = 0; j < this->client_count; j++) {
		this->network_player_pointers[j]->render(this->window);
	}
}

// Render all platforms
void Game::renderPlatforms() {
	this->platform_base->render(this->window);
	for (auto it = begin (this->moving_platform_pointers); it != end (this->moving_platform_pointers); ++it) {
		(*it)->render(this->window);
	}
}

// Combined Render function to render all objects.
void Game::render() {
	this->window.clear();
	//Render player
	this->renderPlayer();
	//Render network entities
	this->renderNetwork();
    //Render platforms
    this->renderPlatforms();
	//Display text
	this->window.draw(this->display_text);
	this->window.display();
}

// Return reference to game window in order to run main game loop
const sf::RenderWindow & Game::getWindow() const {
	return this->window;
}

// Parse the response from Server
sf::Vector2f Game::parseRequest(std::string input) {
	sf::Vector2f output;
	std::string delimiter = ",";
	output.x = std::stof(input.substr(0, input.find(delimiter)));
	input.erase(0, input.find(delimiter) + delimiter.length());
	output.y = std::stof(input);
	return output;
}

// Victory Condition
void Game::victory() {
	this->display_text.setString("VICTORY");
	this->display_text.setFillColor(sf::Color::Green);
}

// Victory Condition
void Game::failure() {
	this->display_text.setString("FAILURE");
	this->display_text.setFillColor(sf::Color::Red);
}