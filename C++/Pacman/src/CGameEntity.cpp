#include "CGameEntity.h"

CGameEntity::CGameEntity( pair < size_t, size_t > coordinates, char sign, char wall_lab_sign )
        : mCoordinates (move (coordinates ) ),
          mSign (sign ),
          mWallLabyrinthSign (wall_lab_sign )
{
}

pair < size_t, size_t > CGameEntity :: getCoordinates () const
{
    return mCoordinates;
}
char CGameEntity :: getSign () const
{
    return mSign;
}

void CGameEntity :: setCoordinates ( pair < size_t, size_t > coordinates )
{
    mCoordinates = coordinates;
}
