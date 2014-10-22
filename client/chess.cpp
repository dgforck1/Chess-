#include <iostream>
#include <string>
#include <cmath>

#include "chess.h"


///////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////

bool knightMove(int, int, int, int, std::string board[][8]);
bool rookMove(int, int, int, int, std::string board[][8]);
bool bishopMove(int, int, int, int, std::string board[][8]);
bool queenMove(int, int, int, int, std::string board[][8]);
bool kingMove(int, int, int, int);
bool putsKingInCheck(int, std::string board[][8], std::vector<Piece> p);
int findPiece(int, int, std::vector<Piece> p);


///////////////////////////////////////////////////////////////////////////////
// Piece Member Funcitons
///////////////////////////////////////////////////////////////////////////////

void Piece::print() const
{
    std::cout << "rank : " << rank
              << " file : " << file
              << " player : " << player
              << " moved : " << moved
              << " type : " << type
              << std::endl;
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

bool Piece::getMoved() const
{
    return moved;
}

std::string Piece::getType() const
{
    return type;
}

bool Piece::checkMove(int destR, int destF, std::string B[][8],
                      std::vector <Piece> p) const
{
    // check if the move is on the board
    if (destR < 0 || destR > 7 || destF < 0 || destF > 7)
    {
        // std::cout << "move outside bounds of board" << std::endl;
        return false;
    }
    // check if they actually moved the piece
    if (destR == rank && destF == file)
    {
        // std::cout << "you didnt move the piece to a new square"
        //           << std::endl; 
        return false;
    }

    // figure out if there is a piece at the target location and who's it is
    int targetPieceIndex = -1;
    int targetPiecePlayer = -1;
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getRank() == destR && p[i].getFile() == destF)
        {
            targetPieceIndex = i;
            targetPiecePlayer = p[i].getPlayer();
            break;
        }
    }

    // if there is a piece at the target location...
    if (targetPieceIndex > -1)
    {
        // if the piece at the target location isn't the player's...
        if (player != targetPiecePlayer)
        {
            // get a temporary board and list of pieces where the move was made
            // so we can see if that would put the player's king in check
            std::vector<Piece> t1 = p;
            t1.erase(t1.begin() + targetPieceIndex);
            t1[findPiece(rank, file, p)].movePiece(destR, destF);
            std::string t2 [8][8];
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    t2[j][i] = " ";
                }
            }
            for (int i = 0; i < p.size(); i++)
            {
                t2[t1[i].getRank()][t1[i].getFile()] = t1[i].getType();
            }

            // if the move doesn't put the player's king in check...
            if (!putsKingInCheck(player, t2, t1))
            {
                // if the piece is a rook see if it is a good move
                if (type == "R")
                {
                    return (rookMove(rank, file, destR, destF) ? true : false);
                }
                // if the piece is a knight see if it is a good move
                if (type == "N")
                {
                    return (knightMove(rank, file, destR, destF) ? true : false);
                }
                // if the piece is a bishop see if it is a good move
                if (type == "B")
                {
                    return (bishopMove(rank, file, destR, destF) ? true : false);
                }
                // if the piece is a queen see if it is a good move
                if (type == "Q")
                {
                    return (queenMove(rank, file, destR, destF) ? true : false);
                }
                // if the piece is a pawn see if it is a good move
                if (type == "P")
                {
                    // if player is white...
                    if (player == 0) 
                    {
                        // if pawn is in starting location doing
                        // the 2 space move, the location was occupied bad move
                        if (rank == 1 && destR == 3)
                        {return false;}
                        // if the pawn is only moving foward 1 space
                        else if (destR == rank + 1)
                        {
                            // the location was occupied, bad move
                            if (destF == file)
                                return false;
                            // the location was an enemy and the can move to
                            // take it
                            else
                                return true;
                        }
                        // every other weird, illegal move
                        else
                            return false;
                    }
                    // if the player is black
                    else
                    {
                        // if pawn is in starting location doing
                        // the 2 space move, the location was occupied bad move
                        if (rank == 6 && destR == 4)
                        {return false;}
                        // if the pawn is only moving foward 1 space
                        else if (destR == rank - 1)
                        {
                            // the location was occupied, bad move
                            if (destF == file)
                                return false;
                            // the location was an enemy and the can move to
                            // take it
                            else
                                return true;
                        }
                        // every other weird, illegal move
                        else
                            return false;
                    }
                }
            }
            
            //king stuff donw here
        }
        else
            return false;
    }
    else
    {}// en passant stuff down here
    
    ////////////////////////// still need that occupied function

    // determine if the occupied target square is friendly piece or enemy piece


    // if friendly return false
    // else ....
}

bool Piece::checkMove2(int destR, int destF, std::string b[][8],
                    std::vector<Piece> p) const
{return false;}

void Piece::movePiece(int destR, int destF)
{
    if (!moved)
    {moved = true;}
    rank = destR;
    file = destF;
}

///////////////////////////////////////////////////////////////////////////////
// Piece Non-Memeber Functions
///////////////////////////////////////////////////////////////////////////////


// bool pawnMoveOccupied(int r, int f, int nr, int nf)
// {
//     if (r - nr > 0) // up the board
//     {
//         if (r == 6 && nr == 4)////////////////////////////need and unoccupied function
//         {
//             // if occupied return false
//             return true;
//         }
//         else if (1 == std::abs(nr - r) && nf == f)// && unocupied
//         {
//             return true;
//         }
//     }
//     else // down the board
//     {
        
