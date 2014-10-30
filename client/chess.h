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
    bool operator==(const Piece &)const;
private:
    int rank, file, player;
    bool moved;
    std::string type;
};

std::ostream & operator<<(std::ostream & cout, const Piece & p)
{
    cout << "<Piece: rank: " << p.getRank() << ", file: " << p.getFile()
         << ", player: " << (p.getPlayer() ? "Black" : "White")
         << ", moved: " << p.getMoved() << ", type: " << p.getType()
         << ">";
    return cout;
}


class Board
{
public:
    Board()
    {
        initalizePieces();
        updateBoard();
    };
    // need a constructor that accepts a string of moves made and builds that board

    void initalizePieces();
    void updateBoard();
    Piece getPiece(int r, int f);// piece by rank and file
    Piece getPiece(int i);// piece by index
    int getPieceIndex(int r, int f);// find the pieces index by its rank and file
    bool checkMove(int i, int destR, int destF);//, std::string & prev);    
    void movePiece(int i, int destR, int destF);
    std::string capturePiece(int r, int f);
    std::string capturePiece(int i);
    void printBoard() const;
    int getPieceSize() const;
    std::string getSquare(int, int) const;
    bool checkmate(int);
    bool stalemate(int);
private:
    std::string board[8][8];
    std::vector<Piece> p;
};

std::ostream & operator<<(std::ostream & cout, const Board & b)
{
    cout << "  1 2 3 4 5 6 7 8\n";
    for (int i = 0; i < 8; i++)
    {
        cout << " +-+-+-+-+-+-+-+-+\n" << 8 - i;
        for (int j = 0; j < 8; j++)
        {
            cout << "|" << b.getSquare(i, j);
        }
        cout << "|" << 8 - i << "\n";
    }
    cout << " +-+-+-+-+-+-+-+-+\n";
    cout << "  1 2 3 4 5 6 7 8\n";
    return cout;
}

#endif
