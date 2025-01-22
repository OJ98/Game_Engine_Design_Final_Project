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

	int shoottimer;
	int shootlimit;
	int num_shot;

	Player* player;

	Platform* platform_base;
	Platform* alien_1;
	Platform* alien_2;
	Platform* alien_3;
	Platform* alien_4;
	Platform* alien_5;

	std::vector<Platform*> alien_pointers;
	std::vector<Platform*> shot_aliens;

	std::vector<Projectile> projectiles;
	
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
	void initShoot();

public:
	EventHandler* EH;

	//Constructors and Destructors
	Game(Timeline*, ScriptManager*, v8::Isolate*, v8::Local<v8::Context>);
	virtual ~Game();

	//Update Functions
	void updatePlayer();
	void updateNetwork();
	void updatePlatforms();
	void resolveAlienCollision(Platform*, int);
	void resolvePlatformCollision(sf::FloatRect, sf::FloatRect);
	void updateCollision();
	void updateView();
	void updateProjectile();
	void update();

	//Handler functions
	void handleSpawn();
	void handleDeath();
	void handleReset();
	void handleShoot();
	void handleCollision();
	void handleReplayStart();
	void handleReplayEnd();
	void handleReplayNetwork();
	void handleRespawn_Script();

	//Render Functions
	void renderPlayer();
	void renderProjectiles();
	void renderNetwork();
	void renderPlatforms();
	void render();
	const sf::RenderWindow& getWindow() const;

	//Helper Functions
	sf::Vector2f parseRequest(std::string);
	void victory();
	void failure();
};