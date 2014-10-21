//standard libraries
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

//custom libraries
#include "Chess.h"
#include "ChessGame.h"



int Welcome();



int main()
{
    //bool RectClicked(int mX, int mY, Rect&);
    
    int player = 0;
    //need some way to determine who is white and who is black
    // 0 = white, 1 = black

    /*player = Welcome();
    if(player > -1)
    {*/
        ChessMain(player);
        //}
    
    
    return 0;
}


int Welcome()
{
    int ret = -1;

    Surface sm(900, 650);
    Event event;
    Mouse mouse;

    int mousex = -1, mousey = -1;
    bool clicked = false,
        released = false;

    Rect playWhite = Rect(200, 400, 50, 50),
        playBlack = Rect(200, 475, 50, 50),
        exitGame = Rect(200, 550, 50, 50);


    TextSurface welcomes = TextSurface(
        "Chess!!!", "fonts/FreeSerif.ttf", 200, 255, 255, 255);

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


        if(mousex >= playWhite.x && mousex <= playWhite.x + playWhite.w)
        {
            if(mousey >= playWhite.y && mousey <= playWhite.y + playWhite.h)
            {
                return 0;
            }
        }

        if(mousex >= playBlack.x && mousex <= playBlack.x + playBlack.w)
        {
            if(mousey >= playBlack.y && mousey <= playBlack.y + playBlack.h)
            {
                return 1;
            }
        }

        if(mousex >= exitGame.x && mousex <= exitGame.x + exitGame.w)
        {
            if(mousey >= exitGame.y && mousey <= exitGame.y + exitGame.h)
            {
                return -1;
            }
        }
        


        
        sm.lock();
        sm.fill(GRAY);
        sm.put_text(welcomes, 100, 0);
        sm.put_text("Play as White", 275, 400, 0, 0, 0,
                   "fonts/FreeSerif.ttf", 24);
        sm.put_text("Play as Black", 275, 475, 0, 0, 0,
                   "fonts/FreeSerif.ttf", 24);
        sm.put_text("Exit Game", 275, 550, 0, 0, 0,
                   "fonts/FreeSerif.ttf", 24);
        sm.put_rect(playWhite, WHITE);
        sm.put_rect(playBlack, BLACK);
        sm.put_rect(exitGame, RED);
        sm.unlock();
        sm.flip();
        
        delay(10);
    }

    
    
    return ret;
}




