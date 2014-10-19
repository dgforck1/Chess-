//############################################################################
// TO DO:
//       -generate_string_for_clients function
//       -split the removeFromGame fucntion into two parts, the player side 
//        and spectator side.
//           -player side: removes the player and sends a win message to the
//            other player
//           -spectator side: removes the spectator
//         ALTERNATIVELY: keep it as it is and write another function that 
//                        sends the win message to the other player and call 
//                        that from removeFromGame
//       -make a way for the system to send messages to the users
//        really just need a format for the messages and specify what they do
//############################################################################


//////////////////////////////////////////////////////////////////////////////
// BEGIN Includes.
//////////////////////////////////////////////////////////////////////////////

// Standard includes
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// SDL wrapper from Dr. Liow
#include "Includes.h"
#include "Event.h"
#include "compgeom.h"
#include "Constants.h"
#include "Surface.h"

// SDL net
#include "SDL_net.h"

// Our includes
#include "ChessGame.cpp"

//////////////////////////////////////////////////////////////////////////////
// END Includes.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Some Function Prototypes
//////////////////////////////////////////////////////////////////////////////
std::string ftos(float);
std::string itos(int);
std::string recv_message(TCPsocket);
int send_message(std::string, TCPsocket);
int find_client(TCPsocket);
int find_client_name(std::string);
void handle_login(TCPsocket, std::string);
void add_client(TCPsocket, std::string);
void handle_disconnect(int);
void reconnect_client(std::string);
SDLNet_SocketSet create_sockset();
void send_all(std::string);
void send_client(int, std::string);
std::string generate_string_for_clients();// not filled/working
void parse(std::string, int);
void move(std::string &, int);
void quitGame(int);
void make(int);
void joinSpectate(int);
void joinPlay(int);
void load(std::string &, int);
void draw(std::string &, int c);
void nope(std::string &, int c);
void exitProgram(int c);



//void send_client(int, std::string);
//void send_all(std::string buf);
////void reconnect(std::string name);
//void add_client(TCPsocket sock, std::string name);



//////////////////////////////////////////////////////////////////////////////
// Class Definitions
//////////////////////////////////////////////////////////////////////////////

class Client
{
public:    
    Client(TCPsocket s = NULL, std::string n = "", bool a = false, int l = 0)
        : sock(s), name(n), active(a), location(l)
    {};
    
	TCPsocket sock;
	std::string name;
    bool active;
    int location;
};

class Game
{
public:
    // the constructor will always need a player, that will be player 1
    Game(Client * p1)
    {
        players.push_back(p1);
        started = false;
        draw = false;
    }

    bool getDraw()
    {return draw;}

    // add spectator
    void addSpectator (Client * s)
    {
        spectators.push_back(s);
    }
    
    // add player, if there are already 2 players add a spectator instead
    void addPlayer (Client * p)
    {
        if (players.size() > 1)
            addSpectator(p);
        else
            players.push_back(p);
    }

