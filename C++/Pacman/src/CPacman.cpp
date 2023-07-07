#include "CPacman.h"

CPacman :: CPacman ()
        : mScore ( 0 ),
          mPowerMode ( false ),
          mCoordinates ( 1,1 ),
          mBeginningOfPowerMode ( chrono :: steady_clock :: now () ),
          mSign ( 'P' ),
          mNumOfLives ( NUM_OF_LIVES )
{
}

void CPacman :: die ()
{
    if ( mNumOfLives == 0 )
        return;
    mNumOfLives --;
}


void CPacman :: setSign ( char sign )
{
    mSign = sign;
}

void CPacman :: increaseScore ( int value )
{
    mScore += value;
}

void CPacman :: setPowerMode ( bool mode )
{
    mPowerMode = mode;
    if ( mode )
        mBeginningOfPowerMode = chrono :: steady_clock :: now ();
}

bool CPacman :: isPowerMode ()
{
    if (mPowerMode &&
        chrono :: duration_cast < chrono :: seconds > (chrono :: steady_clock :: now () - mBeginningOfPowerMode ) . count () > 5 )
        mPowerMode = false;
    return mPowerMode;
}

pair < size_t, size_t > CPacman :: getCoordinates () const
{
    return mCoordinates;
}

void CPacman :: setCoordinates ( const pair < size_t, size_t > & coordinates )
{
    mCoordinates = coordinates;
}

char CPacman :: getSign () const
{
    return mSign;
}

bool CPacman::isDead() const
{
    return mNumOfLives == 0;
}

int CPacman :: getScore() const
{
    return mScore;
}

void CPacman :: reset()
{
    mScore = 0;
    mPowerMode = false;
    mNumOfLives = NUM_OF_LIVES;
}
