#ifndef CGHOSTTYPETHREE_H
#define CGHOSTTYPETHREE_H
#include "CGhost.h"
using namespace std;


/**
 * @brief A subclass to represent the spirit which moves randomly.
 *
 * This class is responsible for catching Pacman using unpredictable
 * movements to confuse Pacman.
 */
class CGhostTypeThree : public CGhost {
public:
    using CGhost::CGhost;

    /**
     * @brief Randomly chooses the next position of the spirit
     * and changes its coordinates.
     *
     * @param fieldWidth is used to represent the maximum of x
     * coordinate value on the game field.
     * @param fieldHeight is used to represent the maximum of y
     * coordinate value on the game field.
     * @param field represents the game field.
     */
    void move (size_t fieldWidth, size_t fieldHeight, vector < string > & field ) override;
};

#endif // CGHOSTTYPETHREE_H
