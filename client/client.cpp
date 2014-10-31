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
#include "ChessGame.cpp"

//////////////////////////////////////////////////////////////////////////////
// END Includes.
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Constants.
/////////////////////////////////////////////////////////////////////////////
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

int Make(TCPsocket &, SDLNet_SocketSet &);
int Join(TCPsocket &, SDLNet_SocketSet &);
int Welcome();
int Load();
void getSaves(std::vector<std::string> &);


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
    if(mX >= r.x && mX < r.x + r.w)
    {
        if(mY >= r.y && mY < r.y + r.h)
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
	sock = SDLNet_TCP_Open(&ip);
	if(!sock)
	{
		std::cout << "SDLNet_TCP_Open ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		return 0;
	}
	
	if(SDLNet_TCP_AddSocket(set, sock) == -1)
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
    int choice = -1;
    bool play = true;
    
    while(play)
    {
        choice = Welcome(); //main menu
        
        
        switch(choice)
        {
            case 0: //exit game entirely
                play = false;
                break;
            case 1: //make game
                choice = Make(sock, set);                
                break;
            case 2: //join game
                choice = Join(sock, set);
                break;
            case 3: //watch game
                break;
            case 4: //load game
                //choice = Load();
                break;
            default:
                break;
        }
        
        if(choice == 0)
        {
            play = false;
        }
    }
    send_message("exit", sock);
    SDLNet_Quit();
    SDL_Quit();
    
    return(0);
}



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



int Make(TCPsocket & sock, SDLNet_SocketSet & set)
{
    send_message("make", sock);
    
    Surface sm(W, H);
    Event event;
    Mouse mouse;

    int mousex = -1, mousey = -1;
    bool clicked = false,
        released = false;
    
    //images
    Image searchI = Image("images/SearchForGame.png");
    Image cancelI = Image("images/Cancel.png");
    
    //rects
    Rect searchR = searchI.getRect();
    Rect cancelR = cancelI.getRect();

    //set rect locations
    searchR.x = (W / 2) - (searchR.w / 2);
    searchR.y = (H / 2) - (searchR.h / 2);
    
    bool foundGame = false;
    while(!foundGame)
    {
        int numready = SDLNet_CheckSockets(set, 100);
        if(numready == -1)
		{
			std::cout << "SDLNet_CheckSockets ERROR" << std::endl;
			break;
		}
        
        std::string from_server = "";
		if(numready && SDLNet_SocketReady(sock))
		{
			from_server = recv_message(sock);
            if (from_server == "good")
            {
                foundGame = true;
                playerSide = 0;
            }
            else
            {
                std::cout << "shit: ";
            }
            std::cout << from_server << std::endl;// comment out after complete
            
//            parse_player_data(from_server);
		}
        if(event.poll())
        {        
            if(event.type() == QUIT)
            {
                send_message("quit", sock);
                return 0;
            }
            else
            {
                if(event.type() == MOUSEBUTTONDOWN)
                {
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();                                        
                    clicked = true;
                }
            }
        }
        if(clicked)
        {
            if(MainRectClicked(mousex, mousey, cancelR))
            {
                send_message("quit", sock);
                return -1;
            }            

            //reset vars
            clicked = false;
            mousex = -1;
            mousey = -1;
        }

    
        //print all the things!!!
        sm.lock();
        sm.fill(GRAY);
        sm.put_image(searchI, searchR);
        sm.put_image(cancelI, cancelR);
        sm.unlock();
        sm.flip();
        
        delay(10);
    }
    // call ChessMain here
    ChessMain(sock, set, playerSide);
    return -1;
}

int Join(TCPsocket & sock, SDLNet_SocketSet & set)
{
    send_message("joinp", sock);
    Surface sm(W, H);
    Event event;
    Mouse mouse;

    int mousex = -1, mousey = -1;
    bool clicked = false,
        released = false;


    //images
    Image searchI = Image("images/SearchForGame.png");
    Image cancelI = Image("images/Cancel.png");
    
    
    
    //rects
    Rect searchR = searchI.getRect();
    Rect cancelR = cancelI.getRect();
    

    //set rect locations
    searchR.x = (W / 2) - (searchR.w / 2);
    searchR.y = (H / 2) - (searchR.h / 2);
    
    bool foundGame = false;
    while(!foundGame)
    {
        int numready = SDLNet_CheckSockets(set, 100);
        if(numready == -1)
		{
			std::cout << "SDLNet_CheckSockets ERROR" << std::endl;
			break;
		}
        
        std::string from_server = "";
		if(numready && SDLNet_SocketReady(sock))
		{
			from_server = recv_message(sock);
            if (from_server == "good")
            {
                foundGame = true;
                playerSide = 1;
            }
            std::cout << from_server << std::endl;// comment out after complete
            
//            parse_player_data(from_server);
		}
        if(event.poll())
        {        
            if(event.type() == QUIT)
            {
                send_message("quit", sock);
                return 0;
            }
            else
            {
                if(event.type() == MOUSEBUTTONDOWN)
                {
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();                                        
                    clicked = true;
                }
            }
        }
        
        

        if(clicked)
        {
            if(MainRectClicked(mousex, mousey, cancelR))
            {
                send_message("quit", sock);
                return -1;
            }            

            //reset vars
            clicked = false;
            mousex = -1;
            mousey = -1;
        }

    
        //print all the things!!!
        sm.lock();
        sm.fill(GRAY);
        sm.put_image(searchI, searchR);
        sm.put_image(cancelI, cancelR);
        sm.unlock();
        sm.flip();
        
        delay(10);
    }
    ChessMain(sock, set, playerSide);
    return -1;
}


