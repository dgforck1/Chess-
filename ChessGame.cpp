#include <iostream>
#include <string>
#include "ChessGame.h"
#include "Chess.h"

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"



const int MYWIDTH = 900, MYHEIGHT = 650;

bool RectClicked(int mX, int mY, Rect&);

void ChessMain()
{
    Surface s(MYWIDTH, MYHEIGHT);
    Event event;
    Mouse mouse;

    int mousex = -1,
        mousey = -1;
    int workingPieceIndex = -1;
    bool clicked = false,
        released = false;

    Rect boardRect = Rect(250, 150, 400, 400);
    Rect blackRect = Rect(0, 0, 50, 50);
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

                    /*std::cout << "<<<< mouse click coords, x: "
                              << mousex
                              << " y: "
                              << mousey
                              << std::endl;*/
                    
                    clicked = true;
                }
                else if(event.type() == MOUSEBUTTONUP)
                {
                    mouse.update(event);
                    mousex = mouse.x();
                    mousey = mouse.y();

                    /*std::cout << "<<<< mouse release coords, x: "
                              << mousex
                              << " y: "
                              << mousey
                              << std::endl;*/
                    
                    released = true;
                }
            }
        }



        if(RectClicked(mousex, mousey, boardRect))  //player operating on board
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
                    //std::cout << "<<<< valid move!" << std::endl;
                    //b.capturePiece(tempy, tempx);
                    b.movePiece(workingPieceIndex, tempy, tempx);
                }
                else
                {
                    std::cout << "<<<< invalid move!" << std::endl;
                }
                
                //reset variables
                workingPieceIndex = -1;
                clicked = false;
                released = false;
            }
        }


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
        
        //draw all the things
        s.lock();
        s.fill(BLACK);
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
        s.unlock();
        s.flip();
        
        delay(10);

        //mayhaps call networking stuffs here?
    }
    
    
    
    //detect mouse click
    //get mouse coords
    //get board coords
    //get piece
    //detect mouse release
    //get mouse coords
    //get board coords
    //validate move
    //capture piece, if applicable
    //move piece
    //toggle player move
    //decrement clock, if applicable
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


