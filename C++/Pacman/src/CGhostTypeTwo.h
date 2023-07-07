#ifndef CGHOSTTYPETWO_H
#define CGHOSTTYPETWO_H
#include "CGhost.h"

using namespace std;


/**
 * @brief A subclass to represent the spirit which follows Pacman.
 *
 * This class is responsible for chasing Pacman to catch and eat.
 * This type of the spirits tries to predict Pacman next move and
 * help following type to lock Pacman and catch it.
 */
class CGhostTypeTwo : public CGhost
{
public:
    using CGhost :: CGhost;

    /**
     * @brief Uses BFS method from the parent to chase Pacman,
     * predicts its next position and change the spirit's one.
     *
     * @param fieldWidth is used to represent the maximum of x
     * coordinate value on the game field.
     * @param fieldHeight is used to represent the maximum of y
     * coordinate value on the game field.
     * @param field represents the game field.
     */
    void move (size_t fieldWidth, size_t fieldHeight, vector < string > & field ) override;
};

#endif // CGHOSTTYPETWO_H