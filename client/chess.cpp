#include <iostream>
#include <string>
#include <cmath>

#include "chess.h"


// TO DO:
//  en passant
//  promotion -- think I got this done... modified the piece move function


///////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////

bool knightMove(int, int, int, int);
bool rookMove(int, int, int, int, std::string board[][8]);
bool bishopMove(int, int, int, int, std::string board[][8]);
bool queenMove(int, int, int, int, std::string board[][8]);
bool kingMove(int, int, int, int, std::string board[][8],
              std::vector <Piece> p);
bool putsKingInCheck(int, std::string board[][8], std::vector<Piece> p);
int findPiece(int, int, std::vector<Piece> p);
std::vector<int> findThreats(int, std::string board[][8],
                             std::vector <Piece> p);

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
//     std::cout << std::endl << *this << std::endl;
//     std::cout << "in Piece::checkMove " << (player ? "Black" : "White") << std::endl;
//     std::cout << "move on board?" << std::endl;
    
    // check if the move is on the board
    if (destR < 0 || destR > 7 || destF < 0 || destF > 7)
    {
        
//         std::cout << "\tmove outside bounds of board" << std::endl;

        return false;
    }
    
//     std::cout << "\tmove on the board" << std::endl;
//     std::cout << "move off origional square?" << std::endl;
    
    // check if they actually moved the piece
    if (destR == rank && destF == file)
    {
        
//         std::cout << "\tyou didnt move the piece to a new square"
//                   << std::endl; 

        return false;
    }

//     std::cout << "\tmove is off origional square" << std::endl;
//     std::cout << "piece at target location?" << std::endl;
    
    // figure out if there is a piece at the target location and who's it is
    int targetPieceIndex = -1;
    int targetPiecePlayer = -1;
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getRank() == destR && p[i].getFile() == destF)
        {
            targetPieceIndex = i;
            targetPiecePlayer = p[i].getPlayer();
//             std::cout << "\tyes : " << p[i] << std::endl;
            break;
        }
    }
    
    // if there is a piece at the target location...
    if (targetPieceIndex > -1)
    {
        
//         std::cout << "\nin the \"there is a target piece\" location" << std::endl;
//         std::cout << "piece friendly?" << std::endl;

        // if the piece at the target location isn't the player's...
        if (player != targetPiecePlayer)
        {
            
//             std::cout << "\tthe target is not a friendly piece" << std::endl;

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
                    return (rookMove(rank, file, destR, destF, B) ? true : false);
                }
                // if the piece is a knight see if it is a good move
                if (type == "N")
                {
                    return (knightMove(rank, file, destR, destF) ? true : false);
                }
                // if the piece is a bishop see if it is a good move
                if (type == "B")
                {
                    return (bishopMove(rank, file, destR, destF, B) ? true : false);
                }
                // if the piece is a queen see if it is a good move
                if (type == "Q")
                {
                    return (queenMove(rank, file, destR, destF, B) ? true : false);
                }
                // if the piece if a king see if ti is a good move
                if (type == "K")
                {
                    return (kingMove(rank, file, destR, destF, B, p) ? true : false);
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
        }
        else
            return false;
    }
    else// en passant stuff down here
    {
//        std::cout << "in the \"no target piece section\"" << std::endl;
        // get a temporary board and list of pieces where the move was made
        // so we can see if that would put the player's king in check
        std::vector<Piece> t1 = p;
//        std::cout << "f" << std::endl;
        t1[findPiece(rank, file, p)].movePiece(destR, destF);
//        std::cout << "uc" << std::endl;
        std::string t2 [8][8];
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                t2[j][i] = " ";
            }
        }
//        std::cout << "k" << std::endl;
        for (int i = 0; i < p.size(); i++)
        {
            t2[t1[i].getRank()][t1[i].getFile()] = t1[i].getType();
        }
