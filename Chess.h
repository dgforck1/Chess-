#ifndef CHESS_H
#define CHESS_H

class Piece
{
public:
    //constructors
    Piece() {}; 
    Piece(int r, int f, int p, std::string t) :
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





#endif
