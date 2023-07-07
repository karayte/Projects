#include "CGhostTypeThree.h"


void CGhostTypeThree :: move (size_t fieldWidth, size_t fieldHeight, vector < string > & field )
{
    if ( mIsDead )
        return;
    vector < pair < int, int > > possibleDirections = {{-1, 0 }, {1, 0 }, {0, -1 }, {0, 1 } };
    shuffle (possibleDirections . begin(), possibleDirections . end(), mt19937 (random_device () () ) );

    for ( auto dir : possibleDirections )
    {
        size_t newY = mCoordinates . first + dir . first;
        size_t newX = mCoordinates . second + dir . second;

        if ( canStepOn ( newY, newX, field ) )
        {
            setCoordinatesAndPrintPrevSign ( make_pair ( newY, newX ), field );
            changeFieldAndSavePrevSign ( field );
            break;
        }
    }
}