//        std::cout << "getting ready to check for check" << std::endl;
        // if the move doesn't put the player's king in check...
        if (!putsKingInCheck(player, t2, t1))
        {
            // if the piece is a rook see if it is a good move
            if (type == "R")
            {
                return (rookMove(rank, file, destR, destF, B) ? true : false);
            }
            // if the piece is a knight see if it is a good move
            if (type == "N")
            {
                return (knightMove(rank, file, destR, destF) ? true : false);
            }
            // if the piece is a bishop see if it is a good move
            if (type == "B")
            {
                return (bishopMove(rank, file, destR, destF, B) ? true : false);
            }
            // if the piece is a queen see if it is a good move
            if (type == "Q")
            {
                return (queenMove(rank, file, destR, destF, B) ? true : false);
            }
            // if the piece if a king see if ti is a good move
            if (type == "K")
            {
                return (kingMove(rank, file, destR, destF, B, p) ? true : false);
            }
                // if the piece is a pawn see if it is a good move
            if (type == "P")
            {
                // if player is white...
                if (player == 0) 
                {
                    // if pawn is in starting location doing
                    // the 2 space move
                    if (rank == 1 && destR == 3)
                    {return true;}
                    // if the pawn is only moving foward 1 space
                    else if (destR == rank + 1)
                    {
                        if (destF == file)
                            return true;
                        // the location has no enemy, the pawn cannot
                        // move to capture
                        else
                            return false; // en passsant stuff here
                    }
                    // every other weird, illegal move
                    else
                        return false;
                }
                // if the player is black
                else
                {
                    // if pawn is in starting location doing
                    // the 2 space move
                    if (rank == 6 && destR == 4)
                    {return true;}
                    // if the pawn is only moving foward 1 space
                    else if (destR == rank - 1)
                    {
                        if (destF == file)
                            return true;
                        // the location has no enemy, the pawn cannot
                        // move to capture
                        else
                            return false;// en passant stuff here
                    }
                    // every other weird, illegal move
                    else
                        return false;
                }
            }
        }
    }
    return false;
}

bool Piece::checkMove2(int destR, int destF, std::string b[][8],
                    std::vector<Piece> p) const
{
    if (type == "K")
    {return false;}
    else if (type == "Q")
    {return (queenMove(rank, file, destR, destF, b));}
    else if (type == "R")
    {return (rookMove(rank, file, destR, destF, b));}
    else if (type == "B")
    {return (bishopMove(rank, file, destR, destF, b));}
    else if (type == "N")
    {return (knightMove(rank, file, destR, destF));}
    else if (type == "P")
    {
        if (player == 0 && rank + 1 == destR && (file + 1 == destF || file - 1 == destF))
        {return true;}
        else if (player == 1 && rank - 1 == destR && (file + 1 == destF || file - 1 == destF))
        {return true;}
        return false;
    }
}

void Piece::movePiece(int destR, int destF)
{
    if (!moved)
    {moved = true;}
    if (type == "P" && ((player == 0 && destR == 7) ||
                        (player == 1 && destR == 0)))
    {type = "Q";}
    rank = destR;
    file = destF;
}


bool Piece::operator==(const Piece & p)const
{
    return (rank == p.getRank() && file == p.getFile()
            && player == p.getPlayer() && moved == p.getMoved()
            && type == p.getType());
}

///////////////////////////////////////////////////////////////////////////////
// Piece Non-Memeber Functions
///////////////////////////////////////////////////////////////////////////////


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
            if (nr > r)
            {
                for (int i = r + 1; i < nr; i++)
                {
                    if (board[i][f] != " ")
                        return false;
                }
            }
            else
            {
                for (int i = r + 1; i > nr; i--)
                {
                    if (board[i][f] != " ")
                        return false;
                }
            }
            return true;
        }
        return false;
    }
    else
    {
        if (nr == r)
        {
            if (nf > f)
            {
                for (int i = f + 1; i < nf; i++)
                {
                    if (board[r][i] != " ")
                        return false;
                }
            }
            else
            {
                for (int i = f + 1; i > nf; i--)
                {
                    if (board[r][i] != " ")
                        return false;
                }
            }
            return true;
        }
        return false;
    }
}

bool bishopMove(int r, int f, int nr, int nf, std::string board[][8])
{
    if (nr - r == 1)
    {
        if (nf - f == 1)
        {
            for (int i = 1; i < nr - r; i++)
            {
                if (board[r + i][f + i] == " ")
                    return false;
            }
        }
        else
        {
            for (int i = 1; i < nr - r; i++)
            {
                if (board[r + i][f - i] == " ")
                    return false;
            }
        }
        return true;
    }
    else if (nr - r == -1)
    {
        if (nf - f == 1)
        {
            for (int i = 1; i < r - nr; i++)
            {
                if (board[r - i][f + i] == " ")
                    return false;
            }
        }
        else
        {
            for (int i = 1; i < r - nr; i++)
            {
                if (board[r - i][f - i] == " ")
                    return false;
            }
        }
        return true;
    }
    return false;
}

