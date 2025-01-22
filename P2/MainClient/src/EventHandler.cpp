#include"GameHeaders.h"

EventHandler::EventHandler(ev_queue EQ, Timeline* tl) {
    this->EventQueue = EQ;
    this->main_tl = tl;
    std::cout<<this->guid<<std::endl;
}

EventHandler::~EventHandler() {

}

void EventHandler::raiseEvent(ev E) {
    this->EventQueue.push(E);
}

bool EventHandler::isEmpty() {
    return this->EventQueue.empty();
}

void EventHandler::raiseNetworkEvent(json N) {
    json Request = N;
    json Reply;
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};
	// construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
	socket.connect("tcp://localhost:5555");
    Request["clientID"] = "Event";
    socket.send(zmq::buffer(Request.dump()), zmq::send_flags::none);
    // wait for reply
    zmq::message_t reply_dump{};
    zmq::recv_result_t status = socket.recv(reply_dump, zmq::recv_flags::none);
    Reply = json::parse(reply_dump.to_string());
    std::cout<<Reply["Event_Ack"]<<std::endl;
    if(status.has_value() != true) {
        std::cout<<"Response not received."<<std::endl;
    }
    return;
}

void EventHandler::raiseScriptEvent(json J) {
    int64_t time = this->main_tl->getTime_Absolute();
    ev E = make_tuple(time, J);
    this->EventQueue.push(E);
}


void EventHandler::registerEvent(std::string Event_Name, std::function<void()> Handler_Function){
    auto temp = make_tuple(Event_Name, Handler_Function);
    this->RegistrationTable.push_back(temp);
}

// Deregisters an event and removes it from the table.
void EventHandler::deregisterEvent(std::string Event_Name) {
    this->RegistrationTable.erase(std::remove_if(this->RegistrationTable.begin(), this->RegistrationTable.end(),
    [Event_Name](auto s) { return (std::get<0>(s) == Event_Name); }),
    this->RegistrationTable.end());
    //this->printQ(this->EventQueue);
}

void EventHandler::handleEvent() {
    ev temp = this->EventQueue.top();
    this->CurrentEvent = std::get<1>(temp);
    for(reg_table::iterator i = this->RegistrationTable.begin(); i != this->RegistrationTable.end(); ++i) {
        if(this->CurrentEvent["Event_Type"] == std::get<0>(*i)) {
            std::get<1>(*i)();
        }
    }
    this->EventQueue.pop();
}

void EventHandler::printQ(ev_queue priorityQueue)  { 
  while (!priorityQueue.empty())  { 
    // Each element of the priority 
    // queue is a tuple itself 
    ev Tuple =  priorityQueue.top(); 
  
    std::cout << "[ "; 
    std::cout << std::to_string(std::get<0>(Tuple)) << ' ' <<  (std::get<1>(Tuple)).dump() << ' ';
    std::cout << ']'; 
    std::cout << '\n'; 
  
    // Pop out the topmost tuple 
    priorityQueue.pop(); 
  } 
} 