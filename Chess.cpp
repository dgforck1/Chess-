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

bool Piece::checkMove(int destR, int destF) const
{
    //todo: implement actual checks
    return true;
}

void Piece::movePiece(int destR, int destF)
{    
    rank = destR;
    file = destF;
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


void Board::updateBoard()
{
    for(int i = 0; i < 8; i++)
    {
        for(int n = 0; n < 8; n++)
        {
            board[i][n] = '-';
        }
    }
    
    for(int i = 0; i < p.size(); i++)
    {
        board[ p[i].getRank() ][ p[i].getFile() ] = p[i].getType();
    }        
}


Piece& Board::getPiece(int r, int f)
{
    for(int i = 0; i < p.size(); i++)
    {
        int tempr = p[i].getRank();

        if(tempr == r)
        {
            int tempf = p[i].getFile();

            if(tempf == f)
            {
                return p[i];
            }
        }
    }

    //todo:  throw error if piece not found
}


int Board::getPieceIndex(int r, int f)
{
    for(int i = 0; i < p.size(); i++)
    {
        int tempr = p[i].getRank();

        if(tempr == r)
        {
            int tempf = p[i].getFile();

            if(tempf == f)
            {
                return i;
            }
        }
    }

    return -1; //didn't find the piece
}


bool Board::checkMove(int i, int destR, int destF) const
{        
    return p[i].checkMove(destR, destF);
}


void Board::movePiece(int i, int destR, int destF)
{
    p[i].movePiece(destR, destF);
}


std::string Board::capturePiece(int r, int f)
{
    int i = getPieceIndex(r, f);
    std::string ret = p[i].getType();
    p.erase(p.begin() + i);

    return ret;
}

void Board::printBoard() const
{
    //print board
    for(int i = 7; i >= 0; i--)
    {
        for(int n = 0; n < 8; n++)
        {
            std::cout << board[i][n];
        }
        
        std::cout << std::endl;
    }
}


int Board::getPieceSize() const
{
    return p.size();
}