    // checks players and spectators, if it finds a client who matches the specified address
    // it removes them from the vector and returns true.
    bool removeFromGame(Client * p)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i] == p)
            {
                players.erase(players.begin() + i);
                return true;
            }
        }
        for (int i = 0; i < spectators.size(); i++)
        {
            if (spectators[i] == p)
            {
                spectators.erase(spectators.begin() + i);
                return true;
            }
        }
        return false;
    }
    
    // send a move from one player to all other in the same game
    void sendMove(std::string & move, std::string & sender)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->name == sender)
                continue;
            send_message(move, players[i]->sock);
        }
        for (int i = 0; i < spectators.size(); i++)
        {
            send_message(move, spectators[i]->sock);
        }
    }

    void sendDrawOffer(std::string & d, Client * sender)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i] == sender)
                continue;
            send_message(d, players[i]->sock);
        }
        draw = true;
    }

    void sendRefuseDraw(std::string & d, Client * sender)
    {
        this->sendDrawOffer(d, sender);
        draw = false;
    }

    // returns true if there is a player with the specified name in the game
    bool isPlayerN(std::string & n)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->name == n)
                return true;
        }
        return false;
    }

    // returns true if there is a player with the specified address in the game
    bool isPlayerA(Client * c)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i] == c)
                return true;
        }
        return false;
    }

    // returns true if there is a spectator with the specified name in the game
    bool isSpectatorN(std::string & n)
    {
        for (int i = 0; i < spectators.size(); i++)
        {
            if (spectators[i]->name == n)
                return true;
        }
        return false;
    }

    // returns true if there is a spectator with the specified address in the game
    bool isSpectatorA(Client * c)
    {
        for (int i = 0; i < spectators.size(); i++)
        {
            if (spectators[i] == c)
                return true;
        }
        return false;
    }

    void startGame()
    {
        for (int i = 0; i < players.size(); i++)
        {
            players[i]->location = 2;
        }
        for (int i = 0; i < spectators.size(); i++)
        {
            spectators[i]->location = 2;
        }
        started = true;
    }

    void endGame()
    {
        for (int i = 0; i < players.size(); i++)
        {
            int j = find_client(players[i]->sock);
            // send draw message to both players, have them dealwith it on the their end
            // to finish up and go to main menu
            // thought here, just re-send draw
            // clients will watch for a draw after they send one
            // if they recieve one then they know the game is a draw and they should
            // get over themselves
            quitGame(j);
        }
        for (int i = 0; i < spectators.size(); i++)
        {
            int j = find_client(spectators[i]->sock);
            // send draw message to all spectators, have them dealwith it on the their end
            // to finish up and go to main menu
            quitGame(j);
        }
    }
private:
    //Client player1;
    //Client player2;
    std::vector<Client*> players;
    std::vector<Client*> spectators;
    bool started;
    bool draw;
};



//////////////////////////////////////////////////////////////////////////////
// Global Constants.
//////////////////////////////////////////////////////////////////////////////
const int MAXLEN = 1024;

//////////////////////////////////////////////////////////////////////////////
// Global Variables.
//////////////////////////////////////////////////////////////////////////////
std::vector<Client> clients;
std::vector<Game> games;
std::vector<Client*> mainMenu;      // location 0
std::vector<Client*> reviewing;     // location 1
std::vector<Game*> gamesInProgress; // location 2
std::vector<Game*> gamesWaiting;    // location 3
int num_clients=0;
TCPsocket server;

//////////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////////

// Converts a float to string
std::string ftos(float f)
{
	std::ostringstream buff;
	buff << f;

	return buff.str();
}


// Converts an integer to string
std::string itos(int i)
{
	std::ostringstream buff;
	buff << i;

	return buff.str();
}


// Receive a string over TCP/IP
std::string recv_message(TCPsocket sock)
{
    char buff[MAXLEN] = {' '};
    SDLNet_TCP_Recv(sock, buff, MAXLEN);

    if (buff == NULL)
    {
        std::string ret = "";
        return ret;
    }
    
    std::string ret(buff, strlen(buff));
    return ret;
}


// Send a string over TCP/IP
int send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();      
    return SDLNet_TCP_Send(sock, buff, MAXLEN);
}


// find a client in our array of clients by it's socket.
// the socket is always unique 
int find_client(TCPsocket sock)
{
	for(int i = 0; i < num_clients; i++)
		if(clients[i].sock == sock)
			return(i);

    return -1;
}


// find a client in our array of clients by it's name.
// the name is always unique
int find_client_name(std::string name)
{
	for(int i=0; i < num_clients;i++)
		if (clients[i].name == name)
			return i;
		
	return -1;
}


// Handles logging in
void handle_login(TCPsocket sock, std::string name)
{
    if(!name.length())
	{
		send_message("Invalid Nickname...bye bye!", sock);
		SDLNet_TCP_Close(sock);
		return;
	}

    int cindex = find_client_name(name);

    if (cindex == -1)
    {
        add_client(sock, name);
        return;
    }
    
    if (clients[cindex].active)
    {
        send_message("Duplicate Nickname... bye bye!", sock);
        SDLNet_TCP_Close(sock);
        return;
    }

    clients[cindex].sock = sock;
    clients[cindex].active = true;
    return;
}


