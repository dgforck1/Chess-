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
        rank(r), file(f), player(p), type(t)
    {};

    //methods
    int getRank() const;
    int getFile()const;
    int getPlayer() const;
    std::string getType() const;
    bool checkMove(int r, int f, const char b[8][8]);
    void movePiece(int r, int f, char b[8][8]);
    void print() const;
    
private:
    //members
    int rank, file, player;
    std::string type;
};



class Board
{
public:
    Board() //default constructor
    {
        initializePieces();
        initializeBoard();
    }

    void initializePieces();
    void initializeBoard();
    void updateBoard();
    Piece& getPiece(int r, int f);
    int getPieceIndex(int r, int f);
    bool checkMove(int i, int sourceR, int sourceF,
                   int destR, int destF) const;
    void movePiece(int i, int r, int f);
    void capturePiece(int r, int f);
    void printBoard() const;
    
private:
    char board[8][8];
    std::vector< Piece > p;
    
};


#endif
