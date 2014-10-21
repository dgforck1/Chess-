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
#include "ChessGame.h"
#include "Chess.h"

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
// Function Prototypes
//////////////////////////////////////////////////////////////////////////////
int Welcome();


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

// bool RectClicked(int mX, int mY, Rect&);
// std::string MoveString(bool captured, std::string type, int rank, int file,
//     int sourceF);

// void ChessMain(int player)
// {
//     Surface s(W, H);
//     Event event;
//     Mouse mouse;

//     int mousex = -1,
//         mousey = -1,
//         workingPieceIndex = -1,
//         scrollstart = 2,
//         scrollend = 31;
//     bool clicked = false,
//         released = false,
//         scroll = false;

//     Rect boardRect = Rect(250, 150, 400, 400),
//         blackRect = Rect(0, 0, 50, 50),
//         capRect = Rect(25, 150, 200, 400),
//         movRect = Rect(675, 150, 200, 400),
//         supRect = Rect(850, 150, 25, 25),
//         sdwRect = Rect(850, 525, 25, 25);
//     Board b = Board();


    
//     Image WP = Image("images/WhitePawn.png");
//     Image WR = Image("images/WhiteRook.png");
//     Image WN = Image("images/WhiteKnight.png");
//     Image WB = Image("images/WhiteBishop.png");
//     Image WQ = Image("images/WhiteQueen.png");
//     Image WK = Image("images/WhiteKing.png");    
//     Image BP = Image("images/BlackPawn.png");
//     Image BR = Image("images/BlackRook.png");
//     Image BN = Image("images/BlackKnight.png");
//     Image BB = Image("images/BlackBishop.png");
//     Image BQ = Image("images/BlackQueen.png");
//     Image BK = Image("images/BlackKing.png");    
//     Image WPS = Image("images/WhitePawn-Small.png");
//     Image BPS = Image("images/BlackPawn-Small.png");
//     Image WRS = Image("images/WhiteRook-Small.png");
//     Image WNS = Image("images/WhiteKnight-Small.png");
//     Image WBS = Image("images/WhiteBishop-Small.png");
//     Image WQS = Image("images/WhiteQueen-Small.png");
//     Image WKS = Image("images/WhiteKing-Small.png");        
//     Image BRS = Image("images/BlackRook-Small.png");
//     Image BNS = Image("images/BlackKnight-Small.png");
//     Image BBS = Image("images/BlackBishop-Small.png");
//     Image BQS = Image("images/BlackQueen-Small.png");
//     Image BKS = Image("images/BlackKing-Small.png");
//     Image SUP = Image("images/ScrollUp.png");
//     Image SDW = Image("images/ScrollDown.png");
    
//     std::vector< std::string > CapturedWhite;
//     std::vector< std::string > CapturedBlack;
//     std::vector< std::string > Moves;
    
//     int playerTurn = 0;    
    
    
//     while(1)
//     {
//         if(event.poll())
//         {        
//             if(event.type() == QUIT) break;
//             else
//             {
//                 if(event.type() == MOUSEBUTTONDOWN)
//                 {
//                     mouse.update(event);                    
//                     mousex = mouse.x();
//                     mousey = mouse.y();                                        
//                     clicked = true;
//                 }
//                 else if(event.type() == MOUSEBUTTONUP)
//                 {
//                     mouse.update(event);
//                     mousex = mouse.x();
//                     mousey = mouse.y();                                       
//                     released = true;
//                 }
//             }
//         }
// // here is the first area I'll add to, if havent recieved message then wait
//         //if(playerTurn == player)
//             //only cares if it's the current player's turn
//         //{
//             //player operating on board
//             if(RectClicked(mousex, mousey, boardRect))                
//             {
//                 if(clicked && workingPieceIndex == -1)
//                 {
//                     //convert mouse coords to board coords (rank and file)
//                     int tempx = mousex - boardRect.x;
//                     int tempy = mousey - boardRect.y;
//                     tempx /= 50;
//                     tempy /= 50;                
//                     tempy = 7 - tempy;
                    
//                     workingPieceIndex = b.getPieceIndex(tempy, tempx);
                    
//                     if(workingPieceIndex >= 0)
//                     {
//                         //std::cout << "<<<< working piece: ";
//                         Piece tempP = b.getPiece(workingPieceIndex);

//                         /*tempP.print();
//                           std::cout << std::endl;*/
                        
//                         if(tempP.getPlayer()  != playerTurn)
//                         {
//                             //not the current player's piece
//                             workingPieceIndex = -1;                    
//                         }
//                     }
//                 }
                
