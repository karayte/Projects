#include "CCharacterManager.h"

CCharacterManager :: CCharacterManager ( char emptyCellSign, char wallLabyrinthSign )
        : mPacman(),
          mAllEntities(),
          mAllSpirits(),
          mSpiritsVulnerableModeSign ( SPIRITS_VULNERABLE_MODE_SIGN ),
          mAllCharactersSigns (),
          mEmptyCellSign ( emptyCellSign ),
          mWallLabyrinthSigh ( wallLabyrinthSign )
{
}

bool CCharacterManager :: readCharacters ( istream & file, const vector < string > & allCharactersType )
{

    for ( const auto & type : allCharactersType )
    {
        if ( ! initializeCharactersFromInput ( file, type ) )
            return false;
    }
    return true;
}

bool CCharacterManager :: initializeCharactersFromInput ( istream & file, const string & type )
{
    string line;
    char sign ( ' ' );
    int row = -1, column = -1;
    size_t numOfTeleports = 0;

    if ( getline ( file, line ) )
    {
        istringstream stringByTokens (line );
        stringByTokens >> sign >> row >> column;
        if ( ( ! stringByTokens . good() && ! stringByTokens . eof() )
             || sign == mSpiritsVulnerableModeSign
             || sign == mEmptyCellSign
             || sign == mWallLabyrinthSigh
             || mAllCharactersSigns . find ( sign ) != mAllCharactersSigns . end() )
            return false;
        if ( row <= 0 || column <= 0 )
            return false;
        initializeOrInsertCharacters ( type, make_pair ( row, column ), sign );
        mAllCharactersSigns . insert ( sign );
        if ( type == "teleport" )
            numOfTeleports ++;
        row = -1;
        column = -1;
        if ( type != "pacman" && sign == mPacman . getSign() )
            return false;
        while ( type != "pacman" && stringByTokens >> row )
        {
            if (! ( stringByTokens >> column ) || row <= 0 || column <= 0 )
                return false;
            initializeOrInsertCharacters ( type, make_pair ( row, column ), sign );
            if ( type == "teleport" )
                numOfTeleports ++;
        }
        if ( stringByTokens >> sign )
            return false;
    }
    else
        return false;
    if ( type == "teleport" )
        return numOfTeleports >= 2;
    return true;
}

void CCharacterManager :: initializeOrInsertCharacters ( const string & type, pair < size_t, size_t > && coordinates, char symbol )
{
    if ( type == "pacman" )
    {
        mPacman . setSign (symbol );
        mPacman . setCoordinates (coordinates );
        return;
    }
    if ( type == "coin" )
    {
        mAllEntities . push_back ( make_unique < CCoin > ( coordinates, symbol, mWallLabyrinthSigh ) );
        return;
    }
    if ( type == "berry" )
    {
        mAllEntities . push_back ( make_unique < CBerry > ( coordinates, symbol, mWallLabyrinthSigh ) );
        return;
    }
    if ( type == "teleport" )
    {
        mAllEntities . push_back ( make_unique < CTeleport > ( coordinates, symbol, mWallLabyrinthSigh ) );
        return;
    }
    if ( type == "ghostTypeOne" )
    {
        mAllSpirits . push_back ( make_unique < CGhostTypeOne > ( coordinates, symbol, & mPacman, mSpiritsVulnerableModeSign,
                                                                  mEmptyCellSign, mWallLabyrinthSigh ) );
        return;
    }
    if ( type == "ghostTypeTwo" )
    {
        mAllSpirits . push_back ( make_unique < CGhostTypeTwo > ( coordinates, symbol, & mPacman, mSpiritsVulnerableModeSign,
                                                                  mEmptyCellSign, mWallLabyrinthSigh) );
        return;
    }
    if ( type == "ghostTypeThree" )
    {
        mAllSpirits . push_back ( make_unique < CGhostTypeThree > ( coordinates, symbol, & mPacman, mSpiritsVulnerableModeSign,
                                                                    mEmptyCellSign, mWallLabyrinthSigh ) );
        return;
    }
}

void CCharacterManager :: reset ()
{
    mAllCharactersSigns . clear();
    mAllSpirits . clear();
    mAllEntities . clear();
    mPacman . reset();
}

void CCharacterManager :: getAllCharactersCoordinates ( vector < pair < size_t, size_t > > & allCoordinates ) const
{
    allCoordinates . clear();
    allCoordinates . emplace_back ( mPacman . getCoordinates() );
    for ( const auto & entity : mAllEntities )
        allCoordinates . push_back ( entity -> getCoordinates() );
    for ( const auto & ghost : mAllSpirits )
        allCoordinates . push_back ( ghost -> getCoordinates() );
}

void CCharacterManager :: getAllEntitiesSignAndCoordinates ( vector < pair < char, pair < size_t, size_t > > > & allEntities ) const
{
    for ( const auto & entity : mAllEntities )
        allEntities . emplace_back ( entity -> getSign(), entity -> getCoordinates() );
}

void CCharacterManager :: setPacmanCoordinates ( const pair < size_t, size_t > & coordinates )
{
    mPacman . setCoordinates ( coordinates );
}

pair < size_t, size_t > CCharacterManager :: getPacManCoordinates () const
{
    return mPacman . getCoordinates();
}

char CCharacterManager :: getPacmanSign () const
{
    return mPacman . getSign();
}

bool CCharacterManager :: pacmanIsDead() const
{
    return mPacman . isDead ();
}

bool CCharacterManager :: pacmanPowerMode ()
{
    return mPacman . isPowerMode();
}

int CCharacterManager :: getScore () const
{
    return mPacman . getScore();
}

void CCharacterManager :: makeSpiritsMove ( vector < string > & gameField )
{
    for ( auto & ghost : mAllSpirits)
    {
        if ( ghost -> getCoordinates() == mPacman . getCoordinates() )
            ghost -> action();

        mPacman . isPowerMode() ? ghost -> moveToAvoidPacman (gameField )
                                : ghost -> move ( gameField [ 0 ] . size(), gameField . size(), gameField );

        if ( ghost -> getCoordinates() == mPacman . getCoordinates() )
            ghost -> action();
    }
}

void CCharacterManager :: setSpiritsVulnerableMode ( bool mode )
{
    for ( auto & spirit : mAllSpirits )
        spirit -> switchVulnerableMode ( mode );
}

void CCharacterManager :: checkIfSpiritsAreAtPacmanPlace ()
{
    for ( const auto & ghost : mAllSpirits )
    {
        if ( ghost -> getCoordinates() == mPacman . getCoordinates() )
            ghost -> action();
    }
}

void CCharacterManager :: checkIfGameEntitiesGotEaten ( vector < string > & gameField )
{
    pair < size_t, size_t > entityCoordinates;
    for ( auto & entity : mAllEntities )
    {
        entityCoordinates = entity -> getCoordinates();
        if ( entityCoordinates == mPacman . getCoordinates() )
        {
            gameField [ entityCoordinates . first ] [ entityCoordinates . second ] = mEmptyCellSign;
            entity -> action ( & mPacman, gameField );
        }
    }
}