bool queenMove(int r, int f, int nr, int nf, std::string board[][8])
{
    return (bishopMove(r, f, nr, nf, board) || rookMove(r, f, nr, nf, board));
}

bool kingMove(int r, int f, int nr, int nf, std::string board[][8],
              std::vector <Piece> p)
{
    if (1 == std::abs(nr - r) || 1 == std::abs(nf - f))
    {
        return true;
    }
    else if (nf == f + 2 && nr == r)
    {
        // get kings moved value and pertint rooks move value
        int k = findPiece(r, f, p);
        if (p[k].getMoved() == false && p[k].getType() == "K")
        {
            int r = findPiece(r, f + 3, p);
            if (p[r].getMoved() == false && p[r].getType() == "R")
            {
                // verify that interving spaces are empty
                for (int i = f + 1; i < f + 3; i++)
                {
                    if (board[r][i] != " ")
                        return false;
                }
                // check king in check in current position
                // get a temporary board and list of pieces where the move was made
                // so we can see if that would put the player's king in check
                std::vector<Piece> t1 = p;
                for (int h = 0; h < 3; h++)
                {
                    int tempr = t1[k].getRank();
                    int tempf = t1[k].getFile();
                    t1[findPiece(tempr, tempf, p)].movePiece(r, f + h);
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
                    if (putsKingInCheck(t1[k].getPlayer(), t2, t1))
                    {return false;}
                }
                return true;
            }
            return false;
        }
    }
    else if (nf == f - 2 && nr == r)
    {
        // get kings moved value and pertint rooks move value
        int k = findPiece(r, f, p);
        if (p[k].getMoved() == false && p[k].getType() == "K")
        {
            int r = findPiece(r, f - 4, p);
            if (p[r].getMoved() == false && p[r].getType() == "R")
            {
                // verify that interving spaces are empty
                for (int i = f - 1; i > -1; i++)
                {
                    if (board[r][i] != " ")
                        return false;
                }
                // check king in check in current position
                // get a temporary board and list of pieces where the move was made
                // so we can see if that would put the player's king in check
                std::vector<Piece> t1 = p;
                for (int h = 0; h < 3; h++)
                {
                    int tempr = t1[k].getRank();
                    int tempf = t1[k].getFile();
                    t1[findPiece(tempr, tempf, p)].movePiece(r, f - h);
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
                    if (putsKingInCheck(t1[k].getPlayer(), t2, t1))
                    {return false;}
                }
                return true;
            }
            return false;
        }
    }
    return false;
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

///////////////////////////////////////////////////////////////////////////////
// Board Member Functions
///////////////////////////////////////////////////////////////////////////////

void Board::initalizePieces()
{
    for (int i = 0; i < 8; i++)
    {p.push_back(Piece(1, i, 0, "P"));}

    p.push_back(Piece(0, 0, 0, "R"));
    p.push_back(Piece(0, 1, 0, "N"));
    p.push_back(Piece(0, 2, 0, "B"));
    p.push_back(Piece(0, 3, 0, "Q"));
    p.push_back(Piece(0, 4, 0, "K"));
    p.push_back(Piece(0, 5, 0, "B"));
    p.push_back(Piece(0, 6, 0, "N"));
    p.push_back(Piece(0, 7, 0, "R"));
    
    for (int i = 0; i < 8; i++)
    {p.push_back(Piece(6, i, 1, "P"));}

    p.push_back(Piece(7, 0, 1, "R"));
    p.push_back(Piece(7, 1, 1, "N"));
    p.push_back(Piece(7, 2, 1, "B"));
    p.push_back(Piece(7, 3, 1, "Q"));
    p.push_back(Piece(7, 4, 1, "K"));
    p.push_back(Piece(7, 5, 1, "B"));
    p.push_back(Piece(7, 6, 1, "N"));
    p.push_back(Piece(7, 7, 1, "R"));    
}
void Board::updateBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[j][i] = " ";
        }
    }
    for (int i = 0; i < p.size(); i++)
    {
        board[p[i].getRank()][p[i].getFile()] = p[i].getType();
    }        
}
Piece Board::getPiece(int r, int f)
{
    int index = findPiece(r, f, p);
    return p[index];
}
Piece Board::getPiece(int i)
{
    return p[i];
}
int Board::getPieceIndex(int r, int f)
{
    return findPiece(r, f, p);
}
bool Board::checkMove(int i, int destR, int destF)//, std::string & prev)
{
    return p[i].checkMove(destR, destF, board, p);
}
void Board::movePiece(int i, int destR, int destF)
{
    p[i].movePiece(destR, destF);
}
std::string Board::capturePiece(int r, int f)
{
    int i = getPieceIndex(r, f);
    if (i > -1)
    {
        std::string ret = p[i].getType();
        p.erase(p.begin() + i);
        return ret;
    }
}
std::string Board::capturePiece(int i)
{
    std::string ret = p[i].getType();
    p.erase(p.begin() + i);
    return ret;
}

