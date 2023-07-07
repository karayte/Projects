#include "CGameMap.h"


CGameMap :: CGameMap (  char emptyCellSign, char wallLabyrinthSigh )
        : mField (),
          mGameFieldHeight ( 0 ),
          mGameFieldWidth ( 0 ),
          mEmptyCellSign ( emptyCellSign ),
          mWallLabyrinthSigh ( wallLabyrinthSigh )
{
}

bool CGameMap :: isRowValid ( const string & row ) const
{
    for ( const auto & sign : row )
    {
        if ( sign != mEmptyCellSign && sign != mWallLabyrinthSigh )
            return false;
    }
    return true;
}

bool CGameMap :: readMapFromFile ( istream & file, const vector < pair < size_t, size_t > > & allCoordinates )
{
    mGameFieldHeight = 0;
    mGameFieldWidth = 0;
    string lineOfInputField = ""s;

    while ( getline (file, lineOfInputField ) )
    {
        if ( ! mGameFieldWidth )
        {
            mGameFieldWidth = lineOfInputField.length();
        }
        if ( mGameFieldWidth != lineOfInputField . length() || ! isRowValid ( lineOfInputField ) )
            return false;
        mField . emplace_back (move (lineOfInputField ) );
        mGameFieldHeight ++;
    }

    if ( mGameFieldHeight == 0 || mGameFieldWidth == 0 || ! checkAllCharactersCoordinates ( allCoordinates ) )
        return false;

    return true;
}

void CGameMap :: printGameField () const
{
    for (size_t i = 0; i < mGameFieldHeight; i ++ )
        printw ("%s\n", mField [ i ] . c_str () );
    refresh();
}



void CGameMap :: reset ()
{
    mField . clear();
    mGameFieldWidth = 0;
    mGameFieldHeight = 0;
}

bool CGameMap :: canStepOn ( size_t row, size_t column ) const
{
    if ( row >= mGameFieldHeight || column >= mGameFieldWidth )
        return false;
    if (mField [ row ] [ column ] == mWallLabyrinthSigh )
        return false;
    return true;
}

bool CGameMap :: checkAllCharactersCoordinates ( const vector < pair < size_t, size_t > > & allCoordinates ) const
{
    for ( const auto & coordinates : allCoordinates )
    {
        if ( ! canStepOn ( coordinates . first, coordinates . second ) )
            return false;
    }
    return true;
}

void CGameMap :: changeFieldSign ( char sign, const pair < size_t, size_t > & coordinates )
{
    mField [ coordinates . first ] [ coordinates . second ] = sign;
}