//                 if(released)
//                 {                
//                     //convert mouse coords to board coords (rank and file)
//                     int tempx = mousex - boardRect.x;
//                     int tempy = mousey - boardRect.y;
//                     tempx /= 50;
//                     tempy /= 50;                
//                     tempy = 7 - tempy;
//                     Piece wp = b.getPiece(workingPieceIndex);
                    
//                     if(b.checkMove(workingPieceIndex, tempy, tempx))
//                     {
//                         std::string temp;                                    
                        
//                         if(b.getPieceIndex(tempy, tempx) >= 0)
//                         {
//                             int temp = b.getPieceIndex(tempy, tempx);
                            
                            
//                             Piece tempP = b.getPiece(tempy, tempx);

//                             Moves.push_back(
//                                 MoveString(true, wp.getType(),
//                                            tempy, tempx, wp.getFile())
//                                 );
                            
//                             //todo: fix weird capture bug
//                             if(tempP.getPlayer() == 0)
//                             {
//                                 CapturedWhite.push_back(                     
//                                     b.capturePiece(temp)
//                                     );
//                             }
//                             else
//                             {
//                                 CapturedBlack.push_back(               
//                                     b.capturePiece(temp)
//                                     );
//                             }
                                                        
//                         }
//                         else
//                         {
//                             Moves.push_back(
//                                 MoveString(false, wp.getType(),
//                                            tempy, tempx, wp.getFile())
//                                 );
//                         }

//                         if(Moves.size() > 30)
//                         {
//                             scroll = true;
//                             scrollstart = Moves.size() - 30 -
//                                 (Moves.size() % 2);
//                             scrollend = Moves.size() -
//                                 (Moves.size() % 2);                     
//                         }
                        
//                         b.movePiece(workingPieceIndex, tempy, tempx);         
                        
//                         if(playerTurn == 0)
//                         {
//                             playerTurn = 1;
//                         }
//                         else
//                         {
//                             playerTurn = 0;
//                         }


//                     }                
                    
//                     //reset variables
//                     workingPieceIndex = -1;
//                     clicked = false;
//                     released = false;
//                 }
//             }
//             else
//             {
//                 if(scroll)
//                 {
//                     if(RectClicked(mousex, mousey, supRect))
//                     {
//                         if(scrollstart > 0)
//                         {
//                             scrollstart -= 2;
//                             scrollend -= 2;
//                         }
                        
//                         std::cout << "<<<<scrolling!"
//                               << " start: " << scrollstart
//                               << " end: " << scrollend
//                               << std::endl;
//                     }

//                     if(RectClicked(mousex, mousey, sdwRect))
//                     {
//                         if(scrollend < Moves.size() - 1)
//                         {
//                             scrollstart += 2;
//                             scrollend += 2;
//                         }
                        
//                         std::cout << "<<<<scrolling!"
//                               << " start: " << scrollstart
//                               << " end: " << scrollend
//                               << std::endl;
//                     }
                    
//                 }
//             }
                
//             //}


//         //reset mouse variables
//         if(mousex != -1)
//         {
//             mousex = -1;
//         }

//         if(mousey != -1)
//         {
//             mousey = -1;
//         }
        

//         int toggle = -1;
//         int x = 0, y = 0;


        
//         //draw all the things
//         s.lock();
//         s.fill(GRAY);
//         s.put_rect(boardRect, WHITE);
//         s.put_rect(movRect, WHITE);
//         s.put_rect(capRect, WHITE);
//         s.put_image(WPS, Rect(250, 120, 25, 25));
//         s.put_image(BPS, Rect(625, 120, 25, 25));
//         //draw squares
//         for(int i = boardRect.x; i < boardRect.x + boardRect.w ; i+= 50)
//         {
//             for(int n = boardRect.y; n < boardRect.y + boardRect.h; n+= 50)
//             {
//                 blackRect.x = i;
//                 blackRect.y = n;

//                 if(toggle == 1)
//                 {
//                     s.put_rect(blackRect, RED);
//                 }
                
//                 toggle *= -1;
//             }
//             toggle *= -1;
//         }
//         //draw pieces
//         for(int i = 0; i < b.getPieceSize(); i++)
//         {            
//             Piece tempP = b.getPiece(i);

//             int tempx = boardRect.x;
//             int tempy = boardRect.y;            

            
//             tempx += tempP.getFile() * 50;
//             tempy += ((7 - tempP.getRank()) * 50);
            
