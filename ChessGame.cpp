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

                //for testing only:
                Piece tempP = b.getPiece(workingPieceIndex);
                std::cout << "<<<< piece type: " << tempP.getType()
                          << " player: " << tempP.getPlayer()
                          << std::endl;
            }
            
            if(released)
            {
/*                std::cout << "<<<< this is where they wanna go, validate!"
                  << std::endl;*/
                
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


