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


bool Piece::checkMove(int destR, int destF, std::string B[][8],
                      std::vector < Piece > p) const
{

    //invalid move, outside scope of board
    if(destR < 0 || destR >= 8 || destF < 0 || destF >= 8)
    {
        return false;
    }
    
    int targetPieceIndex = -1; //index of the piece that is at the dest square
    
    for(int i = 0; i < p.size(); i++)
    {
        int tempr = p[i].getRank();

        if(tempr == destR)
        {
            int tempf = p[i].getFile();

            if(tempf == destF)
            {
                targetPieceIndex = i;
            }
        }
    }

    int targetPiecePlayer = -1;

    if(targetPieceIndex >= 0)
    {
        targetPiecePlayer = p[targetPieceIndex].getPlayer();
    }
    
    if(player == 0) //white
    {
        if(type == "P") //white pawn
        {
            //moves that can be made at any time:

            //move forward one square
            if(destF == file)
            {
                if(destR == rank + 1)
                {
                    if(targetPieceIndex == -1)
                    {
                        return true;
                    }
                }
            }

            
            //capture diagonal left or right
            if(destF == file - 1 || destF == file + 1)
            {
                if(destR == rank + 1)
                {
                    if(targetPieceIndex != -1)
                    {
                        if(targetPiecePlayer == 1)
                        {
                            return true;
                        }
                    }
                }
            }
                        

            //moves that can only be made from home square

            //double jump
            if(!moved)
            {
                if(destF == file)
                {
                    if(destR == rank + 2)
                    {
                        if(targetPieceIndex == -1)
                        {
                            return true;
                        }
                    }
                }
            }

            //special moves

            //en passant
            

            return false; //if not returned true by now, return false
        }
        else if(type == "R") //white rook
        {
            //horizontal or vertical move
            if(destR == rank || destF == file) 
            {
                if(targetPieceIndex == -1) //empty square
                {
                    return true;
                }
                else if (targetPieceIndex != -1)
                {
                    if(targetPiecePlayer == 1)
                    {
                        return true;
                    }
                }                
            }                     
        }
        else if(type == "N") //white knight
        {
            //vertical two squares
            if(destR == rank + 2 || destR == rank - 2)
            {
                if(destF == file + 1 || destF == file - 1)
                {
                    if(targetPieceIndex == -1) //empty square
                    {
                        return true;
                    }
                    else if (targetPieceIndex != -1)
                    {
                        if(targetPiecePlayer == 1)
                        {
                            return true;
                        }
                    }   
                }
            }


            //vertical one square
            if(destR == rank + 1 || destR == rank - 1)
            {
                if(destF == file + 2 || destF == file - 2)
                {
                    if(targetPieceIndex == -1) //empty square
                    {
                        return true;
                    }
                    else if (targetPieceIndex != -1)
                    {
                        if(targetPiecePlayer == 1)
                        {
                            return true;
                        }
                    }   
                }
            }
        }
        else if(type == "B") //white bishop
        {
            //diagonal move
            if(((destR - rank) * 1.0) / ((destF - file) * 1.0) == 1
               || ((destR - rank) * 1.0) / ((destF - file) * 1.0) == -1)
            {
                if(targetPieceIndex == -1) //empty square
                {
                    return true;
                }
                else if (targetPieceIndex != -1)
                {
                    if(targetPiecePlayer == 1)
                    {
                        return true;
                    }
                }
            }
            
            
        }
        else if(type == "Q") //white queen
        {
            //horizontal or vertical move
            if(destR == rank || destF == file) 
            {
                if(targetPieceIndex == -1) //empty square
                {
                    return true;
                }
                else if (targetPieceIndex != -1)
                {
                    if(targetPiecePlayer == 1)
                    {
                        return true;
                    }
                }                
            }

            //diagonal move
            if(((destR - rank) * 1.0) / ((destF - file) * 1.0) == 1
               || ((destR - rank) * 1.0) / ((destF - file) * 1.0) == -1)
            {
                if(targetPieceIndex == -1) //empty square
                {
                    return true;
                }
                else if (targetPieceIndex != -1)
                {
                    if(targetPiecePlayer == 1)
                    {
                        return true;
                    }
                }
            }
        }
        else if(type == "K") //white king
        {
            if(destR - rank >= -1 && destR - rank <= 1)
            {
                if(destF - file >= -1 && destF - file <= 1)
                {                    
                    if(targetPieceIndex == -1) //empty square
                    {
                        return true;
                    }
                    else if (targetPieceIndex != -1)
                    {
                        if(targetPiecePlayer == 1)
                        {
                            return true;
                        }
                    }                
                }
            }
                
            
            //castle
            if(!moved)
            {
                if(destR == rank)
                {
                    if(destF == file - 2) //queen side castle
                    {
                        
                    }

                    if(destF == file + 2) //kind side castle
                    {
                    }
                }
            }
        }
    }
    else if(player == 1) //black
    {
    }
    
    return false;
}


void Piece::movePiece(int destR, int destF, std::string B[][8])
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


bool Board::checkMove(int i, int destR, int destF)
{
    bool ret = p[i].checkMove(destR, destF, board, p);

    return ret;
}


void Board::movePiece(int i, int destR, int destF)
{


    
    //p[i].movePiece(destR, destF);
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