// Add a client to the list of clients
void add_client(TCPsocket sock, std::string name)
{	
	Client c;

	c.name = name;
	c.sock = sock;
	c.active = true;

	clients.push_back(c);
    mainMenu.push_back(&c);

	num_clients++;

	std::cout << "inside add client" << std::endl;
	std::cout << "num clients: " << num_clients << std::endl;

	// Send an acknowledgement
    std::string player_number = "N";
	player_number += itos(num_clients - 1);
	player_number += ";#";
	// send client their player number
	send_client(num_clients - 1, player_number);
}


// closes the socket of a disconnected client
void handle_disconnect(int i)
{
	std::string name=clients[i].name;

	if(i<0 || i>=num_clients)
		return;
	
	// close the old socket, even if it's dead... 
	SDLNet_TCP_Close(clients[i].sock);
    clients[i].active = false;
    //std::cout << "Removed client # " << i << std::endl;
    //std::cin.ignore();
}


// Reconnects a client 
void reconnect_client(std::string name)
{
    clients[find_client_name(name)].active = true;
    // pass for now
}


// create a socket set that has the server socket and all the client sockets 
SDLNet_SocketSet create_sockset()
{
	static SDLNet_SocketSet set=NULL;

	if(set)
		SDLNet_FreeSocketSet(set);
	set = SDLNet_AllocSocketSet(num_clients + 1);
	if(!set)
    {
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
        return 0;
	}
	SDLNet_TCP_AddSocket(set, server);
	for(int i=0; i < num_clients; i++)
        if (clients[i].active)
            SDLNet_TCP_AddSocket(set, clients[i].sock);
	return(set);
}


// send a buffer to all clients 
void send_all(std::string buf)
{
	if(buf == "" || num_clients == 0)
		return;
    
    for (int i = 0; i < num_clients; i++)
	{
		if(!send_message(buf, clients[i].sock))
        {
            std::cout << "errr what ";
            handle_disconnect(i);
            std::cout << clients[i].active << std::endl;
        }
	}
}


// Send a string to a particular client
void send_client(int i, std::string buf)
{
	if (buf == "")
        return;

	send_message(buf, clients[i].sock);
}


// Generate the string to be sent or something
std::string generate_string_for_clients()
{
//     std::ostringstream ret;

//     ret << num_clients << ' ';
//     for (int i = 0; i < num_clients; i++)
//     {
//         ret << clients[i].active << ' ';
// 	}

//     return ret.str();
    return "bacon";
}


// possible inputs:
//     move "..."
//        - indicates a move the player made, the substring in ""
//        - should be sent to other players and spectators in the same game
//        - needs no alteration or additional work
//     draw
//        - indicates the player is offering a draw, or if a draw
//          had been previously offered without a move being made
//        - note: the offering player can choose to make a move instead
//          of waiting for a response, this will cancel the offer
//     nope
//        - indicates a player refusing a draw
//        - no more draws can be offered in the turn a nope occurs
//     quit
//        - indicates the player is leaving the game they are in and
//          returning to the main menu
//        - this includes players who are reviewing a game
//     make
//        - indicates the player would like to make a game
//        - they will be moved into a newly constructed game where
//          they are the first player
//     load
//        - indicates the player is viewing a loaded game
//        - the player will be moved to a vector of others doing the same
//     joins
//        - indicates the client would like to join the pre-existing game
//        - if the game has only one player the client becomes the other
//          player, else the client becomes a spectator
//     joinp
//        - indicates the client would like to join a pre-existing game
//        - if the game has only one player the client becomes the other
//          player, else the client becomes a spectator
//        - will attempt to join a game with only one player
//        - if there are no available games with only one player the server
//          returns a error message
//     exit
//        - the player quits the program by hitting the quit button
//        - the player is removed from the mainMenu vector,
//          and the vector of clients
void parse(std::string i, int sender)
{
    std::string command = i.substr(0, 4);
    std::cout << command << '\t' << i << std::endl;    
    if (command == "move")
    {
        move(i, sender);
    }
    else if (command == "quit")
    {
        quitGame(sender);
    }
    else if (command == "make")
    {
        make(sender);
    }
    else if (command == "join")
    {
        char target = i[4];
        if (target = 's')
        {
        }
        if (target = 'p')
        {
        }
    }
    else if (command == "load")
    {
    }
    else if (command == "draw")
    {
    }
    else if (command == "nope")
    {
    }
    else if (command == "exit")
    {
    }
}

