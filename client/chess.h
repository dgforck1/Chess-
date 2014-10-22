#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>

class Piece
{
public:
    Piece(){};
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

#endif
