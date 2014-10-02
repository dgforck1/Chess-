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

