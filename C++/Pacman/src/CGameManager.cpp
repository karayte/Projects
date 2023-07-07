#include "CGameManager.h"

CGameManager :: CGameManager ( char emptyCellSign, char wallLabyrinthSigh )
        : mCharacters ( emptyCellSign, wallLabyrinthSigh ),
          mGameField ( emptyCellSign, wallLabyrinthSigh ),
          mEmptyCellSign ( emptyCellSign )
{
}

bool CGameManager :: readMapFromFile ( istream & file )
{
    string lineOfInputField = ""s;

    vector < string > allCharactersTypes = { "pacman", "ghostTypeOne", "ghostTypeTwo", "ghostTypeThree",
                                            "coin", "berry", "teleport" };

    if ( ! mCharacters . readCharacters ( file, allCharactersTypes ) )
        return false;
    vector < pair < size_t, size_t > > allCharactersCoordinates;
    mCharacters . getAllCharactersCoordinates ( allCharactersCoordinates );

    if ( ! mGameField . readMapFromFile ( file, allCharactersCoordinates ) )
        return false;

    mGameField . changeFieldSign ( mCharacters . getPacmanSign(), mCharacters . getPacManCoordinates() );
    vector < pair < char, pair < size_t, size_t > > > allEntities;
    mCharacters . getAllEntitiesSignAndCoordinates ( allEntities );
    for ( const auto & entity : allEntities )
        mGameField . changeFieldSign ( entity . first, entity . second );

    return true;
}

void CGameManager :: checkIfGameEntitiesGotEaten ()
{
    mCharacters . checkIfGameEntitiesGotEaten ( mGameField . mField );
}

void CGameManager::makeSpiritsMove()
{
    mCharacters . makeSpiritsMove ( mGameField . mField );
}

void CGameManager :: reset ()
{
    mCharacters . reset();
    mGameField . reset();
}

void CGameManager :: printGameField () const
{
    printw ( "%s\n", "Press the button Backspace to end the game." );
    printw ( "%s\n", "Press the button Enter to start a new game." );
    printw ( "%s\n", "Press the UP key to move up." );
    printw ( "%s\n", "Press the DOWN key to move down." );
    printw ( "%s\n", "Press the LEFT key to move left." );
    printw ( "%s\n", "Press the RIGHT key to move right." );
    printw ( "%s\n", "Press the button Space to mPause/unpause the game." );
    printw ("%s %d\n", "Current score is ", mCharacters . getScore() );

    mGameField . printGameField();
}
