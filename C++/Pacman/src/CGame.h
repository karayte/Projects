#ifndef CHECKPOINT_CGAME_H
#define CHECKPOINT_CGAME_H

#include "CGameManager.h"
#define DEFAULT_EMPTY_SIGN ' '
#define DEFAULT_WALL_SIGN '#'
#define SPIRITS_SPEED 5
#define SPIRITS_SPEED_FAST_MODE 2
#define SCORE_TO_UPDATE_SPIRITS_SPEED 200

/**
 * @brief A class to handle the game process including reading
 * the map and initializing each character and map.
 *
 */
class CGame
{
public:

    /**
     * @brief Constructor for creating an object.
     *
     * @param name represents the name of the file the input
     * will be reading from.
     */
    CGame ( string name );
    /**
     * @brief Opens the file with given name and initializes
     * sign of empty cell and a piece of wall and labyrinth.
     * It also starts the game itself.
     */
    bool openFileAndGetSign ();
    /**
     * @brief Calculates the results and shows the board of
     * leaders.
     */
    void showGameResults ();
    /**
     * @brief Handles the whole game process including
     * initialization of the game map, characters and
     * starting a new game.
     * @param file is the opened file the information towards
     * the game is taken from.
     * @return false when there are problems with the initialization
     * of each character and the game field, otherwise is true.
     */
    bool runTheGame ( ifstream & file );
    /**
     * @brief Is responsible for running one cycle of the
     * game which includes collecting information from
     * the player and moving objects.
     *
     * @param game represents game characters and the game field.
     *
     * @return false in case there are problems with the input
     * file, otherwise is true.
     */
    bool runOneGameCycle ( CGameManager & game );
    /**
     * @brief Handles the pause player asked for and runs the game
     * again when player asks for it.
     *
     * @param game represents game characters and the game field.
     * Here is used to print the game field during pause.
     */
    void checkPauseState ( CGameManager & game );
    /**
     * @brief Handles the game steps including players input and
     * movements of the characters.
     *
     * @param game represents game characters and the game field.
     *
     * @return false in case there are problems with the input
     * file, otherwise is true.
     */
    bool gameCycle ( CGameManager & game );
    /**
     * @brief Is used in case of creating a new game = game with
     * the same params from the input file = from the very beginning.
     * It calls the reset function to allow the game field be brought
     * to the default ( beginning ) state and all the characters in it.
     * @param game represents game characters and the game field.
     *
     * @return false in case there are problems with the input
     * file, otherwise is true.
     */
    bool openFileRereadMap ( CGameManager & game );
    /**
     * @brief Is responsible for reading the first line of the input
     * file as well as checks the correctness of it.
     *
     * @param gameMap represents the input file with all the
     * information towards the game.
     *
     * @return false in case there is a problem with getting the line
     * or the line does not correspond the conditions, otherwise is true.
     */
    bool readFirstLine ( ifstream & gameMap );
    /**
     * @brief Is responsible for the map initialization and all the
     * characters in it. It also initializes the screen so the player
     * can see the game running.
     *
     * @param game represents game characters and the game field.
     * @param file represent the input file the information towards
     * the game should be taken from.
     *
     * @return false in case there are problems with the input
     * file, otherwise is true.
     */
    bool readMapAndInitializeScreen ( CGameManager & game, ifstream & file );
    /**
     * @brief Collects and processes the information from the player's input.
     * @param game represents game characters and the game field.
     */
    void ProcessInput ( CGameManager & game );

private:
    string mFileName;
    /**
     * @brief Stores the information towards each player's score.
     */
    vector < pair < int, string > > mLeadersBoard;
    char mEmptyCellSigh, mWallLabSign;
    /**
     * @brief Boolean variables indicate whether the action
     * ( mNewGame - if player wants to starts a new game )
     * ( mTimeToDie - if Pacman was eaten or the player want
     * to quit )
     * ( mPause - whether the game is paused )
     */
    bool mNewGame, mTimeToDie, mPause;
    /**
     * @brief Is used to identify players by numbers.
     */
    int mNumOfPlayer;
    /**
     * @brief Is used to handle the speed of spirits movements.
     */
    size_t mCurrStep;
};


#endif //CHECKPOINT_CGAME_H