void Board::printBoard() const
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}

int Board::getPieceSize() const
{
    return p.size();
}

std::string Board::getSquare(int r, int f) const
{
    return board[r][f];
}
    
bool Board::checkmate(int player)// player is the person checking their enemy for mate
{
    std::vector<Piece> t1;// mater
    std::vector<Piece> t2;// matee
    std::vector<int> t2i;// t2 piece index in p
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getPlayer() == player)
        {
            t2.push_back(p[i]);
            t2i.push_back(i);
        }
        else
            t1.push_back(p[i]);
    }
    bool inCheck = putsKingInCheck(player, board, p);

    if (inCheck)
    {
        int kingRank, kingFile, kingi;
        for (int i = 0; i < t2.size(); i++)
        {
            if (t2[i].getType() == "K")
            {
                kingRank = t2[i].getRank();
                kingFile = t2[i].getFile();
                kingi = i;
                break;
            }
        }
        // find dodge
        for (int i = 0; i < 8; i++)
        {
            int destR, destF;
            switch(i)
            {
                case 0: destR = kingRank; destF = kingFile + 1; break;
                case 1: destR = kingRank; destF = kingFile - 1; break;
                case 2: destR = kingRank + 1; destF = kingFile + 1; break;
                case 3: destR = kingRank + 1; destF = kingFile - 1; break;
                case 4: destR = kingRank - 1; destF = kingFile + 1; break;
                case 5: destR = kingRank - 1; destF = kingFile - 1; break;
                case 6: destR = kingRank + 1; destF = kingFile; break;
                case 7: destR = kingRank - 1; destF = kingFile; break;
            }
            // get a temporary board and list of pieces where the move was made
            // so we can see if that would put the player's king in check
            std::vector<Piece> t3 = p;
            t3[kingi].movePiece(destR, destF);
            std::string t4 [8][8];
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    t4[j][i] = " ";
                }
            }
            for (int i = 0; i < p.size(); i++)
            {
                t4[t3[i].getRank()][t3[i].getFile()] = t3[i].getType();
            }
            
            if (!putsKingInCheck(player, t4, t3))
            {return false;}
        }

        // find the threats
        std::vector<int> threats = findThreats(player, board, p);
        if (threats.size() == 1)
        {
            int threati = threats[0];
            int threatR = p[threati].getRank();
            int threatF = p[threati].getFile();
            
            for (int i = 0; i < t2.size(); i++)
            {
                std::vector<Piece> t3 = p;
                t3[t2i[i]].movePiece(threatR, threatF);
                std::string t4 [8][8];
                for (int k = 0; k < 8; k++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        t4[j][k] = " ";
                    }
                }
                for (int k = 0; k < p.size(); k++)
                {
                    t4[t3[k].getRank()][t3[k].getFile()] = t3[k].getType();
                }
                
                
                if (!putsKingInCheck(player, t4, t3))
                {return false;}
            }
        }
        else
        {
            for (int i = 0; i < threats.size(); i++)
            {
                if (p[threats[i]].getType() == "N")
                    return true;
                else
                {
                    int threati = threats[i];
                    int threatR = p[threati].getRank();
                    int threatF = p[threati].getFile();
                    std::vector<int> targetR;
                    std::vector<int> targetF;
                    
                    if (kingRank == threatR)
                    {
                        if (kingFile > threatF)
                        {
                            for (int j = kingFile - 1; j > threatF; j--)
                            {
                                if (board[kingRank][j] == " ")
                                {
                                    targetR.push_back(kingRank);
                                    targetF.push_back(j);
                                }
                                else
                                    break;
                            }
                        }
                        else // kingFile < threatF
                        {
                            for (int j = kingFile + 1; j < threatF; j++)
                            {
                                if (board[kingRank][j] == " ")
                                {
                                    targetR.push_back(kingRank);
                                    targetF.push_back(j);
                                }
                                else
                                    break;
                            }
                        }
                    }
                    else if (kingFile == threatF)
                    {
                        if (kingRank > threatR)
                        {
                            for (int j = kingRank - 1; j > threatR; j--)
                            {
                                if (board[j][kingFile] == " ")
                                {
                                    targetR.push_back(j);
                                    targetF.push_back(kingFile);
                                }
                                else
                                    break;
                            }
                        }
                        else // kingRank < threatR
                        {
                            for (int j = kingRank + 1; j < threatR; j++)
                            {
                                if (board[j][kingFile] == " ")
                                {
                                    targetR.push_back(j);
                                    targetF.push_back(kingFile);
                                }
                                else
                                    break;
                            }
                        }
                    }
                    else if (kingRank > threatR)
                    {
                        if (kingFile > threatF)
                        {
                            int r = kingRank - 1;
                            int f = kingFile - 1;
                            while(r > threatR && f > threatF)
                            {
                                if (board[r][f] == " ")
                                {
                                    targetR.push_back(r);
                                    targetF.push_back(f);
                                }
                                else
                                    break;
                                r--;
                                f--;
                            }
                        }
                        else // kingFile < threatF
                        {
                            int r = kingRank - 1;
                            int f = kingFile + 1;
                            while(r > threatR && f < threatF)
                            {
                                if (board[r][f] == " ")
                                {
                                    targetR.push_back(r);
                                    targetF.push_back(f);
                                }
                                else
                                    break;
                                r--;
                                f++;
                            }
                        }
                    }
                    else//   kingRank < threatR
                    {
                        if (kingFile > threatF)
                        {
                            int r = kingRank + 1;
                            int f = kingFile - 1;
                            while(r < threatR && f > threatF)
                            {
                                if (board[r][f] == " ")
                                {
                                    targetR.push_back(r);
                                    targetF.push_back(f);
                                }
                                else
                                    break;
                                r++;
                                f--;
                            }
                        }
                        else // kingFile < threatF
                        {
                            int r = kingRank + 1;
                            int f = kingFile + 1;
                            while(r < threatR && f < threatF)
                            {
                                if (board[r][f] == " ")
                                {
                                    targetR.push_back(r);
                                    targetF.push_back(f);
                                }
                                else
                                    break;
                                r++;
                                f++;
                            }
                        }
                    }
                    // try pieces on those spaces
                    for (int j = 0; j < targetR.size(); j++)
                    {
                        for (int i = 0; i < t2.size(); i++)
                        {
                            std::vector<Piece> t3 = p;
                            t3[t2i[i]].movePiece(targetR[j], targetF[j]);
                            std::string t4 [8][8];
                            for (int k = 0; k < 8; k++)
                            {
                                for (int j = 0; j < 8; j++)
                                {
                                    t4[j][k] = " ";
                                }
                            }
                            for (int k = 0; k < p.size(); k++)
                            {
                                t4[t3[k].getRank()][t3[k].getFile()] = t3[k].getType();
                            }
                            
                            
                            if (!putsKingInCheck(player, t4, t3))
                            {return false;}
                        }   
                    }
                }
            }
            return true;
        }
    }
    return false;
}

