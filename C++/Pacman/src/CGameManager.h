#ifndef CHECKPOINT_CGAMEMANAGER_H
#define CHECKPOINT_CGAMEMANAGER_H

#include "CGameMap.h"
#include "CCharacterManager.h"

/**
 * @brief A class to control each character in the game process and
 * the map.
 *
 * This class is responsible for playing actions when Pacman steps
 * on the field with immovable objects, spirits movements and their
 * actions towards Pacman, dynamic change of field and initialization
 * of the game field and characters at the very beginning of the game.
 */
class CGameManager
{
public:

    /**
     * @brief Constructor to create an object.
     *
     * @param EmptyCellSign represents a place on the game field
     * which has not been taken and is free to step on.
     * @param WallLabyrinthSigh represent a piece of wall or labyrinth
     * on the game field.
     */
    CGameManager ( char EmptyCellSign, char WallLabyrinthSigh );

    /**
     * @brief Initializes all the game characters and the game field.
     *
     * @param file represents a file all the information towards game
     * should be taken from.
     */
    bool readMapFromFile ( istream & file );

    /**
     * @brief Activates actions of each immovable character if Pacman
     * steps on their cell during the game.
     */
    void checkIfGameEntitiesGotEaten ();
    /**
     * @brief Is responsible for movements of each spirits and checks if
     * Pacman got caught by a spirit or spirit by Pacman depending on
     * whether the power mode was activated.
     */
    void makeSpiritsMove ();
    /**
     * @brief Is used for a new game initialization. It deletes the
     * current information towards all the characters, so the game
     * can be started from the very beginning.
     */
    void reset ();

    /**
     * @brief Is used for representation of the current state of game field.
     * It also provides player the information towards how the player can
     * control the characters and how the game process can be influenced.
     */
    void printGameField () const;
    /**
     * @brief A public variable is responsible for all the characters movements
     * and actions. Is used in the main function as well for enable player's
     * participation in the game process.
     */
    CCharacterManager mCharacters;
    /**
     * @brief A public variable is responsible for the game field and its
     * dynamic changes. Enables player to see the current state of the game
     * field and make changes on it moving Pacman.
     */
    CGameMap mGameField;
    char mEmptyCellSign;
};


#endif //CHECKPOINT_CGAMEMANAGER_H