int Welcome()
{
    int ret = -1;

    Surface sm(W, H);
    Event event;
    Mouse mouse;

    int mousex = -1, mousey = -1;
    bool clicked = false,
        released = false;


    TextSurface welcomes = TextSurface(
        "Chess!!!", "fonts/FreeSerif.ttf", 200, 255, 255, 255);

    //images
    Image exitI = Image("images/Exit.png");
    Image makeI = Image("images/Make.png");
    Image joinI = Image("images/Join.png");
    Image watchI = Image("images/Watch.png");
    Image loadI = Image("images/Load.png");
    
    
    //rects
    Rect exitR = exitI.getRect();
    Rect makeR = makeI.getRect();
    Rect joinR = joinI.getRect();
    Rect watchR = watchI.getRect();
    Rect loadR = loadI.getRect();
    
    
    //set rect coords
    exitR.x = 900 - exitR.w;
    
    makeR.x = 150;
    makeR.y = 300;
    
    joinR.x = makeR.x;
    joinR.y = makeR.y + (joinR.h * 1.5);

    watchR.x = makeR.x + (watchR.w * 1.5);
    watchR.y = makeR.y;

    loadR.x = watchR.x;
    loadR.y = watchR.y + (loadR.h * 1.5);

    while(1)
    {
        if(event.poll())
        {        
            if(event.type() == QUIT)
            {
                return 0;
            }
            else
            {
                if(event.type() == MOUSEBUTTONDOWN)
                {
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();                                        
                    clicked = true;
                }
                else if(event.type() == MOUSEBUTTONUP)
                {
                    mouse.update(event);
                    mousex = mouse.x();
                    mousey = mouse.y();                                       
                    released = true;
                }
            }
        }

        

        if(clicked)
        {
            if(MainRectClicked(mousex, mousey, exitR))
            {
                return 0;
            }
            else if(MainRectClicked(mousex, mousey, makeR))
            {
                return 1;
            }
            else if(MainRectClicked(mousex, mousey, joinR))
            {
                return 2;
            }
            else if(MainRectClicked(mousex, mousey, watchR))
            {
                return 3;
            }
            else if(MainRectClicked(mousex, mousey, loadR))
            {
                return 4;
            }

            //reset vars
            clicked = false;
            mousex = -1;
            mousey = -1;
        }

                       
        //print all the things!!!
        sm.lock();
        sm.fill(GRAY);
        sm.put_text(welcomes, 100, 0);        
        sm.put_image(exitI, exitR);
        sm.put_image(makeI, makeR);
        sm.put_image(joinI, joinR);
        sm.put_image(watchI, watchR);
        sm.put_image(loadI, loadR);
        sm.unlock();
        sm.flip();
        
        delay(10);
    }    
    
    return ret;
}

int Load()
{        
    Surface sm(W, H);
    Event event;
    Mouse mouse;
    
    int mousex = -1, mousey = -1, saveindex = -1;
    bool clicked = false,
        released = false;
    

    //images
    Image exitI = Image("images/Exit.png");
    
    
    
    //rects
    Rect exitR = exitI.getRect();
    

    //set rect locations
    exitR.x = 900 - exitR.w;

    std::vector< std::string > SaveFiles; //list of all of the save files
    std::vector< std::string > Contents; //file contents
    
       

    getSaves(SaveFiles);
    
    while(1)
    {
        if(event.poll())
        {        
            if(event.type() == QUIT)
            {
                return 0;
            }
            else
            {
                if(event.type() == MOUSEBUTTONDOWN)
                {
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();                                        
                    clicked = true;
                }
            }
        }
        
        

        if(clicked)
        {
            saveindex= -1;
            
            if(MainRectClicked(mousex, mousey, exitR))
            {
                return 0;
            }

            for(int i = 0; i < SaveFiles.size(); i++)            
            {
                TextSurface ts = TextSurface(
                    SaveFiles[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);
                
                Rect tempR =
                    Rect(20, (i * ts.getHeight()) + 100, ts.getWidth(),
                         ts.getHeight());

                if(MainRectClicked(mousex, mousey, tempR))
                {
                    saveindex = i;
                }                
            }

            if(saveindex > -1)
            {
                //this is a hack in place of having time to create
                //a text box in sdl
                if(system(NULL))
                {
                    std::string c = "pluma saves/" + SaveFiles[saveindex];
                    system(c.c_str());
                }
                
            }
            
            //reset vars
            clicked = false;
            mousex = -1;
            mousey = -1;
        }
        
        
        //print all the things!!!
        sm.lock();
        sm.fill(GRAY);
        //print saves games        
        for(int i = 0; i < SaveFiles.size(); i++)
        {            
            TextSurface ts = TextSurface(
                SaveFiles[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);

            sm.put_rect(20, (i * ts.getHeight()) + 100, ts.getWidth(),
                        ts.getHeight(), 0, 200, 0);
            
            sm.put_text(ts, 20, (i * ts.getHeight()) + 100);
        }
        sm.put_image(exitI, exitR);
        sm.unlock();
        sm.flip();
        
        delay(10);
    }
    
}


void getSaves(std::vector<std::string> &s)
{
    std::fstream saves("saves/saves.txt", std::fstream::in
                       | std::fstream::app);
    
    if(saves.is_open())
    {
        std::string temp;               
        //saves >> temp;
        
        //get list of saved files from saves/saves.txt
        while (std::getline(saves, temp))
        {
            s.push_back(temp);
                 
        }        
    
        saves.close();        
    }
}



