#include "CGame.h"

int main ()
{
    CGame gameProcess ( "src/mapComplete.txt" );
    if ( ! gameProcess . openFileAndGetSign () )
    {
        return -1;
    }
    return 0;
}
