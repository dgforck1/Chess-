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

// Our Includes
#include "chess.cpp"

//////////////////////////////////////////////////////////////////////////////
// END Includes.
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Constants.
/////////////////////////////////////////////////////////////////////////////
const int MAXLEN = 1024;
#define QUITS -1
#define MAINMENU 0
#define PLAYING 1
#define WATCHING 2
#define WAITING 3
#define LOADED 4

/////////////////////////////////////////////////////////////////////////////
// Global Variables.
/////////////////////////////////////////////////////////////////////////////
//Player players;
int player_number = -1;
int state = -1;
int playerSide = -1;
bool draw = false;


//////////////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// Functions
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

bool MainRectClicked(int mX, int mY, Rect& r)
{
    if(mX >= r.x && mX <= r.x + r.w)
    {
        if(mY >= r.y && mY <= r.y + r.h)
        {
            return true;
        }
    }
    
    return false;
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
    
// 	//-------------------------------------------------------------------------
// 	// GAME SEGMENT
// 	//-------------------------------------------------------------------------
    Surface sm(W, H);
    Event event;
    Mouse mouse;
    int mousex = -1, mousey = -1;
    bool clicked = false, released = false;
    state = 0;
    
    TextSurface welcomes = TextSurface("Chess!!!",
                                       "fonts/FreeSerif.ttf",
                                       200, 255, 255, 255);
    
    //images  
    Image exitI = Image("images/Exit.png");
    Image makeI = Image("images/Make.png");
//     Image joinI = Image("images/Join.png");
//     Image watchI = Image("images/Watch.png");
//     Image loadI = Image("images/Load.png");
    
//     //rects
//     Rect exitR = exitI.getRect();
//     Rect makeR = makeI.getRect();
//     Rect joinR = joinI.getRect();
//     Rect watchR = watchI.getRect();
//     Rect loadR = loadI.getRect();
    
//     //set rect coords
//     exitR.x = 900 - exitR.w;
//     makeR.x = 150;
//     makeR.y = 300;
//     joinR.x = makeR.x;
//     joinR.y = makeR.y + (joinR.h * 1.5);
//     watchR.x = makeR.x + (watchR.w * 1.5);
//     watchR.y = makeR.y;
//     loadR.x = watchR.x;
//     loadR.y = watchR.y + (loadR.h * 1.5);
    
    
// 	while(1)
// 	{
//         // int t = SDL_GetTicks();
// //         std::cout << "start of loop" << std::endl;
//         ///////////////////////////////////////////////////////////////////
//         // Get shit from server
//         ///////////////////////////////////////////////////////////////////
//         numready=SDLNet_CheckSockets(set, 100);
//         if(numready == -1)
// 		{
// 			std::cout << "SDLNet_CheckSockets ERROR" << std::endl;
// 			break;
// 		}
        
//         from_server = "";
// 		if(numready && SDLNet_SocketReady(sock))
// 		{
// 			from_server = recv_message(sock);
//             std::cout << from_server << std::endl;// comment out after complete
            
// //            parse_player_data(from_server);
// 		}
        
//         // int t1 = SDL_GetTicks();
// //         std::cout << "After server stuff: " << t1 - t << std::endl;
        
//         if (state == QUITS)
//             // leave program
//         {                    
//             break;
//         }
//         else if (state == MAINMENU)
//         {
//             if(event.poll())
//             {        
//                 if(event.type() == QUIT)
//                 {
//         //             std::cout << "in the quit spot" << std::endl;
//                     break;
//                     //state = QUITS;
//                 }
//                 else
//                 {
//                     if(event.type() == MOUSEBUTTONDOWN)
//                     {
//                         mouse.update(event);                    
//                         mousex = mouse.x();
//                         mousey = mouse.y();                                        
//                         clicked = true;
//                     }
//                     else if(event.type() == MOUSEBUTTONUP)
//                     {
//                         mouse.update(event);
//                         mousex = mouse.x();
//                         mousey = mouse.y();                                       
//                         released = true;
//                     }
//                 }
//             }
            
//             if(clicked)
//             {
//                 if(MainRectClicked(mousex, mousey, exitR))
//                 {
//                     std::cout << "quiting" << std::endl;
//                     state = QUITS;
//                 }
//                 else if(MainRectClicked(mousex, mousey, makeR))
//                 {
//                     std::cout << "makeing game" << std::endl;
//                     state = WAITING;
//                 }
//                 else if(MainRectClicked(mousex, mousey, joinR))
//                 {
//                     std::cout << "joining game" << std::endl;
//                     state = WAITING;
//                 }
//                 else if(MainRectClicked(mousex, mousey, watchR))
//                 {
//                     std::cout << "watching game" << std::endl;
//                     state = WAITING;
//                 }
//                 else if(MainRectClicked(mousex, mousey, loadR))
//                 {
//                     std::cout << "loading game" << std::endl;
//                     state = LOADED;
//                 }
                
//                 //reset vars
//                 clicked = false;
//                 mousex = -1;
//                 mousey = -1;
//             }
            
//         //     t1 = SDL_GetTicks();
// //             std::cout << "After body in main: " << t1 - t << std::endl;
            
//             //print all the things!!!
//             sm.lock();
//             sm.fill(GRAY);
//             sm.put_text(welcomes, 100, 0);        
//             sm.put_image(exitI, exitR);
//             sm.put_image(makeI, makeR);
//             sm.put_image(joinI, joinR);
//             sm.put_image(watchI, watchR);
//             sm.put_image(loadI, loadR);
//             sm.flip();
//             sm.unlock();
            
//         //     t1 = SDL_GetTicks();
//         //     std::cout << "after print in main: " << t1 - t << std::endl;
//         }
//         else if (state == PLAYING)
//         {}
//         else if (state == WATCHING)
//         {}
//         else if (state == WAITING)
//         {}
//         else if (state == LOADED)
//         {}
        
//         delay(10);   
//     }
    send_message("exit", sock);
    SDLNet_Quit();
    SDL_Quit();
    
    return(0);
}
