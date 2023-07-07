#include "CGhostTypeOne.h"


void CGhostTypeOne :: move (size_t fieldWidth, size_t fieldHeight, vector < string > & field )
{
    if ( mIsDead )
        return;
    pair < size_t, size_t > pacmanPoint = mPacman -> getCoordinates();
    pair < size_t, size_t > ghostPoint = mCoordinates;
    pair < size_t, size_t > newCoordinates = BFS (ghostPoint, pacmanPoint, field );
    setCoordinatesAndPrintPrevSign (newCoordinates, field );
    changeFieldAndSavePrevSign ( field );
}