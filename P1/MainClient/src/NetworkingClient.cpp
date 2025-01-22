#include"GameHeaders.h"
	
NetworkingClient::NetworkingClient() {
    using namespace std::chrono_literals;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};
    
    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");
    
    // initialize the socket reference
    this->sref = &socket;

    // initialize a default send and receive strings
    this->datasend = "";
    this->clientID = "";

}

NetworkingClient::~NetworkingClient() {
}
     
// used by all clients to recieve movement data from server.
// sf::Vector2f NetworkingClient::recieveData(zmq::socket_ref sr) {
//     sf::Vector2f temp;
//     sr.send(zmq::buffer(this->clientID), zmq::send_flags::none);  
//     // wait for reply from server
//     zmq::message_t reply;
//     sr.recv(reply, zmq::recv_flags::none);
//     temp = NetworkingClient::parseReply(reply.to_string());
//     return temp;
// }

// // used by the mainClient to send movement data to the server.
// void NetworkingClient::sendData(zmq::socket_t* sr, sf::Vector2f input) {
//     sf::Vector2f temp;
//     // construct the response string
//     this->datasend = "mainClient:" + std::to_string(input.x) + "," + std::to_string(input.y);
//     std::cout << "sent message: " << this->datasend<<std::endl;
//     // send the reply to the server
//     (*sr).send(zmq::buffer(this->datasend), zmq::send_flags::none);
//     std::cout << "sent message: " << this->datasend;
//     // wait for reply from server
//     zmq::message_t reply{};
//     (*sr).recv(reply, zmq::recv_flags::none);
//     temp = NetworkingClient::parseReply(reply.to_string());
//     return;
// }


// parses reply recieved from server of the form "xvalue,yvalue"
sf::Vector2f NetworkingClient::parseReply(std::string s) {
    sf::Vector2f output;
    std::string delimiter = ",";
    output.x = std::stof(s.substr(0, s.find(delimiter)));
    s.erase(0, s.find(delimiter) + delimiter.length());
    output.y = std::stof(s.substr(0, s.find(delimiter)));
    return output;
}

// returns the pointer to the socket
zmq::socket_t* NetworkingClient::getSocketRef() {
    return this->sref;
}

// used to set client ID to a unique value
void NetworkingClient::setClientID(std::string ID) {
    this->clientID = ID;
    return;
}

// used to get the current value of client ID
std::string NetworkingClient::getClientID() {
    return this->clientID;
}