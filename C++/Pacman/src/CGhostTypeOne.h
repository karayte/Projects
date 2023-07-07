#ifndef CGHOSTTYPEONE_H
#define CGHOSTTYPEONE_H
#include "CGhost.h"
using namespace std;

/**
 * @brief A subclass to represent the spirit which follows Pacman.
 *
 * This class is responsible for chasing Pacman to catch and eat.
 */
class CGhostTypeOne : public CGhost {
public:
    using CGhost::CGhost;

    /**
     * @brief Uses BFS method from the parent to chase Pacman and
     * change spirit's position.
     *
     * @param fieldWidth is used to represent the maximum of x
     * coordinate value on the game field.
     * @param fieldHeight is used to represent the maximum of y
     * coordinate value on the game field.
     * @param field represents the game field.
     */
    void move ( size_t fieldWidth, size_t fieldHeight, vector < string > & field ) override;
};

#endif // CGHOSTTYPEONE_H
