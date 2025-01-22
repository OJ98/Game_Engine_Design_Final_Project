#pragma once

class Game {
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::View view;
	sf::Vector2f viewCenter;

	bool resize_behaviour_showmore;

	bool victory_cond;
	sf::Text display_text;
	sf::Font display_font;

	Player* player;

	Platform* platform_base;
	Platform* platform_wall_vert1;
	Platform* platform_wall_vert2;
	Platform* platform_wall_vert3;
	Platform* platform_wall_vert4;

	std::vector<Platform*> wall_platform_pointers;
	
	std::vector<Player*> network_player_pointers;

	HiddenObjects* spawnZone;
	HiddenObjects* deathZone;

	Timeline* main_tl;

	ScriptManager* sm;

	v8::Isolate* is;

	v8::Local<v8::Context> context;

	ReplayHandler* RH;

	int client_count;

	void initWindow();
	void initPlayer();
	void initPlatforms();
	void initEvents();
	void initHiddenObjects();
	void initReplays();
	void initText();

public:
	EventHandler* EH;

	//Constructors and Destructors
	Game(Timeline*, ScriptManager*, v8::Isolate*, v8::Local<v8::Context>);
	virtual ~Game();

	//Update Functions
	void updatePlayer();
	void updateNetwork();
	void updatePlatforms();
	void resolvePlatformCollision(sf::FloatRect, sf::FloatRect);
	void updateCollision();
	void updateView();
	void update();

	//Handler functions
	void handleSpawn();
	void handleDeath();
	void handleCollision();
	void handleReplayStart();
	void handleReplayEnd();
	void handleReplayNetwork();
	void handleRespawn_Script();

	//Render Functions
	void renderPlayer();
	void renderNetwork();
	void renderPlatforms();
	void render();
	const sf::RenderWindow& getWindow() const;

	//Helper Functions
	sf::Vector2f parseRequest(std::string);
	void victory();
	void failure();
};