std::vector<int> findThreats(int player, std::string board[][8],
                             std::vector <Piece> p)
{
    std::vector<int> threats;
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);                    
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);
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
                    threats.push_back(pindex);
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
                threats.push_back(i);               
            if (r == kingRank - 2 &&
               (f == kingFile + 1 || f == kingFile - 1))
                threats.push_back(i);
            if (f == kingFile + 2 &&
               (r == kingRank + 1 || r == kingRank - 1))
                threats.push_back(i);
            if (f == kingFile - 2 &&
               (r == kingRank + 1 || r == kingRank - 1))
                threats.push_back(i);
            if (flag)
                break;
            flag = true;
        }
    }
    return threats;
}

bool Board::stalemate(int player) // turn player
{
//     std::cout << "\n\nin stalemate" << std::endl;
//     std::cout << "turn player is " << (player ? "Black" : "White") << std::endl;
    
    std::vector<Piece> t2;// turn player pieces
    std::vector<int> t2i;// t2 piece index in p
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i].getPlayer() == player)
        {
            t2.push_back(p[i]);
            t2i.push_back(i);
        }
    }
//     std::cout << t2.size() << std::endl;
//     std::cout << std::endl << std::endl << std::endl;
//     for (int i = 0; i < t2.size(); i++)
//         std::cout << t2[i] << std::endl;
//     std::cout << std::endl << std::endl << std::endl;
    bool inCheck = putsKingInCheck(player, board, p);

