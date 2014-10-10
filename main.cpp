#include <iostream>
#include "Chess.h"
#include <string>
#include <iomanip>


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
    std::cout << "    "
              << "Piece empty constructor passed" << std::endl;

    delete p;
    
    Piece p2(0, "P", 1, 2);
    std::cout << "    "
              << "Piece constructor passed" << std::endl;

    
    int testr = p2.getRank();
    if(testr == 1)
    {
        std::cout << "    "
                  << "Piece getRank passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Piece getRank failed" << std::endl;
        pass = false;
    }

    
    int testf = p2.getFile();
    if(testf == 2)
    {
        std::cout << "    "
                  << "Piece getFile passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Piece getFile failed" << std::endl;
        pass = false;
    }

    
    int testp = p2.getPlayer();
    if(testp == 0)
    {
        std::cout << "    "
                  << "Piece getPlayer passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Piece getPlayer failed" << std::endl;
        pass = false;
    }

    
    std::string stest = p2.getType();
    if(stest == "P")
    {
        std::cout << "    "
                  << "Piece getType passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Piece getType failed" << std::endl;
        pass = false;
    }

    
    p2.movePiece(0, 0);
    testr = p2.getRank();
    testf = p2.getFile();

    if(testr == 0 && testf == 0)
    {
        std::cout << "    "
                  << "Piece movePiece passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Piece movePiece failed" << std::endl;
        pass = false;
    }
    

    return pass;
}

bool test_board()
{
    bool pass = true;

     
    Board *b = new Board();
    std::cout << "    "
              << "Board default constructor passed" << std::endl;

    
    Piece p = b->getPiece(0, 0);
    std::string test = p.getType();

    if(test == "R")
    {
        std::cout << "    "
                  << "Board getPiece function passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Board getPiece function failed" << std::endl;
        pass = false;
    }

    
    int i = b->getPieceIndex(0, 0);    

    if(i == 19)
    {
        std::cout << "    "
                  << "Board getPieceIndex function passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Board getPieceIndex function failed" << std::endl;
        pass = false;
    }

    
    bool checkMove = b->checkMove(i, 0, 0);

    if(checkMove)
    {
        std::cout << "    "
                  << "Board checkMove function passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Board checkMove function failed" << std::endl;
        pass = false;
    }

    
    b->movePiece(i, 0, 0);
    
    if(p.getRank() == 0 && p.getFile() == 0)
    {
        std::cout << "    "
                  << "Board movePiece function passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Board movePiece function failed" << std::endl;
        pass = false;
    }

    
    int size = b->getPieceSize();
    std::string stest = b->capturePiece(0, 0);

    if(b->getPieceSize() == size - 1 && stest == "R")
    {
        std::cout << "    "
                  << "Board capturePiece function passed" << std::endl;
    }
    else
    {
        std::cout << "    "
                  << "Board capturePiece function failed" << std::endl;
        pass = false;
    }
    
    
    
    return pass;
}
