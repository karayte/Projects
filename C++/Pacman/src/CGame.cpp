#include "CGame.h"

CGame :: CGame ( string name )
        : mFileName ( move ( name ) ),
          mLeadersBoard (),
          mEmptyCellSigh ( DEFAULT_EMPTY_SIGN ),
          mWallLabSign ( DEFAULT_WALL_SIGN ),
          mNewGame ( false ),
          mTimeToDie ( false ),
          mPause ( false ),
          mNumOfPlayer ( 1 ),
          mCurrStep ( SPIRITS_SPEED )
{
}

bool CGame :: openFileAndGetSign()
{
    ifstream gameMap ( mFileName );

    if ( ! readFirstLine ( gameMap ) || ! runTheGame ( gameMap ) )
    {
        return false;
    }
    showGameResults ();
    endwin();
    return true;
}

bool cmpPlayers ( const pair < int, string > & player1, const pair < int, string > & players2 )
{
    return player1 . first > players2 . first;
}

void CGame :: showGameResults()
{
    clear();
    int key = -1;
    string borders ( 30, '#' );
    sort ( mLeadersBoard . begin(), mLeadersBoard . end(), cmpPlayers );
    printw ( "%s\n", "Here you see our leaders board. Congratulations!" );
    printw ( "%s\n", borders . c_str() );
    int order = 1;
    for ( const auto & player : mLeadersBoard )
    {
        printw ( "%d. %s : %d\n", order, player . second . c_str(), player . first );
        order ++;
    }
    printw ( "%s\n", borders . c_str() );
    printw ( "%s\n", "To quit, please, press the button Backspace." );
    while ( key != KEY_BACKSPACE )
    {
        key = getch();
    }
}

bool CGame :: runTheGame ( ifstream & file )
{
    CGameManager game ( mEmptyCellSigh, mWallLabSign );
    if ( ! readMapAndInitializeScreen ( game, file ) )
    {
        return false;
    }

    while ( ! mTimeToDie )
    {
        if ( ! runOneGameCycle (game ) )
        {
            return false;
        }
        if ( game . mCharacters . pacmanPowerMode() )
        {
            game . mCharacters . setSpiritsVulnerableMode (true );
            while ( game . mCharacters . pacmanPowerMode() && ! mTimeToDie )
            {
                if ( ! runOneGameCycle ( game ) )
                {
                    return false;
                }
            }
            game . mCharacters . setSpiritsVulnerableMode (false );
        }
    }
    mLeadersBoard . emplace_back ( game . mCharacters . getScore(), "player number " + to_string ( mNumOfPlayer ) );
    return true;
}

bool CGame::runOneGameCycle ( CGameManager & game )
{
    if ( ! gameCycle ( game ) )
    {
        return false;
    }
    if ( mNewGame )
    {
        mLeadersBoard . emplace_back ( game . mCharacters . getScore(), "player number " + to_string ( mNumOfPlayer ) );
        mNumOfPlayer ++;
    }
    while ( mPause )
    {
        checkPauseState ( game );
    }
    return true;
}

void CGame :: checkPauseState ( CGameManager & game )
{
    int key = getch();
    if ( key == ' ' )
        mPause = false;
    game . printGameField();
    clear();
    usleep ( 100000 );
}

bool CGame :: gameCycle ( CGameManager & game )
{
    if ( mNewGame )
    {
        if ( ! openFileRereadMap ( game ) )
        {
            return false;
        }
        mNewGame = false;
        mCurrStep = 5;
    }

    if ( mCurrStep % ( game . mCharacters . getScore() >= SCORE_TO_UPDATE_SPIRITS_SPEED ? SPIRITS_SPEED_FAST_MODE : SPIRITS_SPEED ) == 0 )
    {
        game . makeSpiritsMove();
    }

    game . printGameField();

    ProcessInput (  game );

    game . mCharacters . checkIfSpiritsAreAtPacmanPlace();

    game . checkIfGameEntitiesGotEaten();

    clear();
    usleep ( 100000 );
    mCurrStep ++;
    return true;
}

bool CGame :: openFileRereadMap ( CGameManager & game )
{
    game . reset();
    endwin();
    ifstream gameMap ( mFileName );
    if ( ! readFirstLine ( gameMap ) || ! readMapAndInitializeScreen ( game, gameMap ) )
    {
        return false;
    }
    return true;
}

bool CGame :: readFirstLine ( ifstream & gameMap )
{
    if ( ! gameMap . is_open() )
    {
        cout << "Could not open the file :(" << endl;
        return false;
    }
    string line;
    char sign3 ( ' ' );
    if ( getline ( gameMap, line ) && ! line . empty() )
    {
        istringstream stringByTokens ( line );
        stringByTokens >> mEmptyCellSigh;
        if ( stringByTokens . eof() )
        {
            cout << "Wrong first line of input." << endl;
            return false;
        }
        stringByTokens >> mWallLabSign;
        if ( stringByTokens . eof() )
        {
            mWallLabSign = mEmptyCellSigh;
            mEmptyCellSigh = ' ';
        }
        if ( stringByTokens >> sign3 || mWallLabSign == mEmptyCellSigh )
        {
            cout << "Wrong first line of input." << endl;
            return false;
        }
    }
    else
    {
        cout << "Cannot read the first line of input." << endl;
        return false;
    }
    return true;
}

void initializeScreen ()
{
    initscr ();
    noecho ();
    curs_set ( FALSE );
    keypad ( stdscr, TRUE );
    nodelay ( stdscr, TRUE );
}

bool CGame :: readMapAndInitializeScreen ( CGameManager & game, ifstream & file )
{
    initializeScreen();
    if ( ! game . readMapFromFile ( file ) )
    {
        endwin();
        cout << "Wrong map." << endl;
        return false;
    }
    return true;
}

void CGame :: ProcessInput ( CGameManager & game )
{
    if ( game . mCharacters . pacmanIsDead() )
    {
        mTimeToDie = true;
        return;
    }

    int key = getch();
    game . mGameField . changeFieldSign ( game . mEmptyCellSign, game . mCharacters . getPacManCoordinates() );
    mNewGame = false;
    pair < size_t, size_t > currPacmanCoordinates = game . mCharacters . getPacManCoordinates();
    mTimeToDie = false;

    switch ( key )
    {
        case KEY_UP:
            currPacmanCoordinates . first -= 1;
            break;
        case KEY_LEFT:
            currPacmanCoordinates . second -= 1;
            break;
        case KEY_DOWN:
            currPacmanCoordinates . first += 1;
            break;
        case KEY_RIGHT:
            currPacmanCoordinates . second += 1;
            break;
        case KEY_BACKSPACE:
            mTimeToDie = true;
            break;
        case '\n':
        case '\r':
            mNewGame = true;
            break;
        case ' ':
            mPause = true;
            break;
        default:
            break;
    }

    if ( game . mGameField . canStepOn ( currPacmanCoordinates . first, currPacmanCoordinates . second ) )
        game . mCharacters . setPacmanCoordinates (currPacmanCoordinates );
    game . mGameField . changeFieldSign ( game . mCharacters . getPacmanSign(), game . mCharacters . getPacManCoordinates() );
}