//             if(tempP.getPlayer() == 0)
//             {
//                 if(tempP.getType() == "P")
//                 {                    
//                     s.put_image(WP, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "R")
//                 {                    
//                     s.put_image(WR, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "N")
//                 {                    
//                     s.put_image(WN, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "B")
//                 {                    
//                     s.put_image(WB, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "Q")
//                 {                    
//                     s.put_image(WQ, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "K")
//                 {                    
//                     s.put_image(WK, Rect(tempx, tempy, 50, 50));
//                 }
//             }
//             else if(tempP.getPlayer() == 1)
//             {
//                 if(tempP.getType() == "P")
//                 {                    
//                     s.put_image(BP, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "R")
//                 {                    
//                     s.put_image(BR, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "N")
//                 {                    
//                     s.put_image(BN, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "B")
//                 {                    
//                     s.put_image(BB, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "Q")
//                 {                    
//                     s.put_image(BQ, Rect(tempx, tempy, 50, 50));
//                 }
//                 else if(tempP.getType() == "K")
//                 {                    
//                     s.put_image(BK, Rect(tempx, tempy, 50, 50));
//                 }
//             }
//         }

//         //draw turn indicator
//         if(playerTurn == 0)
//         {
//             s.put_circle(250, 125, 5, GREEN);
//         }
//         else
//         {
//             s.put_circle(650, 125, 5, GREEN);
//         }
        
//         //draw captured white pieces
//         x = 25;
//         y = 150;
//         for(int i = 0; i < CapturedWhite.size(); i++)
//         {
//             if(CapturedWhite[i] == "P")
//             {
//                 s.put_image(WPS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "R")
//             {
//                 s.put_image(WRS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "N")
//             {
//                 s.put_image(WNS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "B")
//             {
//                 s.put_image(WBS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "Q")
//             {
//                 s.put_image(WQS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "K")
//             {
//                 s.put_image(WKS, Rect(x + (i * 25), y, 25, 25));
//             }
//             if(x + (i * 25) >= 200)
//             {
//                 x -= ((i * 25) + 25);
//                 y += 25;
//             }
//         }
//         //draw captured black pieces
//         x = 25;
//         y = 350;
//         for(int i = 0; i < CapturedBlack.size(); i++)
//         {
//             if(CapturedBlack[i] == "P")
//             {
//                 s.put_image(BPS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "R")
//             {
//                 s.put_image(BRS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "N")
//             {
//                 s.put_image(BNS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "B")
//             {
//                 s.put_image(BBS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "Q")
//             {
//                 s.put_image(BQS, Rect(x + (i * 25), y, 25, 25));
//             }
//             else if(CapturedWhite[i] == "K")
//             {
//                 s.put_image(BKS, Rect(x + (i * 25), y, 25, 25));
//             }
//             if(x + (i * 25) >= 200)
//             {
//                 x -= ((i * 25) + 25);
//                 y += 25;
//             }
//         }
//         //print moves
//         if(scroll)
//         {
//             x = 675;
//             y = 150;
            
//             for(int i = scrollstart; i <= scrollend; i++)
//             {
//                 TextSurface ts = TextSurface(
//                     Moves[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);
                
//                 s.put_text(ts, x, y);
//                 if(i % 2 == 1)
//                 {
//                     x = 675;
//                     y += 25;
//                 }
//                 else
//                 {
//                     x += 100;
//                 }
//             }

//             s.put_image(SUP, supRect);
//             s.put_image(SDW, sdwRect);
//         }        
//         else
//         {
//             x = 675;
//             y = 150;
//             for(int i = 0; i < Moves.size(); i++)
//             {
//                 TextSurface ts = TextSurface(
//                     Moves[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);
                
//                 s.put_text(ts, x, y);
//                 if(i % 2 == 1)
//                 {
//                     x = 675;
//                     y += 25;
//                 }
//                 else
//                 {
//                     x += 100;
//                 }                
//             }
//         }                
//         s.unlock();
//         s.flip();
        
//         delay(10);        
//     }
    
        
//     //todo:
//     //decrement clock, if applicable
// }


// bool RectClicked(int mX, int mY, Rect& r) //  ok as long as there is no overlap in rects i.e. no rects that share edges
// {
//     if(mX >= r.x && mX <= r.x + r.w)
//     {
//         if(mY >= r.y && mY <= r.y + r.h)
//         {
//             return true;
//         }
//     }
    
//     return false;
// }

