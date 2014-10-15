#include <iostream>
#include <string>
#include "ChessGame.h"
#include "Chess.h"

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"



const int MYWIDTH = 900, MYHEIGHT = 650;

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

        if(clicked && workingPieceIndex == -1)
        {
            std::cout << "<<<< need to grab the piece they want to move"
                      << std::endl;

            workingPieceIndex = 0;
        }
        
        if(released)
        {
            std::cout << "<<<< this is where they wanna go, validate!"
                      << std::endl;
            
            //reset variables
            workingPieceIndex = -1;
            clicked = false;
            released = false;
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


        //draw all the things
        s.lock();
        s.fill(BLACK);
        s.unlock();
        s.flip();
        
        delay(25);

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
