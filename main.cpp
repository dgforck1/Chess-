#include <iostream>
#include "Chess.h"
#include <string>

bool test_piece();
bool test_board();

int main()
{
    //test cases
    std::cout << "Chess Test cases: " << std::endl;

    if(test_piece())
    {
        std::cout << "Piece Tests passed" << std::endl;
    }
    else
    {
        std::cout << "Piece Tests failed" << std::endl;
    }

    if(test_board())
    {
        std::cout << "Board Tests passed" << std::endl;
    }
    else
    {
        std::cout << "Board Tests failed" << std::endl;
    }
    
    return 0;
}


bool test_piece()
{
    bool pass = true;
    
    Piece *p = new Piece();
    std::cout << "Piece empty constructor passed" << std::endl;

    delete p;
    
    Piece p2(0, "P", 1, 2);
    std::cout << "Piece constructor passed" << std::endl;

    
    int test = p2.getRank();
    if(test == 1)
    {
        std::cout << "Piece getRank passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getRank failed" << std::endl;
        pass = false;
    }

    
    test = p2.getFile();
    if(test == 2)
    {
        std::cout << "Piece getFile passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getFile failed" << std::endl;
        pass = false;
    }

    
    test = p2.getPlayer();
    if(test == 0)
    {
        std::cout << "Piece getPlayer passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getPlayer failed" << std::endl;
        pass = false;
    }

    
    std::string stest = p2.getType();
    if(stest == "P")
    {
        std::cout << "Piece getType passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getType failed" << std::endl;
        pass = false;
    }

    return pass;
}

bool test_board()
{
    bool pass = true;

    Board *b = new Board();
    std::cout << "Board default constructor passed" << std::endl;
    
    return pass;
}
