#include "CGhostTypeTwo.h"


void CGhostTypeTwo :: move (size_t fieldWidth, size_t fieldHeight, vector < string > & field )
{
    if ( mIsDead )
        return;

    vector < pair < int, int > > directions = { { 0, 3 }, { 3, 0 }, { 0, -3 }, { -3, 0 },
                                                { 0, 2 }, { 2, 0 }, { 0, -2 }, { -2, 0 },
                                                { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
    for ( auto dir : directions )
    {
        pair < size_t, size_t > pacmanCoordinates = mPacman -> getCoordinates();
        pacmanCoordinates . first += dir . first;
        pacmanCoordinates . second += dir . second;

        if ( canStepOn (pacmanCoordinates . first, pacmanCoordinates . second, field ) )
        {
            pair < size_t, size_t > newCoordinates = BFS (mCoordinates, pacmanCoordinates, field );
            setCoordinatesAndPrintPrevSign (newCoordinates, field );
            changeFieldAndSavePrevSign ( field );
            break;
        }
    }
}
