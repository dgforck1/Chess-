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
    Client(TCPsocket s=NULL, std::string n="",
           bool i=false, bool a=false)
        : sock(s), name(n), ingame(i), active(a)
    {}
    
	TCPsocket sock;
	std::string name;
    bool active;
};

class Game
{
public:
    Game(Client & p1)
    {
        players.push_back(p1);
    }

    void addSpectator (Client & s);
    // add spectator
    // add player
    // the constructor will always need a player, that will be player 1
    
private:
    //Client player1;
    //Client player2;
    std::vector<Client> players;
    std::vector<Client> spectators;
}






#endif
