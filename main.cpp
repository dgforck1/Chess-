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
    int player = 0;
    //need some way to determine who is white and who is black
    // 0 = white, 1 = black

    Welcome();
    ChessMain(player);
    
    
    return 0;
}


int Welcome()
{
    int ret = 0;

    Surface s(MYWIDTH, MYHEIGHT);
    Event event;
    Mouse mouse;
    
    return ret;
}
