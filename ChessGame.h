#ifndef CHESSGAME_H
#define CHESSGAME_H


//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"

//////////////////////////////////////////////////////////////////////////////
// Start Client States
//////////////////////////////////////////////////////////////////////////////
#define SEARCHING 0
#define MAKING 1
#define SPECTATING 2
#define PLAYING 3
//////////////////////////////////////////////////////////////////////////////
// End Client States
//////////////////////////////////////////////////////////////////////////////

// random ass fucntion
void ChessMain();

//////////////////////////////////////////////////////////////////////////////
// Class Definitions
//////////////////////////////////////////////////////////////////////////////

class Client
{
public:    
    Client(TCPsocket s = NULL, std::string n = "",
           bool i = false, bool a = false)
        : sock(s), name(n), ingame(i), active(a)
    {}
    
	TCPsocket sock;
	std::string name;
    bool active;
};

class Game
{
public:
    // the constructor will always need a player, that will be player 1
    Game(Client & p1)
    {
        players.push_back(p1);
    }

    // add spectator
    void addSpectator (Client & s);
    // add player, if there are already 2 players add a spectator instead
    void addPlayer (Client & p);
    
private:
    //Client player1;
    //Client player2;
    std::vector<Client> players;
    std::vector<Client> spectators;
}






#endif
