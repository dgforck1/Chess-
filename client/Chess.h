#ifndef CHESS_H
#define CHESS_H

#include<iostream>
#include<vector>

class Piece
{
public:
    //constructors
    Piece() {}; 
    Piece(int p, std::string t, int r, int f) :
        rank(r), file(f), player(p), type(t), moved(false)
    {};
    

    //methods
    int getRank() const;
    int getFile()const;
    int getPlayer() const;
    bool getMoved() const;
    std::string getType() const;
    bool checkMove(int destR, int destF, std::string B[][8],
                   std::vector< Piece > p) const;
    void movePiece(int destR, int destF);
    void print() const;
    
    
private:
    //members
    int rank, file, player;
    bool moved;
    std::string type;
};



class Board
{
public:
    Board() //default constructor
    {
        initializePieces();
        updateBoard();
    }

    void initializePieces();
    void updateBoard();
    Piece getPiece(int r, int f);
    Piece getPiece(int i);
    int getPieceIndex(int r, int f);
    bool checkMove(int i, int destR, int destF);    
    void movePiece(int i, int destR, int destF);
    std::string capturePiece(int r, int f);
    std::string capturePiece(int i);
    void printBoard() const;
    int getPieceSize() const;

    
    
private:
    std::string board[8][8];
    std::vector< Piece > p;
    
};


#endif
