#pragma once

typedef std::vector< std::tuple<int, sf::Vector2f> > clientData;

class NetworkingClient {
private:
    clientData cd;
    std::string datasend;
    std::string clientID;
    zmq::socket_t* sref;
public:
    //Constructors and Destructors
	NetworkingClient();
	virtual ~NetworkingClient();
    
    //Accessors
    zmq::socket_t* getSocketRef();
    void setClientID(std::string);
    std::string getClientID();

    //Utility Functions
    sf::Vector2f recieveData(zmq::socket_ref);
    void sendData(zmq::socket_t*, sf::Vector2f);
    void sendData2(sf::Vector2f);
    sf::Vector2f parseReply(std::string);
};