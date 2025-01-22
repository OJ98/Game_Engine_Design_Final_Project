#pragma once
using json = nlohmann::json;

// Event object is tuple of type <time, event_description_json>
typedef std::tuple<int64_t, json> ev;
// Event queue is simply a min heap priority queue of event objects ordered on time.
typedef std::priority_queue<ev, std::vector<ev>, std::greater<ev>> ev_queue;
// Event registration table is a list of tuples of type <event_name, handlerfunction(Arguments)>
typedef std::vector<std::tuple<std::string, std::function<void()>>> reg_table;


class EventHandler : public GameObject{
private:
    ev_queue EventQueue;
    reg_table RegistrationTable;
    Timeline* main_tl;
public:
    json CurrentEvent;
    
    //Constructor and Destructors
    EventHandler(ev_queue, Timeline*);
    virtual ~EventHandler();

    bool isEmpty();
    void raiseEvent(ev);
    void raiseScriptEvent(json);
    void raiseNetworkEvent(json);
    void registerEvent(std::string, std::function<void()>);
    void deregisterEvent(std::string);
    void handleEvent();

    void printQ(ev_queue);
};