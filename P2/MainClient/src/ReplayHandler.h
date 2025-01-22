#pragma once
using json = nlohmann::json;

class ReplayHandler {
private:
    EventHandler* EventHandlerRef;
    Player* Playerref;
    std::ofstream replayRecord;
    std::ifstream replayPlay;
    sf::Vector2f returnPos;
    std::vector<std::string> replayExecutionOrder;
public:
    //Constructor and Destructors
    ReplayHandler(EventHandler*, Player*);
    virtual ~ReplayHandler();

    void startRecordingReplay();
    void endRecordingReplay();
    void loadReplay();
    void playReplay();
    void cacheReplay();
};