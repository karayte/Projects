#include "CGhost.h"

set < char > CGhost :: mAllSignsOfSpirits = {};

CGhost :: CGhost (const pair < size_t, size_t > & coordinates, char sign, CPacman * pacman, char vulnerableSign,
                  char deadSign, char wallLabyrinthSign )
        : mCoordinates ( coordinates ),
          mSign ( sign ),
          mPrevFieldSign ( deadSign ),
          mVulnerableModeSign ( vulnerableSign ),
          mDeadSign ( deadSign ),
          mWallLabyrinthSign ( wallLabyrinthSign ),
          mPacman ( pacman ),
          mVulnerableModeActivated ( false ),
          mIsDead ( false )
{
    mAllSignsOfSpirits . insert ( sign );
    mAllSignsOfSpirits . insert ( vulnerableSign );
}

CGhost :: CGhost ( size_t row, size_t column, char sign, CPacman * pacman, char vulnerableSign, char deadSign,
                  char wallLabyrinthSign )
        : mCoordinates ( make_pair (row, column ) ),
          mSign ( sign ),
          mPrevFieldSign ( deadSign ),
          mVulnerableModeSign ( vulnerableSign ),
          mDeadSign ( deadSign ),
          mWallLabyrinthSign ( wallLabyrinthSign ),
          mPacman ( pacman ),
          mVulnerableModeActivated ( false ),
          mIsDead ( false )
{
    mAllSignsOfSpirits . insert ( sign );
    mAllSignsOfSpirits . insert ( vulnerableSign );
}

pair < size_t, size_t > CGhost :: getCoordinates () const
{
    return mCoordinates;
}

void CGhost :: switchVulnerableMode ( bool mode )
{
    mVulnerableModeActivated = mode;
}

char CGhost :: getSign () const
{
    return mVulnerableModeActivated ? mVulnerableModeSign : mSign;
}

void CGhost :: die ()
{
    mPacman -> increaseScore (SCORE_FOR_DEAD_SPIRIT );
    mSign = mDeadSign;
    mVulnerableModeSign = mDeadSign;
    mIsDead = true;
}

void CGhost :: action ()
{
    if ( mIsDead )
        return;

    mVulnerableModeActivated ? die() : mPacman -> die ();
}

pair < size_t, size_t > CGhost :: BFS ( pair < size_t, size_t > start, pair < size_t, size_t > target,
                                        const vector < string > & field ) const
{
    size_t field_height = field . size();
    size_t field_width = 0;
    if ( field_height )
        field_width = field [ 0 ] . size();
    vector < vector < bool > > visited ( field_height, vector < bool > ( field_width, false ) );
    queue < pair < int, int > > queue_of_coordinates;
    queue_of_coordinates . push ( start );
    visited [ start . first ] [ start . second ] = true;
    map < pair < size_t, size_t >, pair < size_t, size_t > > coordinates_with_predecessors;

    while ( ! queue_of_coordinates . empty() )
    {
        pair < size_t, size_t > curr_coordinates = queue_of_coordinates . front();
        queue_of_coordinates . pop();

        for ( const auto & dir : directions )
        {
            pair < size_t, size_t > next_dir_coordinates = {curr_coordinates . first + dir . first, curr_coordinates . second + dir . second };

            if ( canStepOn ( next_dir_coordinates . first, next_dir_coordinates . second, field )
            && ! visited [ next_dir_coordinates . first ] [ next_dir_coordinates . second ] )
            {
                visited [ next_dir_coordinates . first ] [ next_dir_coordinates . second ] = true;
                coordinates_with_predecessors . insert ( make_pair (next_dir_coordinates, curr_coordinates ) );
                if ( next_dir_coordinates == target )
                {
                    pair < size_t, size_t > curr_dir_coordinates = next_dir_coordinates;
                    pair < size_t, size_t > prev_dir_coordinates = next_dir_coordinates;
                    while ( prev_dir_coordinates != start )
                    {
                        auto it = coordinates_with_predecessors . find ( prev_dir_coordinates );
                        if ( it == coordinates_with_predecessors . end() )
                            return start;
                        curr_dir_coordinates = prev_dir_coordinates;
                        prev_dir_coordinates = it -> second;
                    }
                    return curr_dir_coordinates;
                }
                queue_of_coordinates . push ( next_dir_coordinates );
            }
        }
    }

    return start;

}

void CGhost :: changeFieldAndSavePrevSign ( vector < string > & field )
{
    mPrevFieldSign = field [ mCoordinates . first ] [ mCoordinates . second ];
    field [ mCoordinates . first ] [ mCoordinates . second ] = getSign();
}

size_t manhattanDistance ( size_t x1, size_t y1, size_t x2, size_t y2 )
{
    size_t distance = 0;
    x1 >= x2 ? distance += x1 - x2 : distance += x2 - x1;
    y1 >= y2 ? distance += y1 - y2 : distance += y2 - y1;

    return distance;
}

void CGhost :: moveToAvoidPacman ( vector < string > & field )
{
    pair < size_t, size_t > pacman_coordinates = mPacman -> getCoordinates();
    size_t distance = manhattanDistance (pacman_coordinates . first, pacman_coordinates . second,
                                         mCoordinates . first, mCoordinates . second );

    for ( const auto & dir : directions )
    {
        if ( canStepOn (mCoordinates . first + dir . first, mCoordinates . second + dir . second, field )
          && manhattanDistance (pacman_coordinates . first, pacman_coordinates . second,
                                mCoordinates . first + dir . first, mCoordinates . second + dir . second ) > distance )
        {
            setCoordinatesAndPrintPrevSign ( make_pair (mCoordinates . first + dir . first, mCoordinates . second + dir . second ),
                                             field );
            changeFieldAndSavePrevSign ( field );
            break;
        }
    }
}

void CGhost :: setCoordinatesAndPrintPrevSign ( const pair < size_t, size_t > & coordinates, vector < string > & field )
{
    if (mPrevFieldSign != mPacman -> getSign()
        && mAllSignsOfSpirits . find (mPrevFieldSign ) == mAllSignsOfSpirits . end() )
        field [ mCoordinates . first ] [ mCoordinates . second ] = mPrevFieldSign;
    mCoordinates = coordinates;
}

bool CGhost :: canStepOn ( size_t row, size_t column, const vector < string > & field ) const
{
    if ( row >= field . size() || column >= field [ 0 ] . size() )
        return false;
    if (field [ row ] [ column ] == mWallLabyrinthSign
         || mAllSignsOfSpirits . find (field [ row ] [ column ] ) != mAllSignsOfSpirits . end() )
        return false;
    return true;
}

