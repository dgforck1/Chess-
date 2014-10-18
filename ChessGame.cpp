#include <iostream>
#include <string>
#include <cstring>
#include "ChessGame.h"
#include "Chess.h"

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"



const int MYWIDTH = 900, MYHEIGHT = 650;

bool RectClicked(int mX, int mY, Rect&);

void ChessMain(int player)
{
    Surface s(MYWIDTH, MYHEIGHT);
    Event event;
    Mouse mouse;

    int mousex = -1,
        mousey = -1,
        workingPieceIndex = -1;
    bool clicked = false,
        released = false;

    Rect boardRect = Rect(250, 150, 400, 400),
        blackRect = Rect(0, 0, 50, 50),
        capRect = Rect(25, 150, 200, 400);
    Board b = Board();
        
    Image WP = Image("images/WhitePawn.png");
    Image WR = Image("images/WhiteRook.png");
    Image WN = Image("images/WhiteKnight.png");
    Image WB = Image("images/WhiteBishop.png");
    Image WQ = Image("images/WhiteQueen.png");
    Image WK = Image("images/WhiteKing.png");    
    Image BP = Image("images/BlackPawn.png");
    Image BR = Image("images/BlackRook.png");
    Image BN = Image("images/BlackKnight.png");
    Image BB = Image("images/BlackBishop.png");
    Image BQ = Image("images/BlackQueen.png");
    Image BK = Image("images/BlackKing.png");
    
    Image WPS = Image("images/WhitePawn-Small.png");
    Image BPS = Image("images/BlackPawn-Small.png");
    Image WRS = Image("images/WhiteRook-Small.png");
    Image WNS = Image("images/WhiteKnight-Small.png");
    Image WBS = Image("images/WhiteBishop-Small.png");
    Image WQS = Image("images/WhiteQueen-Small.png");
    Image WKS = Image("images/WhiteKing-Small.png");        
    Image BRS = Image("images/BlackRook-Small.png");
    Image BNS = Image("images/BlackKnight-Small.png");
    Image BBS = Image("images/BlackBishop-Small.png");
    Image BQS = Image("images/BlackQueen-Small.png");
    Image BKS = Image("images/BlackKing-Small.png");
    
    std::vector< std::string > CapturedWhite;
    std::vector< std::string > CapturedBlack;
    
    int playerTurn = 0;    
    
    
    while(1)
    {
        if(event.poll())
        {        
            if(event.type() == QUIT) break;
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

        //if(playerTurn == player)
            //only cares if it's the current player's turn
        //{
            //player operating on board
            if(RectClicked(mousex, mousey, boardRect))                
            {
                if(clicked && workingPieceIndex == -1)
                {
                    //convert mouse coords to board coords (rank and file)
                    int tempx = mousex - boardRect.x;
                    int tempy = mousey - boardRect.y;
                    tempx /= 50;
                    tempy /= 50;                
                    tempy = 7 - tempy;
                    
                    workingPieceIndex = b.getPieceIndex(tempy, tempx);
                    
                    if(workingPieceIndex >= 0)
                    {
                        std::cout << "<<<< working piece: ";
                        Piece tempP = b.getPiece(workingPieceIndex);

                        tempP.print();
                        std::cout << std::endl;
                        
                        if(tempP.getPlayer()  != playerTurn)
                        {
                            //not the current player's piece
                            workingPieceIndex = -1;                    
                        }
                    }
                }
                
                if(released)
                {                
                    //convert mouse coords to board coords (rank and file)
                    int tempx = mousex - boardRect.x;
                    int tempy = mousey - boardRect.y;
                    tempx /= 50;
                    tempy /= 50;                
                    tempy = 7 - tempy;
                    
                    if(b.checkMove(workingPieceIndex, tempy, tempx))
                    {
                        std::cout << "<<<< tempy: " << tempy
                                  << " tempx: " << tempx
                                  << std::endl;
                        
                        if(b.getPieceIndex(tempy, tempx) >= 0)
                        {
                            int temp = b.getPieceIndex(tempy, tempx);
                            
                            std::cout << "<<<< piece to capture: ";
                            Piece tempP = b.getPiece(tempy, tempx);

                            tempP.print();
                            std::cout << std::endl;
                            
                            //todo: fix weird capture bug
                            if(tempP.getPlayer() == 0)
                            {
                                CapturedWhite.push_back(
                                    //b.capturePiece(tempy, tempx)
                                    b.capturePiece(temp)
                                    );
                            }
                            else
                            {
                                CapturedBlack.push_back(               
                                    //b.capturePiece(tempy, tempx)
                                    b.capturePiece(temp)
                                    );
                            }

                            
                            /*std::cout << "<<<< printing all pieces\n";
                            for(int n = 0; n < b.getPieceSize(); n++)
                            {
                                std::cout << "    n: " << n;
                                Piece tP = b.getPiece(n);
                                
                                tP.print();
                                
                                std::cout << std::endl;
                                }*/
                        }
                        
                        b.movePiece(workingPieceIndex, tempy, tempx);

                        //b.updateBoard();
                        //b.printBoard();
                        
                        if(playerTurn == 0)
                        {
                            playerTurn = 1;
                        }
                        else
                        {
                            playerTurn = 0;
                        }
                    }                
                    
                    //reset variables
                    workingPieceIndex = -1;
                    clicked = false;
                    released = false;
                }
            }
            //}


        //reset mouse variables
        if(mousex != -1)
        {
            mousex = -1;
        }

        if(mousey != -1)
        {
            mousey = -1;
        }


        int toggle = -1;
        int x = 0, y = 0;
        
        //draw all the things
        s.lock();
        s.fill(GRAY);
        s.put_rect(boardRect, WHITE);
        //draw squares
        for(int i = boardRect.x; i < boardRect.x + boardRect.w ; i+= 50)
        {
            for(int n = boardRect.y; n < boardRect.y + boardRect.h; n+= 50)
            {
                blackRect.x = i;
                blackRect.y = n;

                if(toggle == 1)
                {
                    s.put_rect(blackRect, RED);
                }
                
                toggle *= -1;
            }
            toggle *= -1;
        }
        //draw pieces
        for(int i = 0; i < b.getPieceSize(); i++)
        {            
            Piece tempP = b.getPiece(i);

            int tempx = boardRect.x;
            int tempy = boardRect.y;            

            
            tempx += tempP.getFile() * 50;
            tempy += ((7 - tempP.getRank()) * 50);
            
            if(tempP.getPlayer() == 0)
            {
                if(tempP.getType() == "P")
                {                    
                    s.put_image(WP, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "R")
                {                    
                    s.put_image(WR, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "N")
                {                    
                    s.put_image(WN, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "B")
                {                    
                    s.put_image(WB, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "Q")
                {                    
                    s.put_image(WQ, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "K")
                {                    
                    s.put_image(WK, Rect(tempx, tempy, 50, 50));
                }
            }
            else if(tempP.getPlayer() == 1)
            {
                if(tempP.getType() == "P")
                {                    
                    s.put_image(BP, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "R")
                {                    
                    s.put_image(BR, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "N")
                {                    
                    s.put_image(BN, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "B")
                {                    
                    s.put_image(BB, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "Q")
                {                    
                    s.put_image(BQ, Rect(tempx, tempy, 50, 50));
                }
                else if(tempP.getType() == "K")
                {                    
                    s.put_image(BK, Rect(tempx, tempy, 50, 50));
                }
            }
        }
        s.put_image(WPS, Rect(250, 120, 25, 25));
        s.put_image(BPS, Rect(625, 120, 25, 25));
        if(playerTurn == 0)
        {
            s.put_circle(250, 125, 5, GREEN);
        }
        else
        {
            s.put_circle(650, 125, 5, GREEN);
        }
        s.put_rect(capRect, WHITE);
        //draw captured white pieces
        x = 25;
        y = 150;
        for(int i = 0; i < CapturedWhite.size(); i++)
        {
            if(CapturedWhite[i] == "P")
            {
                s.put_image(WPS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "R")
            {
                s.put_image(WRS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "N")
            {
                s.put_image(WNS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "B")
            {
                s.put_image(WBS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "Q")
            {
                s.put_image(WQS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "K")
            {
                s.put_image(WKS, Rect(x + (i * 25), y, 25, 25));
            }
            if(x + (i * 25) >= 200)
            {
                x -= ((i * 25) + 25);
                y += 25;
            }
        }
        //draw captured black pieces
        x = 25;
        y = 350;
        for(int i = 0; i < CapturedBlack.size(); i++)
        {
            if(CapturedBlack[i] == "P")
            {
                s.put_image(BPS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "R")
            {
                s.put_image(BRS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "N")
            {
                s.put_image(BNS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "B")
            {
                s.put_image(BBS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "Q")
            {
                s.put_image(BQS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "K")
            {
                s.put_image(BKS, Rect(x + (i * 25), y, 25, 25));
            }
            if(x + (i * 25) >= 200)
            {
                x -= ((i * 25) + 25);
                y += 25;
            }
        }
        s.unlock();
        s.flip();
        
        delay(10);

        //mayhaps call networking stuffs here?
    }
    
    
    
    //todo:
    //decrement clock, if applicable
    //print captured pieces
    //track moves
    //print moves
    
}


bool RectClicked(int mX, int mY, Rect& r)
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


// add spectator
void Game::addSpectator (Client & s)
{
    spectators.push_back(s);
}

// add player, if there are already 2 players add a spectator instead
void Game::addPlayer (Client & p)
{
    if (players.size() > 1)
        addSpectator(p);
    else
        players.push_back(p);
}
