#ifndef CTELEPORT_H
#define CTELEPORT_H
#include "CGameEntity.h"
using namespace std;


/**
 * @brief A subclass to represent one type of CGameEntity class object.
 * If Pacman steps on a teleport, Pacman will be transferred to another teleportation station.
 *
 * This class is responsible for the change of Pacman place.
 */
class CTeleport : public CGameEntity
{
public:

    /**
     * @brief a variable to control locations of all the teleports.
     * Is used for searching for place Pacman will be redirected to.
     */
    static vector < pair < size_t, size_t > > mAllTeleports;
    
    CTeleport (size_t row, size_t column, char symbol, char wallLabSign );

    CTeleport (const pair < size_t, size_t > & coordinates, char symbol, char wallLabSign );

    /**
     * @brief transfers Pacman to another teleportation station.
     * @param pacman is used to provide changes of the location.
     * @param field is used to update changes on the game field.
     */
    void action ( CPacman * pacman, vector < string > & field ) override;
};

#endif // CTELEPORT_H
