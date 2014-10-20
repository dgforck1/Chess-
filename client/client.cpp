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


//////////////////////////////////////////////////////////////////////////////
// END Includes.
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Constants.
/////////////////////////////////////////////////////////////////////////////
const int MAXLEN = 1024;

/////////////////////////////////////////////////////////////////////////////
// Global Variables.
/////////////////////////////////////////////////////////////////////////////
//Player players;
int player_number = -1;



//////////////////////////////////////////////////////////////////////////////
// Functions.
//////////////////////////////////////////////////////////////////////////////

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
    SDLNet_TCP_Send(sock, buff, MAXLEN);

    return 1;
}


//////////
// Parse data here, need to figure out just what we will be passing along
//////////


void recv_player_number(std::string message)
{
	int i = 0;
	std::string temp_num = "";

	if (message[0] == 'N')
    {
		i++;
		while (message[i] != ';')
        {
			temp_num += message[i];
			i++;
		}
	}

	player_number = atoi(temp_num.c_str());
}


int main(int argc, char **argv)
{
	IPaddress ip;
	TCPsocket sock;
  
	int numready;
	Uint16 port;
	SDLNet_SocketSet set;	

	std::string name;
	std::string to_server;
	std::string from_server;

    /* check our commandline */
	if(argc < 4)
	{
		std::cout << "Usage: " << argv[0] << " host_ip host_port username" << std::endl;
		return 0;
	}
	
	name = argv[3];
	
	/* initialize SDL */
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "SDL_Init ERROR" << std::endl;
		return 0;
	}

	/* initialize SDL_net */
	if(SDLNet_Init() == -1)
	{
		std::cout << "SDLNet_Init ERROR" << std::endl;
		SDL_Quit();
		return 0;
	}

	set = SDLNet_AllocSocketSet(1);
	if(!set)
	{
		std::cout << "SDLNet_AllocSocketSet ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		return 0;
	}

	/* get the port from the commandline */
	port = (Uint16)strtol(argv[2],NULL,0);
	
	/* Resolve the argument into an IPaddress type */
	std::cout << "connecting to " << argv[1] << " port " << port << std::endl;

	if(SDLNet_ResolveHost(&ip,argv[1],port) == -1)
	{
		std::cout << "SDLNet_ResolveHost ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		return 0;
	}

	/* open the server socket */
	sock=SDLNet_TCP_Open(&ip);
	if(!sock)
	{
		std::cout << "SDLNet_TCP_Open ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		return 0;
	}
	
	if(SDLNet_TCP_AddSocket(set,sock)==-1)
	{
		std::cout << "SDLNet_TCP_AddSocket ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		return 0;
	}

	send_message(name, sock);

	std::cout << "Logged in as: " << name << std::endl;
	
	recv_player_number(recv_message(sock));

	std::cout << "player num: " << player_number << std::endl;

	//-------------------------------------------------------------------------
	// GAME SEGMENT
	//-------------------------------------------------------------------------
	Surface surface(W, H);
	Event event;

	while(1)
	{
        numready=SDLNet_CheckSockets(set, 100);
        if(numready == -1)
		{
			std::cout << "SDLNet_CheckSockets ERROR" << std::endl;
			break;
		}

		//-------------------------------------------------------------------------------
		// GET DATA FROM SERVER
		//-------------------------------------------------------------------------------
		from_server = "";
		if(numready && SDLNet_SocketReady(sock))
		{
			from_server = recv_message(sock);
            std::cout << from_server << std::endl;

//            parse_player_data(from_server);
		}

		if (event.poll() && event.type() == QUIT) break;

		KeyPressed keypressed = get_keypressed();

		to_server = "";
		if (keypressed[UPARROW])
        {
			to_server = "draw";
			send_message(to_server, sock);
		}
		else if (keypressed[DOWNARROW])
        {
			to_server = "quit";
			send_message(to_server, sock);
		}
		else if (keypressed[TAB])
        {
			to_server = "load";
			send_message(to_server, sock);
		}
		else if (keypressed[SPACE])
        {
			to_server = "nope";
			send_message(to_server, sock);
		}
		else if (keypressed[LEFTARROW])
        {
			to_server = "make";
			send_message(to_server, sock);
		}
		else if (keypressed[RIGHTARROW])
        {
			to_server = "exit";
			send_message(to_server, sock);
		}

/////////////////////////////////////////////////////////////////////////////
// print area
/////////////////////////////////////////////////////////////////////////////
            
		surface.fill(WHITE);
//		   for (int i = 0; i < players.size(); i++)
//         {
//             if (players[i].status)
//             {
//                 surface.lock();
//                 surface.put_rect(players[i].x, players[i].y,
//                                  players[i].size, players[i].size,
//                                  players[i].color[0],
//                                  players[i].color[1],
//                                  players[i].color[2]);
//                 surface.unlock();
//             }
//         }

		surface.flip();


		delay(1);
	}

	SDLNet_Quit();
	SDL_Quit();

	return(0);
}
