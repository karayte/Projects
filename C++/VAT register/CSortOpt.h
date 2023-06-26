#ifndef UL5_CSORTOPT_H
#define UL5_CSORTOPT_H

#include <utility>
#include <vector>

using namespace std;

/* This class specifies the sorting criteria. Each invoice content can be used for sorting. */
class CSortOpt
{
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;
    vector < pair < int, bool > > mSortingOptions;

    //---------------------------------------------------------------------------------------------
    CSortOpt ( ) = default;

    //---------------------------------------------------------------------------------------------
    CSortOpt ( const CSortOpt & sort )
    {
        mSortingOptions . reserve ( sort . mSortingOptions . size ( ) );
        for ( auto & it : sort . mSortingOptions )
            mSortingOptions . push_back ( it );
    }

    //---------------------------------------------------------------------------------------------
    CSortOpt ( CSortOpt && sort ) noexcept
            : mSortingOptions ( move ( sort . mSortingOptions ) )
    {
    }

    /* This function adds another sorting option sortBy, the sorting direction is given by
     * the ascending flag (true = ascending, false = descending). */
    CSortOpt & addKey ( int key, bool ascending = true );

    //---------------------------------------------------------------------------------------------
    CSortOpt & operator = ( const CSortOpt & sort );

};


#endif //UL5_CSORTOPT_H
