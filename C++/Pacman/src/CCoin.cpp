#include "CCoin.h"

CCoin :: CCoin (size_t row, size_t column, char symbol, char wallLabSign )
        : CGameEntity (make_pair ( row, column ), symbol, wallLabSign )
{
}

CCoin :: CCoin (const pair < size_t, size_t > & coordinates, char symbol, char wallLabSign)
        : CGameEntity (coordinates, symbol, wallLabSign )
{
}

void CCoin :: action ( CPacman * pacman, vector < string > & field )
{
    pacman -> increaseScore ( SCORE );
    setCoordinates ( make_pair ( DEFAULT_ROW, DEFAULT_COLUMN ) );
    pair < size_t, size_t > pacmanCoordinates = pacman -> getCoordinates();
    field [ pacmanCoordinates . first ] [ pacmanCoordinates . second ] = pacman -> getSign();
}