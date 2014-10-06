#include <iostream>
#include "Chess.h"
#include <string>

//piece implementation
void Piece::print() const
{
    std::cout << rank << " " << file << " " << player << " "
              << type << std::endl;
}

int Piece::getRank() const
{
    return rank;
}

int Piece::getFile() const
{
    return file;
}

int Piece::getPlayer() const
{
    return player;
}

std::string Piece::getType() const
{
    return type;
}


//board implementation

void Board::initializePieces()
{
    for(int i = 0; i < 8; i++)
    {
        p.push_back(Piece(0, "P", 1, i)); //white's pawns
        p.push_back(Piece(1, "P", 6, i)); //black's pawns
    }

    //white's pieces

    p.push_back(Piece(0, "K", 0, 4)); //king
    p.push_back(Piece(0, "Q", 0, 3)); //queen
    p.push_back(Piece(0, "R", 0, 7)); //king's rook
    p.push_back(Piece(0, "R", 0, 0)); //queen's rook
    p.push_back(Piece(0, "N", 0, 6)); //king's knight
    p.push_back(Piece(0, "N", 0, 1)); //queen's knight
    p.push_back(Piece(0, "B", 0, 2)); //king's bishop
    p.push_back(Piece(0, "B", 0, 5)); //queen's bishop
    
    //black's pieces

    p.push_back(Piece(1, "K", 7, 4)); //king
    p.push_back(Piece(1, "Q", 7, 3)); //queen
    p.push_back(Piece(1, "R", 7, 7)); //king's rook
    p.push_back(Piece(1, "R", 7, 0)); //queen's rook
    p.push_back(Piece(1, "N", 7, 6)); //king's knight
    p.push_back(Piece(1, "N", 7, 1)); //queen's knight
    p.push_back(Piece(1, "B", 7, 2)); //king's bishop
    p.push_back(Piece(1, "B", 7, 5)); //queen's bishop
}


void Board::initializeBoard()
{
    for(int i = 0; i < 8; i++)
    {
        for(int n = 0; n < 8; n++)
        {
            board[i][n] = '-';
        }
    }


}