//     std::cout << "in check? " << (inCheck ? "Yes" : "No") << std::endl;
    
    if (inCheck)
        return false;
    else
    {
        bool stalemate = true;
        // find king
        int kingRank, kingFile, kingi;
        for (int i = 0; i < t2.size(); i++)
        {
            if (t2[i].getType() == "K")
            {
                kingRank = t2[i].getRank();
                kingFile = t2[i].getFile();
                kingi = i;
                break;
            }
        }
//         std::cout << kingRank << ", "  << kingFile << std::endl;
//         std::cout << "can king move?" << std::endl;;
 
        // can king move?
        for (int i = 0; i < 8; i++)
        {
            int destR, destF;
            switch(i)
            {
                case 0: destR = kingRank; destF = kingFile + 1; break;
                case 1: destR = kingRank; destF = kingFile - 1; break;
                case 2: destR = kingRank + 1; destF = kingFile + 1; break;
                case 3: destR = kingRank + 1; destF = kingFile - 1; break;
                case 4: destR = kingRank - 1; destF = kingFile + 1; break;
                case 5: destR = kingRank - 1; destF = kingFile - 1; break;
                case 6: destR = kingRank + 1; destF = kingFile; break;
                case 7: destR = kingRank - 1; destF = kingFile; break;
            }
//             std::cout << destR << ", " << destF << std::endl;
//             std::cout << t2[kingi];
            if (checkMove(t2i[kingi], destR, destF))
            {

//                 std::cout << "No" << std::endl;

                stalemate = false;
                break;
            }
        }
        if (!stalemate)
            return false;

//         std::cout << "Yes" << std::endl; 

        // can the other player pieces move?

//         std::cout << "other pieces..." << std::endl;

        for (int i = 0; i < t2.size(); i++)
        {
            int pieceRank = t2[i].getRank(),
                pieceFile = t2[i].getFile(),
                pieceIndex = t2i[i];
            std::string pieceType = t2[i].getType();

//             std::cout << "\t" << pieceType << " " << pieceRank << ", " << pieceFile;

            std::vector<int> newr;
            std::vector<int> newf;
            if (pieceType == "P")
            {

//                 std::cout << " " << (player ? "Black" : "White") << std::endl;
//                 std::cout << player << std::endl;

                if (player == 0)// player is white
                {
//                     std::cout << "entering the blarg white" << std::endl;
//                     std::cout << newr.size() << ' ' << newf.size() << std::endl;
                    
                    //straight ahead
                    newr.push_back(pieceRank + 1);
                    newf.push_back(pieceFile);
                    //attack left
                    newr.push_back(pieceRank + 1);
                    newf.push_back(pieceFile - 1);
                    //attack right
                    newr.push_back(pieceRank + 1);
                    newf.push_back(pieceFile + 1);

//                     std::cout << "got the place allocated white" << std::endl;
                    
                }
                else// player is black
                {

//                     std::cout << "entering the blarg black" << std::endl;
//                     std::cout << newr.size() << ' ' << newf.size() << std::endl;
                    //straight ahead
                    newr.push_back(pieceRank - 1);
                    newf.push_back(pieceFile);
                    //attack left
                    newr.push_back(pieceRank - 1);
                    newf.push_back(pieceFile - 1);
                    //attack right
                    newr.push_back(pieceRank - 1);
                    newf.push_back(pieceFile + 1);

//                     std::cout << "got the place allocated black" << std::endl;
                     
                }
            }
            else if (pieceType == "R")
            {
                for (int i = 0; i < 8; i++)
                {
                    // horizontal movement
                    newr.push_back(pieceRank);
                    newf.push_back(i);
                    // vertical movement
                    newr.push_back(i);
                    newf.push_back(pieceFile);
                }
            }
            else if (pieceType == "B")
            {
                int x = pieceRank + 1, y = pieceFile + 1;
                while (x < 8 && y < 8) // ++
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x++;
                    y++;
                }
                x = pieceRank - 1;
                y = pieceFile - 1;
                while (x > -1 && y > -1)//--
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x--;
                    y--;
                }
                x = pieceRank + 1;
                y = pieceFile - 1;
                while (x < 8 && y > -1)//+-
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x++;
                    y--;
                }
                x = pieceRank - 1;
                y = pieceFile + 1;
                while (x > -1 && y < 8)//-+
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x--;
                    y++;
                }
            }
            else if (pieceType == "Q")
            {
                for (int i = 0; i < 8; i++)
                {
                    // horizontal movement
                    newr.push_back(pieceRank);
                    newf.push_back(i);
                    // vertical movement
                    newr.push_back(i);
                    newf.push_back(pieceFile);
                }
                // diagonal movement
                int x = pieceRank + 1, y = pieceFile + 1;
                while (x < 8 && y < 8) // ++
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x++;
                    y++;
                }
                x = pieceRank - 1;
                y = pieceFile - 1;
                while (x > -1 && y > -1)//--
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x--;
                    y--;
                }
                x = pieceRank + 1;
                y = pieceFile - 1;
                while (x < 8 && y > -1)//+-
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x++;
                    y--;
                }
                x = pieceRank - 1;
                y = pieceFile + 1;
                while (x > -1 && y < 8)//-+
                {
                    newr.push_back(x);
                    newf.push_back(y);
                    x--;
                    y++;
                }
            }
            else if (pieceType == "N")
            {
                newr.push_back(pieceRank + 2);
                newf.push_back(pieceFile + 1);
                
                newr.push_back(pieceRank + 1);
                newf.push_back(pieceFile + 2);
                
                newr.push_back(pieceRank - 1);
                newf.push_back(pieceFile + 2);

                newr.push_back(pieceRank - 2);
                newf.push_back(pieceFile + 1);
                
                newr.push_back(pieceRank - 2);
                newf.push_back(pieceFile - 1);
                
                newr.push_back(pieceRank - 1);
                newf.push_back(pieceFile - 2);
                
                newr.push_back(pieceRank + 1);
                newf.push_back(pieceFile - 2);
                
                newr.push_back(pieceRank + 2);
                newf.push_back(pieceFile - 1);                
            }

