#include"GameHeaders.h"

ReplayHandler::ReplayHandler(EventHandler* EH, Player* P) {
    this->EventHandlerRef = EH;
    this->Playerref = P;
}

ReplayHandler::~ReplayHandler() {
    delete this->EventHandlerRef;
    delete this->Playerref;
}

void ReplayHandler::startRecordingReplay() {
    this->replayRecord.open("Replay_record.txt");
    this->EventHandlerRef->registerEvent("Loop_End", std::bind(&ReplayHandler::loadReplay, this));
}

void ReplayHandler::loadReplay() {
    this->replayRecord<< this->Playerref->replayPos();
}

void ReplayHandler::endRecordingReplay() {
    this->replayRecord.close();
    this->EventHandlerRef->deregisterEvent("Loop_End");
    this->EventHandlerRef->registerEvent("Loop_End", std::bind(&ReplayHandler::playReplay, this));
    std::string line;
    this->replayPlay.open("Replay_record.txt");
    while(std::getline(this->replayPlay,line) ) {
        this->replayExecutionOrder.push_back(line);
    }
    this->returnPos = this->Playerref->getPosition();
}

void ReplayHandler::playReplay() {
    if(!this->replayExecutionOrder.empty()) {
        auto temp = this->replayExecutionOrder[0];
        this->Playerref->replayMove(temp);
        this->replayExecutionOrder.erase(this->replayExecutionOrder.begin());
    }
    else {
        this->EventHandlerRef->deregisterEvent("Loop_End");
        this->EventHandlerRef->registerEvent("Loop_End", std::bind(&ReplayHandler::cacheReplay, this));
        this->replayPlay.close();
        std::remove("Replay_record.txt");
        this->Playerref->setPosition(this->returnPos.x, this->returnPos.y);
    }
}

// Dummy function to track loop_end event when no replay is in progress.
void ReplayHandler::cacheReplay() {
}