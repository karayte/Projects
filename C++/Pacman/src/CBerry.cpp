#include "CBerry.h"

CBerry :: CBerry (size_t row, size_t column, char sign, char wallLabSign )
        : CGameEntity (make_pair ( row, column ), sign, wallLabSign )
{
}

CBerry :: CBerry (const pair < size_t, size_t > & coordinates, char sign, char wallLabSign )
        : CGameEntity (coordinates, sign, wallLabSign )
{
}

void CBerry ::  action ( CPacman * pacman, vector < string > & field )
{
    pacman -> setPowerMode ( true );
    setCoordinates ( make_pair ( DEFAULT_ROW, DEFAULT_COLUMN ) );
    pair < size_t, size_t > pacmanCoordinates = pacman -> getCoordinates();
    field [ pacmanCoordinates . first ] [ pacmanCoordinates . second ] = pacman -> getSign();
}