//             std::cout << "at the checking stage" << std::endl;
//             std::cout << newr.size() << std::endl;

            for (int i = 0; i < newr.size(); i++)
            {
                
//                 std::cout << i << std::endl;

                if (checkMove(pieceIndex, newr[i], newf[i]))
                {
                    
//                     std::cout << "in the fail location ... " << std::endl;

                    stalemate = false;
                    break;
                }
            }
            return stalemate;
        }
    }
}
// king cannot make any moves but isnt in check
// rest of pieces cannot make any moves
//      (check this part by seeing in any pertient moves(based on piece)
//       are good moves)




// int main()
// {
//     return 0;
// }












// if (threats.size() > 1)
//         {
//             //find ways to block the threats
//         for loop running from 0 - 7 on rank with king fixed file
//    }
//         else
//         {
//             // find ways to kill the threats
//             for (int i = 0; i < threats.size(); i++)
//             {
//                 int threatr = p[threats[i]].getRank();
//                 int threatf = p[threats[i]].getFile();
//                 for (int i = 0; i < t2.size(); i++)
//                 {
//                     std::vector<Piece> t3 = p;
//                     t3[getPieceIndex(t2[i].getRank(), t2[i].getFile())].movePiece(threatr, threatf);
//                     std::string t4 [8][8];
//                     for (int i = 0; i < 8; i++)
//                     {
//                         for (int j = 0; j < 8; j++)
//                         {
//                             t4[j][i] = " ";
//                         }
//                     }
//                     for (int i = 0; i < p.size(); i++)
//                     {
//                         t4[t3[i].getRank()][t3[i].getFile()] = t3[i].getType();
//                     }
                    
//                     if (!putsKingInCheck(player, t4, t3))
//                     {return false;}
//                 }
//             }
//         }
//     }
//     else
//         return false;