//     }
// }


bool knightMove(int r, int f, int nr, int nf)
{
    if (nr == r + 2)
    {
        if (nf == f + 1 || nf == f - 1)
        {
            return true;
        }
        else
            return false;
    }
    else if (nr == r - 2)
    {
        if (nf == f + 1 || nf == f - 1)
        {
            return true;
        }
        else
            return false;
    }
    else if (nr == r + 1)
    {
        if (nf == f + 2 || nf == f - 2)
        {
            return true;
        }
        else
            return false;
    }
    else if (nr == r - 1)
    {
        if (nf == f + 2 || nf == f - 2)
        {
            return true;
        }
        else
            return false;
    }
    return false;
}

bool rookMove(int r, int f, int nr, int nf, std::string board[][8])
{
    if (nr != r)
    {
        if (nf == f)
        {
            for (int i = r; i < nr; i++)
            {
                if (board[i][f] != " ")
                    return false;
            }
            return true;
        }
        return false;
    }
    else
    {
        if (nr == r)
        {
            for (int i = f; i < nf; i++)
            {
                if (board[r][i] != " ")
                    return false;
            }
            return true;
        }
        return false;
    }
}

bool bishopMove(int r, int f, int nr, int nf)
{
    if (abs((nr-r)/(nf-f)) == 1)/// split into 4 directions for purposes of the for loop stuff... also need do same on rookFUCK
    {
        for (int i = 0; i < nr - r; i++)
        return true;
    }
    return false;
}

bool queenMove(int r, int f, int nr, int nf)
{
    return (bishopMove(r, f, nr, nf) || rookMove(r, f, nr, nf));
}

bool kingMove(int r, int f, int nr, int nf)
{
    return (1 == std::abs(nr - r) || 1 == std::abs(nf - f));
}

// posititve diagonal == slope of 1
// negative daigonal == slope of -1
bool putsKingInCheck(int player, std::string board[][8], std::vector <Piece> p)
{
    // identify correct king piece and obtain its rank and file
    int kingRank = -1;
    int kingFile = -1;
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getPlayer() == player && p[i].getType() == "K")
        {
            kingRank = p[i].getRank();
            kingFile = p[i].getFile();
            break;
        }
    }
    
    // for loop running from 0 - 7 on rank with king fixed file
    for (int i = kingRank - 1; i > -1; i--)
    {
        if (board[i][kingFile] != " ")
        {
            int pindex = findPiece(i, kingFile, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
    }
    for (int i = kingRank + 1; i < 8; i++)
    {
        if (board[i][kingFile] != " ")
        {
            int pindex = findPiece(i, kingFile, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
    }
    
    // for loop running from 0 - 7 on file with king fixed rank
    for (int i = kingFile - 1; i > -1; i--)
    {
        if (board[kingRank][i] != " ")
        {
            int pindex = findPiece(kingRank, i, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
    }
    for (int i = kingFile + 1; i < 8; i++)
    {
        if (board[kingRank][i] != " ")
        {
            int pindex = findPiece(kingRank, i, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
    }
    // while loop running on positive diagonal in positive direction
    int tr = kingRank + 1;
    int ty = kingFile + 1;
    while(tr < 8 && ty < 8)
    {
        if (board[tr][ty] != " ")
        {
            int pindex = findPiece(tr, ty, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
        tr++;
        ty++;
    }
    // while loop running on positive diagonal in negative direction
    tr = kingRank - 1;
    ty = kingFile - 1;
    while(tr > -1 && ty > -1)
    {
        if (board[tr][ty] != " ")
        {
            int pindex = findPiece(tr, ty, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
        tr--;
        ty--;
    }
    // while loop running on negative diagonal in positive direction
    tr = kingRank + 1;
    ty = kingFile - 1;
    while(tr < 8 && ty > -1)
    {
        if (board[tr][ty] != " ")
        {
            int pindex = findPiece(tr, ty, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
        tr++;
        ty--;
    }
    // while loop running on negative diagonal in negative direction
    tr = kingRank - 1;
    ty = kingFile + 1;
    while(tr > -1 && ty < 8)
    {
        if (board[tr][ty] != " ")
        {
            int pindex = findPiece(tr, ty, p);
            if (p[pindex].getPlayer() == player)
                break;
            else
            {
                if (p[pindex].checkMove2(kingRank, kingFile, board, p))
                    return true;
            }
        }
        tr--;
        ty++;
    }
    // check the 8 possible knight possitions
    bool flag = false;
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getType() == "N" && p[i].getPlayer() != player)
        {
            int r = p[i].getRank();
            int f = p[i].getFile();
            if (r == kingRank + 2 &&
               (f == kingFile + 1 || f == kingFile - 1))
                return true;
            if (r == kingRank - 2 &&
               (f == kingFile + 1 || f == kingFile - 1))
                return true;
            if (f == kingFile + 2 &&
               (r == kingRank + 1 || r == kingRank - 1))
                return true;
            if (f == kingFile - 2 &&
               (r == kingRank + 1 || r == kingRank - 1))
                return true;
            if (flag)
                break;
            flag = true;
        }
    }
    return false;
}

int findPiece (int r, int f, std::vector<Piece> p)
{
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getRank() == r && p[i].getFile() == f)
            return i;
    }
    return -1;
}


int main()
{
    return 0;
}
