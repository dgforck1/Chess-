#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>

class Piece
{
public:
    Piece(int r, int f, int p, std::string t, bool m = false)
        : rank(r), file(f), player(p), type(t), moved(m)
    {};

    void print() const;
    int getRank() const;
    int getFile() const;
    int getPlayer() const;
    bool getMoved() const;
    std::string getType() const;
    bool checkMove(int destR, int destF, std::string b[][8],
                   std::vector<Piece> p) const;// checks if move made is good
    bool checkMove2(int destR, int destF, std::string b[][8],
                    std::vector<Piece> p) const;// checks if would be able to put king into check
    void movePiece(int newR, int newF);
    
private:
    int rank, file, player;
    bool moved;
    std::string type;
};


class Board
{
public:
    Board()
    {
        initalizePieces();
        updateBoard();
    }
    // need a constructor that accepts a string of moves made and builds that board

    void initalizePieces();
    void updateBoard();
    Piece getPiece(int r, int f);// piece by rank and file
    Piece getPiece(int i);// piece by index
    int getPieceIndex(int r, int f);// find the pieces index by its rank and file
    bool checkMove(int i, int destR, int destF);    
    void movePiece(int i, int destR, int destF);
    std::string capturePiece(int r, int f);
    std::string capturePiece(int i);
    void printBoard() const;
    int getPieceSize() const;
    bool checkmate(int);
    
private:
    std::string board[8][8];
    std::vector<Piece> p;
};

#endif
