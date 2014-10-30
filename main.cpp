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


//prototypes
int Welcome();
int Make();
int Join();
void Watch();
int Load();
bool MainRectClicked(int mX, int mY, Rect& r);
void getSaves(std::vector<std::string> &s);




int main()
{
    int choice = -1;
    int player = 0;

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
                choice = Make();                
                break;
            case 2: //join game
                choice = Join();
                break;
            case 3: //watch game
                break;
            case 4: //load game
                choice = Load();
                break;
            default:
                break;
        }

        if(choice == 0)
        {
            play = false;
        }
    }

    
    return 0;
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


int Make()
{
    int choice = -1;
    choice = ChessMain(0);
    return choice;
}


int Join()
{
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
            if(MainRectClicked(mousex, mousey, cancelR))
            {
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



