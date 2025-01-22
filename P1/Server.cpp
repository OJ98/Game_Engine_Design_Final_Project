#include"MainClient/src/GameHeaders.h"

using json = nlohmann::json;

typedef std::deque<sf::Vector2f> clientMovement;
typedef std::vector<std::tuple<std::string, clientMovement>> allMovements;
typedef std::vector<std::tuple<std::string, allMovements>> unprocessedMovements;

/* 
    allMovements is a list of tuples of the form <clientID, movestack>.
    An instance of allMovements is used to keep track of all moves made by all clients the server is aware of.

    unprocessedMovements is a list of tuples of the form <clientID, allMovements>.
    unprocessedMovements contains a list of moves the clients have not processed.
    Each client represented by a clientID has a list of tuples of the form <clientID, movestack> indicating moves FROM THAT CLIENT
    that the client has not processed.
    This obviously has redundancies in terms of the client also keeping track of its own moves in the unprocessedMovements list.
    A client will never read its own moves from the unprocessedMovements list (there is a check for this while constructing response string).
*/

void printAllMovements(allMovements am) {
    for(allMovements::iterator i = am.begin(); i != am.end(); i++) {
        std::cout<<std::get<0>(*i) + '\n';
        for(auto iter = std::get<1>(*i).begin(); iter != std::get<1>(*i).end(); ++iter) {
            std::cout<<std::to_string((*iter).x)+','+std::to_string((*iter).y)+ '\n';
        }
    }
}


/* Take the clientID as an input and construct a JSON of moves of all clients that the client has not processed.
Return the result. */

json constructResult(unprocessedMovements* upm, std::string inputID) {
    json Result;
    allMovements temp;
    // Extract the relevant allMovements list.
    for(unprocessedMovements::iterator i = (*upm).begin(); i != (*upm).end(); ++i) {
        if(std::get<0>(*i) == inputID) {
            temp = std::get<1>(*i);
        }
    }
    // Construct the results JSON.
    for(allMovements::iterator i = temp.begin(); i != temp.end(); ++i) {
        std::string tempID = std::get<0>(*i);
        sf::Vector2f tempPos = std::get<1>(*i).back();
        if(tempPos != sf::Vector2f(-100000000.f, -100000000.f)) {
            //std::get<0>(*i).erase(std::get<0>(*i).begin());
            std::get<1>(*i).pop_back();
        }
        std::string tempoutput;
        tempoutput = std::to_string(tempPos.x) + ',' + std::to_string(tempPos.y);
        Result[tempID] = tempoutput;
    }
    // Update the unprocessedMovement list.  
    for(unprocessedMovements::iterator i = (*upm).begin(); i != (*upm).end(); ++i) {
        if(std::get<0>(*i) == inputID) {
            std::get<1>(*i) = temp;
        }
    }
    return Result;
}

/* Take input as an unprocessed string from client.
Deserialize it using the JSON library.
Update allMovements.
Update unprocessedMovements.
Construct output JSON.
Return serialized output. */

std::string processClient(unprocessedMovements *upm, allMovements* am, std::string input, int* count_client) {
    // Initialize output position, intermediate position and return values.
    sf::Vector2f outputPos;
    std::string posx;
    std::string posy;
    json result;
    std::string output;
    
    // Parse input.
    json inputVector = json::parse(input);

    // If the client is new, it will send RegisterClient as the clientID.
    if(inputVector["clientID"] == "RegisterClient") {
        (*count_client) = (*count_client) + 1;
        std::string tempID = "Client0";
        int number = 0;
        
        // Iterate over the allMovements vector and find a unique number to return to the client.
        for(allMovements::iterator i = (*am).begin(); i != (*am).end(); ++i) {
            if(std::get<0>(*i) == tempID) {
                number = number + 1; 
            }
            tempID = "Client" + std::to_string(number);
        }
        
        // Initialize the clientMovement vector to note the movement of the newly detected client.
        clientMovement cm;
        outputPos = sf::Vector2f(-100000000.f, -100000000.f);
        cm.push_back(outputPos);
        posx = inputVector["positionX"];
        posy = inputVector["positionY"];
        outputPos.x = std::stof(posx);
        outputPos.x = std::stof(posy);
        cm.push_back(outputPos);

        // Update the allMovement vector.
        (*am).push_back(std::tuple<std::string, clientMovement>(tempID, cm));

        // Update the unprocessedMovement vector.
        // Create a temporary allMovements vector that can be filled with the last known values of pos for all known clients.
        allMovements tempAM;
        std::string tempCID;
        // Iterate over all client IDs and construct the stack for their latest move to update unprocessedMovements with new client.
        for(allMovements::iterator i = (*am).begin(); i != (*am).end(); ++i) {
            tempCID = std::get<0>(*i);
            clientMovement tempCM;
            // Push the EOL value
            tempCM.push_back(sf::Vector2f(-100000000.f, -100000000.f));
            // Push the last known position value of client.
            tempCM.push_back(std::get<1>(*i).back());
            tempAM.push_back(std::tuple<std::string, clientMovement>(tempCID, tempCM));
        }
        // Update the unprocessedMovement structure with the new client's unprocessed moves.
        (*upm).push_back(std::tuple<std::string, allMovements>(tempID, tempAM));

        // Construct result.
        result = constructResult(upm, tempID);
        result["newID"] = tempID;
    }

    // If the client has sent an event, we need to record it on server.
    else if(inputVector["clientID"] == "Event") {
        std::cout 
        << "Event Detected on Server of type: " << inputVector["Event_Type"] 
        << " at time " << inputVector["Event_Time"]
        <<" on client "<<inputVector["Event_Client"]<<std::endl;
        result["Event_Ack"] = "True";
    }

    // If the client is not new, we need to update the new movement in unprocessedMovements and construct response.
    else {
        // Read the latest position values.
        posx = inputVector["positionX"];
        posy = inputVector["positionY"];
        outputPos.x = std::stof(posx);
        outputPos.y = std::stof(posy);
        for(unprocessedMovements::iterator i = (*upm).begin(); i != (*upm).end(); ++i) {
            // This will give us the <clientID, allMovement> tuples in order.
            for(allMovements::iterator j = (std::get<1>(*i).begin()); j != (std::get<1>(*i).end()); j++) {
                // This will give us the movestacks for all clientIDs in the allMovement list.
                if(std::get<0>(*j) == inputVector["clientID"]) {
                    std::get<1>(*j).push_back(outputPos);
                }
                
            }
            //printAllMovements(std::get<1>(*i));
        }
        
        // Construct output.
        //result = constructResult(upm, inputVector["clientID"]);
        result["client_count"] = (*count_client);
    }
    // Serialize.
    output = result.dump();
    return output;
}

// Main Server Loop
int main() {
    using namespace std::chrono_literals;

    // Initialize the server data structure for unprocessed movements.
    unprocessedMovements upm;

    // Initialize the server data structure for all known movements.
    allMovements am;

    // Initialize number of clients known.
    int i = 0;
    
    // Initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // Construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    // Initialize a default response
    std::string data;

    // Start message
    std::cout<<"Server Started!"<<std::endl;

    while(true) {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        data = processClient(&upm, &am, request.to_string(), &i);
        // send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    return 0;
}