#include <iostream>
#include "Chess.h"
#include <string>
#include <iomanip>


bool test_piece();
bool test_board();
bool test_moving_pieces();

int main()
{
    //test cases
    std::cout << "Chess Test cases: " << std::endl << std::endl;

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

    if(test_moving_pieces())
    {
        std::cout << "Moving Pieces test  passed" << std::endl;
    }
    else
    {
        std::cout << "Moving Pieces test failed" << std::endl;
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


bool test_moving_pieces()
{
    bool pass = true;
    Board b = Board();    
    int workingPiece = b.getPieceIndex(1, 0); //get left most white pawn
    
    int destR = 2, destF = 0;

    std::cout << "    "
              << "Testing White's moves"
              << std::endl;


    
    //check moving left most white pawn one square
    if(b.checkMove(workingPiece, destR, destF)) //check that move is valid
    {
        int targetCapturePiece = b.getPieceIndex(destR, destF);

        if(targetCapturePiece == -1)
        {
            b.movePiece(workingPiece, destR, destF);
            /*b.updateBoard();
              b.printBoard();*/

            Piece p = b.getPiece(workingPiece);

            if(p.getRank() == destR && p.getFile() == destF)
            {
                std::cout << "        "
                          << "Moving pawn one square passed"
                          << std::endl;
            }
            else
            {
                std::cout << "        "
                          << "Moving pawn one square failed "
                          << "- dest rank and file"
                          << std::endl;
                
                pass = false;
            }
        }
        else
        {
            std::cout << "        "
                      << "Moving pawn one square failed "
                      << "- target capture piece"
                      << std::endl;
            
            pass = false;
        }        
    }
    else
    {
        std::cout << "        "
                  << "Moving pawn one square test failed"
                  << std::endl;
        
        pass = false;
    }


    workingPiece = b.getPieceIndex(1, 1); //get second white pawn    
    destR = 3;
    destF = 1;

    
    //check moving second white pawn two squares
    if(b.checkMove(workingPiece, destR, destF)) //check that move is valid
    {
        int targetCapturePiece = b.getPieceIndex(destR, destF);

        if(targetCapturePiece == -1)
        {
            b.movePiece(workingPiece, destR, destF);
            b.updateBoard();
            b.printBoard();

            Piece p = b.getPiece(workingPiece);

            if(p.getRank() == destR && p.getFile() == destF)
            {
                std::cout << "        "
                          << "Moving pawn two squares passed"
                          << std::endl;
            }
            else
            {
                std::cout << "        "
                          << "Moving pawn two squares failed "
                          << "- dest rank and file"
                          << std::endl;
                
                pass = false;
            }
        }
        else
        {
            std::cout << "        "
                      << "Moving pawn two squares failed "
                      << "- target capture piece"
                      << std::endl;
            
            pass = false;
        }        
    }
    else
    {
        std::cout << "        "
                  << "Moving pawn two squares test failed"
                  << std::endl;
        
        pass = false;
    }
    

    workingPiece = b.getPieceIndex(0, 0); //get first white rook
    
    destR = 1;
    destF = 0;

    
    //check moving white rook
    if(b.checkMove(workingPiece, destR, destF)) //check that move is valid
    {
        int targetCapturePiece = b.getPieceIndex(destR, destF);

        if(targetCapturePiece == -1)
        {
            b.movePiece(workingPiece, destR, destF);
            b.updateBoard();
            b.printBoard();

            Piece p = b.getPiece(workingPiece);

            if(p.getRank() == destR && p.getFile() == destF)
            {
                std::cout << "        "
                          << "Moving rook passed"
                          << std::endl;
            }
            else
            {
                std::cout << "        "
                          << "Moving rook failed "
                          << "- dest rank and file"
                          << std::endl;
                
                pass = false;
            }
        }
        else
        {
            std::cout << "        "
                      << "Moving rook failed "
                      << "- target capture piece"
                      << std::endl;
            
            pass = false;
        }        
    }
    else
    {
        std::cout << "        "
                  << "Moving rook failed"
                  << std::endl;
        
        pass = false;
    }


    workingPiece = b.getPieceIndex(0, 1); //get first white knight    
    destR = 2;
    destF = 2;

    
    //check moving white knight
    if(b.checkMove(workingPiece, destR, destF)) //check that move is valid
    {
        int targetCapturePiece = b.getPieceIndex(destR, destF);

        if(targetCapturePiece == -1)
        {
            b.movePiece(workingPiece, destR, destF);
            b.updateBoard();
            b.printBoard();

            Piece p = b.getPiece(workingPiece);

            if(p.getRank() == destR && p.getFile() == destF)
            {
                std::cout << "        "
                          << "Moving knight passed"
                          << std::endl;
            }
            else
            {
                std::cout << "        "
                          << "Moving knight failed "
                          << "- dest rank and file"
                          << std::endl;
                
                pass = false;
            }
        }
        else
        {
            std::cout << "        "
                      << "Moving knight failed "
                      << "- target capture piece"
                      << std::endl;
            
            pass = false;
        }        
    }
    else
    {
        std::cout << "    "
                  << "Moving knight failed"
                  << std::endl;
        
        pass = false;
    }









    

    return pass;
}