void move(std::string & message, int s)
{
    std::string sender = clients[s].name;
    for (int i = 0; i < gamesInProgress.size(); i++)
    {
        if (gamesInProgress[i]->isPlayerN(sender))
        {
            gamesInProgress[i]->sendMove(message,sender);
            break;
        }
    }
}

void quitGame(int s)
{
    std::string sender = clients[s].name;
    Client * u = &(clients[s]);
    switch (clients[s].location)
    {
        case 1:
            clients[s].location = 0;
            for (int i = 0; i < reviewing.size(); i++)
            {
                if (reviewing[i] == u)
                {
                    reviewing.erase(reviewing.begin() + i);
                    break;
                }
            }
            mainMenu.push_back(&clients[s]);
            break;
        case 2:
            clients[s].location = 0;
            for (int i = 0; i < gamesInProgress.size(); i++)
            {
                if (gamesInProgress[i]->isPlayerA(u))
                {
                    gamesInProgress[i]->removeFromGame(u);
                    break;
                }
            }
            mainMenu.push_back(&clients[s]);
            break;
        case 3:
            clients[s].location = 0;
            for (int i = 0; i < gamesWaiting.size(); i++)
            {
                if (gamesWaiting[i]->isPlayerA(u))
                {
                    gamesWaiting[i]->removeFromGame(u);
                    break;
                }
            }
            mainMenu.push_back(&clients[s]);
            break;
        default:
            std::cout << "Aww shit, ERROR, ERROR, ERROR - " << clients[s].location
                      << " is not a recognized location, kindly go fuck yourself."
                      << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(0);
    }
}

void make(int c)
{
    Game temp = &(clients[c]);
    games.push_back(temp);
    gamesWaiting.push_back(&temp);

    clients[c].location = 3;
    for (int i = 0; i < mainMenu.size(); i++)
    {
        if (mainMenu[i] == &(clients[c]))
        {
            mainMenu.erase(mainMenu.begin() + i);
            break;
        }
    }
}

