#ifndef CCOIN_H
#define CCOIN_H
#include "CGameEntity.h"
using namespace std;


/**
 * @brief A subclass to represent one type of CGameEntity class object.
 * If Pacman collects a coin, it will not appear again and increase Pacman score.
 *
 * This class is responsible for increase of Pacman score.
 */
class CCoin : public CGameEntity
{
public:
    CCoin (size_t row, size_t column, char symbol, char wallLabSign );

    CCoin (const pair < size_t, size_t > & coordinates, char symbol, char wallLabSign );

    /**
     * @brief Increases Pacman score.
     * @param pacman is used to provide activation of the power mode.
     * @param field is used for dynamical representation of the game field.
     */
    void action ( CPacman * pacman, vector < string > & field ) override;
};

#endif //CCOIN_H
