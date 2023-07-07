#ifndef CHECKPOINT_CCHARACTERMANAGER_H
#define CHECKPOINT_CCHARACTERMANAGER_H
#include <iostream>
#include "CGameEntity.h"
#include "CCoin.h"
#include "CBerry.h"
#include "CTeleport.h"
#include "CGhostTypeOne.h"
#include "CGhostTypeTwo.h"
#include "CGhostTypeThree.h"

#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

#define SPIRITS_VULNERABLE_MODE_SIGN '?'

using namespace std;

/**
 * @brief A class for handling all the game characters.
 *
 * Is responsible for handling all the changes and dynamic
 * representation of game characters during the game process.
 */
class CCharacterManager
{
public:
    /**
     * @brief Constructor for creating an object.
     *
     * @param emptyCellSign represents a piece of game field
     * which is not taken by any of the characters and walls or
     * labyrinth.
     * @param wallLabyrinthSign represents the sign of a piece of
     * wall or labyrinth.
     */
    CCharacterManager ( char emptyCellSign, char wallLabyrinthSign );

    /**
     * @brief initializes the characters from the input file and
     * checks their correctness.
     *
     * @param file represents the input file with the information
     * towards all the characters.
     * @param allCharactersType represents the array with all the
     * types of the characters.
     *
     * @return false in case the input file has incorrect information
     * about the characters, otherwise is true.
     */
    bool readCharacters ( istream & file, const vector < string > & allCharactersType );

    /**
     * @brief is used to proceed the character and calls initializeOrInsertCharacters
     * to store it.
     *
     * @param file represents the input file with the information
     * towards all the characters.
     * @param type represents the type of given character.
     *
     * @return false in case the input file has incorrect information
     * about the character, otherwise is true.
     */
    bool initializeCharactersFromInput ( istream & file, const string & type );

    /**
     * @brief is used to create the character and calls initializeOrInsertCharacters
     * to store them.
     *
     * @param type represents the type of given character.
     * @param coordinates represents the coordinates of given character.
     * @param represents the sign of given character.
     */
    void initializeOrInsertCharacters ( const string & type, pair < size_t, size_t > && coordinates, char symbol );

    /**
     * @brief is used to erase the information about the characters and
     * create a new game.
     */
    void reset ();

    /**
     * @brief is used to push each character's coordinates into the input
     * vector in order to check their coordinates for invalid ones.
     *
     * @param allCoordinates is vector to store the coordinates of each
     * character.
     */
    void getAllCharactersCoordinates ( vector < pair < size_t, size_t > > & allCoordinates ) const;

    /**
     * @brief is used to push each entity's information including sign
     * and coordinates in order to print them on the game field.
     *
     * @param allEntities to store a pair of sign and coordinates for
     * each entity.
     */
    void getAllEntitiesSignAndCoordinates ( vector < pair < char, pair < size_t, size_t > > > & allEntities ) const;

    /**
     * @brief
     */
    void setPacmanCoordinates ( const pair < size_t, size_t > & coordinates );

    /**
     * @brief
     */
    pair < size_t, size_t > getPacManCoordinates () const;

    /**
     * @brief
     */
    char getPacmanSign () const;

    /**
     * @brief checks if Pacman is still alive or not.
     *
     * @return true in case Pacman is gone, false otherwise.
     */
    bool pacmanIsDead() const;

    /**
     * @brief checks if Pacman has active Power Mode.
     *
     * @return true if the Power Mode is active, false otherwise.
     */
    bool pacmanPowerMode ();

    /**
     * @brief returns Pacman score.
     */
    int getScore () const;


    /**
     * @brief handles movements of all the spirits and
     * checks if a spirit got eaten by Pacman or Pacman by
     * spirit using checkIfSpiritsAreAtPacmanPlace.
     *
     * @param gameField represents the current state of game
     * field and is used to show dynamic changes on it.
     */
    void makeSpiritsMove ( vector < string > & gameField );

    /**
     * @brief changes the behavior of all the spirits
     * depending on whether the Power Mode is active
     * or not.
     *
     * @param mode indicates if the mode is active or not.
     */
    void setSpiritsVulnerableMode ( bool mode );

    /**
     * @brief checks whether Pacman and a spirit have the
     * same coordinates so one can be eaten by the other one.
     */
    void checkIfSpiritsAreAtPacmanPlace ();

    /**
     * @brief checks whether Pacman took the advantage of
     * any of the entities by stepping on the same place
     * where the entity is.
     *
     * @param gameField represents the current state of game
     * field and is used to show dynamic changes on it.
     */
    void checkIfGameEntitiesGotEaten ( vector < string > & gameField );

private:
    CPacman mPacman;
    vector < unique_ptr < CGameEntity > > mAllEntities;
    vector < unique_ptr < CGhost > > mAllSpirits;
    char mSpiritsVulnerableModeSign;
    /**
     * @brief is used to store the information towards signs
     * of all the characters in order to check for duplicate signs
     * during reading the input and initialization.
     */
    set < char > mAllCharactersSigns;
    char mEmptyCellSign;
    char mWallLabyrinthSigh;
};


#endif //CHECKPOINT_CCHARACTERMANAGER_H