void joinSpectate(int c)
{
    int g = rand() % gamesInProgress.size();
    gamesInProgress[g]->addSpectator(&(clients[c]));
    for (int i = 0; i < mainMenu.size(); i++)
    {
        if (mainMenu[i] == &(clients[c]))
        {
            mainMenu.erase(mainMenu.begin() + i);
            clients[c].location = 2;
            break;
        }
    }
}
void joinPlay(int c)
{
    int g = rand() % gamesWaiting.size();
    gamesWaiting[g]->addPlayer(&(clients[c]));
    gamesWaiting[g]->startGame();
    gamesInProgress.push_back(gamesWaiting[g]);
    gamesWaiting.erase(gamesWaiting.begin() + g);
    for (int i = 0; i < mainMenu.size(); i++)
    {
        if (mainMenu[i] == &(clients[c]))
        {
            mainMenu.erase(mainMenu.begin() + i);
            break;
        }
    }
}
void load(int c)
{
    reviewing.push_back(&(clients[c]));
    for (int i = 0; i < mainMenu.size(); i++)
    {
        if (mainMenu[i] == &(clients[c]))
        {
            mainMenu.erase(mainMenu.begin() + i);
            clients[c].location = 1;
            break;
        }
    }
    
}
void draw(std::string & message, int c)
{
    Client * sender = &(clients[c]);
    for (int i = 0; i < gamesInProgress.size(); i++)
    {
        if (gamesInProgress[i]->isPlayerA(sender))
        {
            if (gamesInProgress[i]->getDraw())
            {
                gamesInProgress[i]->endGame();
                gamesInProgress.erase(gamesInProgress.begin() + i);
                break;
            }
            else
            {
                gamesInProgress[i]->sendDrawOffer(message,sender);
                break;
            }
        }
    }
}
void nope(std::string & message, int c)
{
    Client * sender = &(clients[c]);
    for (int i = 0; i < gamesInProgress.size(); i++)
    {
        if (gamesInProgress[i]->isPlayerA(sender))
        {
            gamesInProgress[i]->sendRefuseDraw(message,sender);
            break;
        }
    }
}
void exitProgram(int c)
{
    for (int i = 0; i < mainMenu.size(); i++)
    {
        if (mainMenu[i] == &(clients[c]))
        {
            mainMenu.erase(mainMenu.begin() + i);
            SDLNet_TCP_Close(clients[c].sock);
            clients.erase(clients.begin() + c);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    
	IPaddress ip;
	TCPsocket sock;
	SDLNet_SocketSet set;
	
	std::string message;
	
	Uint32 ipaddr;
	Uint16 port;
	
	// check our commandline
	if(argc < 2)
	{
		std::cout << argv[0] << "port" << std::endl;
		exit(0);
	}
	
	// initialize SDL 
	if(SDL_Init(0) == -1)
	{
		//printf("SDL_Init: %s\n",SDL_GetError());
		std::cout << "ERROR: couldn't initalize SDL" << std::endl;
		exit(1);
	}

	// initialize SDL_net
	if(SDLNet_Init() == -1)
	{
		//printf("SDLNet_Init: %s\n",SDLNet_GetError());
		std::cout << "ERROR: couldn't initalize SDLNet" << std::endl;
		SDL_Quit();
		exit(2);
	}

	// get the port from the commandline
	port = (Uint16)strtol(argv[1],NULL,0);

	//Resolve the argument into an IPaddress type 
	if(SDLNet_ResolveHost(&ip,NULL,port) == -1)
	{
		std::cout << "SDLNet_ResolveHost: ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		exit(3);
	}

	// open the server socket
	server = SDLNet_TCP_Open(&ip);
	if(!server)
	{
		std::cout << "SDLNet_TCP_Open ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		exit(4);
	}

	while(1)
	{
		int numready;
//        std::cout << "HERE " << std::endl;
		set = create_sockset();
//        std::cout << "ALSO HERE " << std::endl;
		numready = SDLNet_CheckSockets(set, (Uint32)1000);
        std::cout << "Numready " << numready << std::endl;
		if(numready == -1)
		{
			std::cout << "SDLNet_CheckSockets ERROR" << std::endl;
			break;
		}
		if(numready == 0)
			continue;
        
		if(SDLNet_SocketReady(server))
		{
			numready--;

			sock = SDLNet_TCP_Accept(server);
			if(sock)
			{
				std::string name;
				name = recv_message(sock);
                handle_login(sock , name);
            }
            else
                SDLNet_TCP_Close(sock);
		}


		//---------------------------------------------------------------------
		// LOOP THROUGH CLIENTS
		//---------------------------------------------------------------------
		std::cout << "numready : " << numready << '\n'
                  << "num_clients : " << num_clients << std::endl;
        for(int i = 0; numready > 0 && i < num_clients; i++)
		{
			std::cout << '\t' << i << std::endl;
			message = "";
            if (clients[i].active)
            {
                if(SDLNet_SocketReady(clients[i].sock))
                {
                    //---------------------------------------------------------
                    // GET DATA FROM CLIENT
                    //---------------------------------------------------------
                    message = recv_message(clients[i].sock);
                    
                    if(message > "") 
                    {
                        parse(message, i);
                    }
                    
                    numready--;
                }
            }
        }
	}
    
	// shutdown SDL_net
	SDLNet_Quit();

	// shutdown SDL
	SDL_Quit();

	return(0);
}
