#ifndef CGAMEENTITY_H
#define CGAMEENTITY_H

#include <string>
#include <random>
#include <algorithm>
#include "CPacman.h"

#define SCORE 10
#define DEFAULT_ROW -1
#define DEFAULT_COLUMN -1

using namespace std;


/**
 * @brief An abstract class to represent one immovable character on the game field.
 *
 * This class is responsible for updating Pacman power during the game.
 */
class CGameEntity {
public:
    CGameEntity ( pair < size_t, size_t > coordinates, char sign, char wall_lab_sign );

    /**
     * @brief Is implemented by each inheritance class separately.
     *
     * @param pacman is used to make changes on the character.
     * @param field is used to represent dynamic changes of the game field.
     */
    virtual void action ( CPacman * pacman, vector < string > & field ) = 0;
    pair < size_t, size_t > getCoordinates () const;
    char getSign () const;
    void setCoordinates ( pair < size_t, size_t > coordinates );

protected:
    pair < size_t, size_t > mCoordinates;
    char mSign;
    /**
     * @brief is used for detection of a wall or labyrinth on the game field.
     */
    char mWallLabyrinthSign;
};

#endif //CGAMEENTITY_H
