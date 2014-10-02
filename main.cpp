#include <iostream>
#include "Chess.h"
#include <string>

bool test_piece();

int main()
{
    //test cases
    std::cout << "Chess Test cases: " << std::endl;
    test_piece();
    
    return 0;
}


bool test_piece()
{
    bool pass = true;
    Piece p();
    std::cout << "Piece empty constructor passed" << std::endl;

    
    Piece p2(0, 1, 2, "P");
    std::cout << "Piece constructor passed" << std::endl;

    
    int test = p2.getRank();
    if(test == 0)
    {
        std::cout << "Piece getRank passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getRank failed" << std::endl;
    }

    
    test = p2.getFile();
    if(test == 1)
    {
        std::cout << "Piece getFile passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getFile failed" << std::endl;
    }

    
    test = p2.getPlayer();
    if(test == 2)
    {
        std::cout << "Piece getPlayer passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getPlayer failed" << std::endl;
    }

    
    std::string stest = p2.getType();
    if(stest == "P")
    {
        std::cout << "Piece getType passed" << std::endl;
    }
    else
    {
        std::cout << "Piece getType failed" << std::endl;
    }

    return pass;
}
