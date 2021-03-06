#include "Server.h"


#define PLAYER_COUNT 4 

int portNum = 8001;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
boost::asio::io_service service;
socket_ptr usersSockPtrs[PLAYER_COUNT];
int playersId [PLAYER_COUNT];

std::queue<std::string> sendQueues[PLAYER_COUNT];

Game game;


void client_session(socket_ptr sock, int threadNum)
 {
 	int clientId = playersId[threadNum];
 	std::cout << "thread: " << clientId << endl;
	Event startEvent = START_GAME;
	std::stringstream line;
   	line << startEvent << " " << game.GetMap();
	for (int i = 0; i < PLAYER_COUNT; i++) {
 		std::string nickName = game.GetPlayerNameById(playersId[i]);
 		line << " " << playersId[i] << " " << nickName << " " << game.GetPlayerPositionById(playersId[i]);
 	}
 	line << " |";
	std::string msg = line.str(); 	
	try {
		sock->send(boost::asio::buffer(msg)); 
	}
	catch(boost::system::system_error e) {
		std::cout << e.code() << std::endl;
		return;
	}
 	msg.resize(0);

    while (true)
    {
    	if(sock->available()) {
    		char buff[512];
			int bytes = 0;
			try {
				bytes = sock->receive(boost::asio::buffer(buff));
			}
			catch(boost::system::system_error e) {
				std::cout << e.code() << std::endl;
				continue;
			}
			if (bytes > 0){
				std::string packetStr(buff, bytes);
			std::cerr << packetStr << std::endl;
			    std::stringstream stream(packetStr);
			    int typeOfPacket = 0;
				stream >> typeOfPacket;
				if(typeOfPacket == 99) {
					std::stringstream line;
					line << "|||" << game.GetPlayerPositionById(clientId) << "|||";
					std::string msg = line.str();
					sock->send(boost::asio::buffer(msg)); 
				}
				else {
					ClientAction action(clientId, (Event)typeOfPacket);
					game.PushClientAction(action);
				}
			}
    	}

    	if (!sendQueues[threadNum].empty()) {
    		while(!sendQueues[threadNum].empty()) {
    			std::string msg = sendQueues[threadNum].front();
    			try {
					sock->send(boost::asio::buffer(msg)); 
					}
				catch(boost::system::system_error e) {
					std::cout << e.code() << std::endl;
					continue;
				}
				sendQueues[threadNum].pop();
    		}
    	}
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    }
}





void server_loop() 
{
	int playersCount = 0;
	boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),portNum));
	std::cout << "Server run on port: " << portNum << std::endl;
	while ( playersCount < PLAYER_COUNT) 
	{	
		char buff[512];
    	socket_ptr sock(new boost::asio::ip::tcp::socket(service));
	    acceptor.accept(*sock);
	    usersSockPtrs[playersCount] = sock;
	    int bytes = 0;
	    try {
		    	bytes = sock->receive(boost::asio::buffer(buff));
			}
		catch(boost::system::system_error e) {
			std::cout << e.code() << std::endl;
			continue;
		}

	    std::string nickName(buff, bytes);
	    if (nickName.size() < 1) {
	    	nickName = std::string("User");
	    	nickName += playersCount + '1';
	    }
	   	int playerId = game.CreatePlayer(nickName);
	   	playersId[playersCount] = playerId;

	   	Event event = SEND_ID;
		std::stringstream line;
	   	line << event << " " << playerId;
		std::string idPack = line.str();	    
	    try {
			sock->send(boost::asio::buffer(idPack)); 
			}
		catch(boost::system::system_error e) {
			std::cout << e.code() << std::endl;
			continue;
		}
		std::cout << "player: " << playerId << " conected" <<std::endl;   
	    playersCount++;
	}

	for (int i = 0; i < PLAYER_COUNT; i++){
		boost::thread(boost::bind(client_session, usersSockPtrs[i], i));
	}

	game.Step();

	// while (true) 
 //    	boost::this_thread::sleep_for(boost::chrono::microseconds(250));

}



void SendMovePlayer(int idPlayer, Event moveType) {
	Event event = MOVE_PLAYER;
	std::stringstream line;
	line << event << " " << idPlayer << " " << moveType << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
	
}

void SendBombPlanted (int id, int coord) {
	Event event = BOMB_PLANTED;
	std::stringstream line;
	line << event << " " << id << " " << coord << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendBombExplode (int id, int radius=1) {
	Event event = BOMB_EXPLODE;
	std::stringstream line;
	line << event << " " << id << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendPlayerDead (int idPlayer) {
	Event event = PLAYER_DEAD;
	std::stringstream line;
	line << event << " " << idPlayer << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendPlusHP(int idPlayer) {
	Event event = PLUS_HP;
	std::stringstream line;
	line << event << " " << idPlayer << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendMinusHP(int idPlayer) {
	Event event = MINUS_HP;
	std::stringstream line;
	line << event << " " << idPlayer << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendBoxExplode(int id, int newType=1) {
	Event event = BOX_EXPLODE;
	std::stringstream line;
	line << event << " " << id << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}

void SendEndGame(int idPlayer) {
	Event event = END_GAME;
	std::stringstream line;
	line << event << " " << idPlayer << " |";
	std::string msg = line.str();

	for (int i = 0; i < PLAYER_COUNT; i++) {
		sendQueues[i].push(msg);
	}
}



int main(int argc, char* argv[]) 
{	
	if (argc > 1) {
		int port = atoi(argv[1]);
		if(port > 2000){
			portNum = port;
		}
	}
	server_loop();

	return 0;
}