// //build the move string
// std::string MoveString(bool captured, std::string type, int rank, int file,
//     int sourceF)
// {
//     std::string temp;

//     if(type != "P")
//     {
//         temp.append(type);
//     }

//     if(captured)
//     {
//         if(type == "P")
//         {
//             switch(sourceF)
//             {
//                 case 0:
//                     temp.append("a");
//                     break;
//                 case 1:
//                     temp.append("b");
//                     break;
//                 case 2:
//                     temp.append("c");
//                     break;
//                 case 3:
//                     temp.append("d");
//                     break;
//                 case 4:
//                     temp.append("e");
//                     break;
//                 case 5:
//                     temp.append("f");
//                     break;
//                 case 6:
//                     temp.append("g");
//                     break;
//                 case 7:
//                     temp.append("h");
//                     break;
//             }
//         }
        
//         temp.append("x");
//     }

//     switch(file)
//     {
//         case 0:
//             temp.append("a");
//             break;
//         case 1:
//             temp.append("b");
//             break;
//         case 2:
//             temp.append("c");
//             break;
//         case 3:
//             temp.append("d");
//             break;
//         case 4:
//             temp.append("e");
//             break;
//         case 5:
//             temp.append("f");
//             break;
//         case 6:
//             temp.append("g");
//             break;
//         case 7:
//             temp.append("h");
//             break;
//     }
    
//     switch(rank)
//     {
//         case 0:
//             temp.append("1");
//             break;
//         case 1:
//             temp.append("2");
//             break;
//         case 2:
//             temp.append("3");
//             break;
//         case 3:
//             temp.append("4");
//             break;
//         case 4:
//             temp.append("5");
//             break;
//         case 5:
//             temp.append("6");
//             break;
//         case 6:
//             temp.append("7");
//             break;
//         case 7:
//             temp.append("8");
//             break;
//     }
    
//     return temp;
// }

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

// int Welcome()
// {
//     int ret = -1;

//     Surface s(900, 650);
//     Event event;
//     Mouse mouse;

//     int mousex = -1, mousey = -1;
//     bool clicked = false,
//         released = false;

//     Rect playWhite = Rect(200, 400, 50, 50),
//         playBlack = Rect(200, 475, 50, 50),
//         exitGame = Rect(200, 550, 50, 50);


//     TextSurface welcomes = TextSurface(
//         "Chess!!!", "fonts/FreeSerif.ttf", 200, 255, 255, 255);

//     while(1)
//     {
//         if(event.poll())
//         {        
//             if(event.type() == QUIT) break;
//             else
//             {
//                 if(event.type() == MOUSEBUTTONDOWN)
//                 {
//                     mouse.update(event);                    
//                     mousex = mouse.x();
//                     mousey = mouse.y();                                        
//                     clicked = true;
//                 }
//                 else if(event.type() == MOUSEBUTTONUP)
//                 {
//                     mouse.update(event);
//                     mousex = mouse.x();
//                     mousey = mouse.y();                                       
//                     released = true;
//                 }
//             }
//         }


//         if(mousex >= playWhite.x && mousex <= playWhite.x + playWhite.w)
//         {
//             if(mousey >= playWhite.y && mousey <= playWhite.y + playWhite.h)
//             {
//                 return 0;
//             }
//         }

//         if(mousex >= playBlack.x && mousex <= playBlack.x + playBlack.w)
//         {
//             if(mousey >= playBlack.y && mousey <= playBlack.y + playBlack.h)
//             {
//                 return 1;
//             }
//         }

//         if(mousex >= exitGame.x && mousex <= exitGame.x + exitGame.w)
//         {
//             if(mousey >= exitGame.y && mousey <= exitGame.y + exitGame.h)
//             {
//                 return -1;
//             }
//         }
        


        
//         s.lock();
//         s.fill(GRAY);
//         s.put_text(welcomes, 100, 0);
//         s.put_text("Play as White", 275, 400, 0, 0, 0,
//                    "fonts/FreeSerif.ttf", 24);
//         s.put_text("Play as Black", 275, 475, 0, 0, 0,
//                    "fonts/FreeSerif.ttf", 24);
//         s.put_text("Exit Game", 275, 550, 0, 0, 0,
//                    "fonts/FreeSerif.ttf", 24);
//         s.put_rect(playWhite, WHITE);
//         s.put_rect(playBlack, BLACK);
//         s.put_rect(exitGame, RED);
//         s.unlock();
//         s.flip();
        
//         delay(10);
//     }
    
//     return ret;
// }



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
