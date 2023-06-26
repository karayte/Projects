#include "CSortOpt.h"

CSortOpt & CSortOpt :: addKey ( int key, bool ascending )
{
    if ( key >= 0 && key <= 4 )
    {
        if ( ! mSortingOptions . empty ( ) )
        {
            for ( const auto & it : mSortingOptions )
            {
                if ( it . first == key )
                    return * this;
            }
        }
        mSortingOptions . emplace_back ( key, ascending );
    }
    return * this;
}

//---------------------------------------------------------------------------------------------
CSortOpt & CSortOpt :: operator = ( const CSortOpt & sort )
{
    if ( this == & sort )
        return * this;
    mSortingOptions . clear ( );
    for ( auto & it : sort . mSortingOptions )
        mSortingOptions . push_back ( it );
    return * this;
}