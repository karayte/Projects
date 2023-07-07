#include "CTeleport.h"

vector < pair < size_t, size_t > > CTeleport :: mAllTeleports = {};

CTeleport :: CTeleport ( size_t row, size_t column, char symbol, char wallLabSign )
        : CGameEntity (make_pair ( row, column ), symbol, wallLabSign )
{
    mAllTeleports . emplace_back ( row, column );
}

CTeleport :: CTeleport ( const pair < size_t, size_t > & coordinates, char symbol, char wallLabSign )
        : CGameEntity (coordinates, symbol, wallLabSign )
{
    mAllTeleports . emplace_back ( coordinates );
}

bool canStepOn ( size_t row, size_t column, char wrongSign, const vector < string > & field )
{
    if ( row >= field . size() || column >= field [ 0 ] . size() )
        return false;
    if ( field [ row ] [ column ] == wrongSign )
        return false;
    return true;
}

void CTeleport :: action ( CPacman * pacman, vector < string > & field )
{
    pair < size_t, size_t > newCoordinatesForPacman;
    shuffle ( mAllTeleports . begin(), mAllTeleports . end(), mt19937 (random_device () () ) );
    for ( const auto & coordinates : mAllTeleports )
    {
        if ( coordinates != mCoordinates )
        {
            newCoordinatesForPacman = coordinates;
            break;
        }
    }
    const vector < pair < int, int > > coordinates_near = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
    for ( const auto & dir : coordinates_near )
    {
        if ( canStepOn ( newCoordinatesForPacman . first + dir . first,
                         newCoordinatesForPacman . second + dir . second,
                         mWallLabyrinthSign, field ) )
        {
            newCoordinatesForPacman . first += dir . first;
            newCoordinatesForPacman . second += dir . second;
            break;
        }
    }

    field [ mCoordinates . first ] [ mCoordinates . second ] = mSign;
    pacman -> setCoordinates ( newCoordinatesForPacman );
    field [ newCoordinatesForPacman . first ] [ newCoordinatesForPacman . second ] = pacman -> getSign();
}
