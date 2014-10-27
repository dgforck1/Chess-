#include <iostream>
#include "Chess.h"
#include <string>
#include "Includes.h"


//piece implementation
void Piece::print() const
{
        std::cout << "player: " << player
              << " rank: " << rank
              << " file: " << file
              << " type: " << type
              << " moved: " << moved;
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

    if(destR == rank && destF == file) //same square as piece
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
                        int targetRookIndex = -1;

                        //find the rook
                        for(int i = 0; i < p.size(); i++)
                        {
                            int tempr = p[i].getRank();
                            
                            if(tempr == 0)
                            {
                                int tempf = p[i].getFile();
                                
                                if(tempf == 0)
                                {
                                    targetRookIndex = i;
                                }
                            }
                        }

                        if(!p[targetRookIndex].getMoved())
                        {
                            //ensure nothing between K and R
                            int a = -1;
                            
                            for(int i = 0; i < p.size(); i++)
                            {
                                int tempr = p[i].getRank();
                                
                                if(tempr == 0)
                                {
                                    int tempf = p[i].getFile();
                                    
                                    if(tempf == 3)
                                    {
                                        a = i;
                                    }
                                }
                            }
                            
                            if(a == -1)
                            {
                                int b = -1;
                                
                                for(int i = 0; i < p.size(); i++)
                                {
                                    int tempr = p[i].getRank();
                                    
                                    if(tempr == 0)
                                    {
                                        int tempf = p[i].getFile();
                                        
                                        if(tempf == 1)
                                        {
                                            b = i;
                                        }
                                    }
                                }
                                
                                if(b == -1)
                                {                            
                                    return true;
                                }       
                            }
                        }
                    }

                    if(destF == file + 2) //king side castle
                    {
                        int targetRookIndex = -1;

                        //find the rook                        
                        for(int i = 0; i < p.size(); i++)
                        {
                            int tempr = p[i].getRank();
                            
                            if(tempr == 0)
                            {
                                int tempf = p[i].getFile();
                                
                                if(tempf == 7)
                                {
                                    targetRookIndex = i;
                                }
                            }
                        }

                        if(!p[targetRookIndex].getMoved())
                        {                            
                            //ensure nothing between K and R
                            int a = -1;

                            for(int i = 0; i < p.size(); i++)
                            {
                                int tempr = p[i].getRank();
                                
                                if(tempr == 0)
                                {
                                    int tempf = p[i].getFile();
                                    
                                    if(tempf == 5)
                                    {
                                        a = i;
                                    }
                                }
                            }
                            
                            if(a == -1)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    else if(player == 1) //black
    {
        if(type == "P") //black pawn
        {
            //moves that can be made at any time:

            //move forward one square
            if(destF == file)
            {
                if(destR == rank - 1)
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
                if(destR == rank - 1)
                {
                    if(targetPieceIndex != -1)
                    {
                        if(targetPiecePlayer == 0)
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
                    if(destR == rank - 2)
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
        else if(type == "R") //black rook
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
                    if(targetPiecePlayer == 0)
                    {
                        return true;
                    }
                }                
            }                     
        }
        else if(type == "N") //black knight
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
                        if(targetPiecePlayer == 0)
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
                        if(targetPiecePlayer == 0)
                        {
                            return true;
                        }
                    }   
                }
            }
        }
        else if(type == "B") //black bishop
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
                    if(targetPiecePlayer == 0)
                    {
                        return true;
                    }
                }
            }                        
        }
        else if(type == "Q") //black queen
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
                    if(targetPiecePlayer == 0)
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
                    if(targetPiecePlayer == 0)
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
                        if(targetPiecePlayer == 0)
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
                        int targetRookIndex = -1;

                        //find the rook
                        for(int i = 0; i < p.size(); i++)
                        {
                            int tempr = p[i].getRank();
                            
                            if(tempr == 7)
                            {
                                int tempf = p[i].getFile();
                                
                                if(tempf == 0)
                                {
                                    targetRookIndex = i;
                                }
                            }
                        }

                        if(!p[targetRookIndex].getMoved())
                        {
                            //ensure nothing between K and R
                            int a = -1;
                            
                            for(int i = 0; i < p.size(); i++)
                            {
                                int tempr = p[i].getRank();
                                
                                if(tempr == 7)
                                {
                                    int tempf = p[i].getFile();
                                    
                                    if(tempf == 3)
                                    {
                                        a = i;
                                    }
                                }
                            }
                            
                            if(a == -1)
                            {
                                int b = -1;
                                
                                for(int i = 0; i < p.size(); i++)
                                {
                                    int tempr = p[i].getRank();
                                    
                                    if(tempr == 7)
                                    {
                                        int tempf = p[i].getFile();
                                        
                                        if(tempf == 1)
                                        {
                                            b = i;
                                        }
                                    }
                                }
                                
                                if(b == -1)
                                {                            
                                    return true;
                                }       
                            }
                        }
                    }

                    if(destF == file + 2) //king side castle
                    {
                        int targetRookIndex = -1;

                        //find the rook                        
                        for(int i = 0; i < p.size(); i++)
                        {
                            int tempr = p[i].getRank();
                            
                            if(tempr == 7)
                            {
                                int tempf = p[i].getFile();
                                
                                if(tempf == 7)
                                {
                                    targetRookIndex = i;
                                }
                            }
                        }

                        if(!p[targetRookIndex].getMoved())
                        {                            
                            //ensure nothing between K and R
                            int a = -1;

                            for(int i = 0; i < p.size(); i++)
                            {
                                int tempr = p[i].getRank();
                                
                                if(tempr == 7)
                                {
                                    int tempf = p[i].getFile();
                                    
                                    if(tempf == 5)
                                    {
                                        a = i;
                                    }
                                }
                            }
                            
                            if(a == -1)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }        
    }
    
    return false;
}


bool Piece::getMoved() const
{
    return moved;
}


void Piece::movePiece(int destR, int destF)
{
    if(!moved)
    {
        moved = true;
    }
    
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


Piece Board::getPiece(int r, int f)
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

//todo: try removing reference, so copy the piece instead
Piece Board::getPiece(int i)
{
    if(i < p.size() && i >= 0)
    {
        return p[i];
    }
    else
    {
        //todo:  throw error if piece not found
    }
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
    if(destR >=0 && destR < 8 && destF >= 0 && destF < 8)
    {
        p[i].movePiece(destR, destF);
    }
    else
    {
        //todo: throw out of bounds error
    }
}


std::string Board::capturePiece(int r, int f)
{    
    int i = getPieceIndex(r, f);

    if(i >= 0)
    {
        std::string ret = p[i].getType();
        p.erase(p.begin() + i);
        
        return ret;
    }
    else
    {
        return "";
    }
}


std::string Board::capturePiece(int i)
{    
    //p[i].print();    
    //std::cout << std::endl;       

    //why are you broken?!?!?!?!?
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


Image *DrawPiece::getImage()
{
    return i;
}


Rect &DrawPiece::getRect()
{
    return r;
}
