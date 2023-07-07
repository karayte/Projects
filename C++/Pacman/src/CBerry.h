#ifndef CBERRY_H
#define CBERRY_H
#include "CGameEntity.h"
#include "CGameMap.h"
using namespace std;

/**
 * @brief A subclass to represent one type of CGameEntity class object.
 * If Pacman eats a berry, it will not appear again and allows Pacman to eat spirits within 5 seconds.
 *
 * This class is responsible for activation of Pacman Power Mode.
 */

class CBerry : public CGameEntity
{
public:

    CBerry ( size_t row, size_t column, char sign, char wallLabSign );

    CBerry ( const pair < size_t, size_t > & coordinates, char sign, char wallLabSign );
    /**
    * @brief Activates Pacman Power Mode which allows eating spirits.
     * @param pacman is used to provide activation of the power mode.
     * @param field is used for dynamical representation of the game field.
    */
    void action ( CPacman * pacman, vector < string > & field ) override;
};

#endif //CBERRY_H
