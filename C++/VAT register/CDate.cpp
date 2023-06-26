#include "CDate.h"

int CDate :: compare ( const CDate & x ) const
{
    if ( mYear != x . mYear )
        return mYear - x . mYear;
    if ( mMonth != x . mMonth )
        return mMonth - x . mMonth;
    return mDay - x . mDay;
}

//---------------------------------------------------------------------------------------------
 int CDate :: getYear ( ) const
{
    return mYear;
}

//---------------------------------------------------------------------------------------------
int CDate :: getMonth ( ) const
{
    return mMonth;
}

//---------------------------------------------------------------------------------------------
int CDate :: getDay ( ) const
{
    return mDay;
}

//---------------------------------------------------------------------------------------------
ostream & operator << ( ostream & os, const CDate & x )
{
    char oldFill = os . fill ( );
    return os << setfill ( '0' ) << setw ( 4 ) << x . mYear << "-"
              << setw ( 2 ) << static_cast<int> ( x . mMonth ) << "-"
              << setw ( 2 ) << static_cast<int> ( x . mDay )
              << setfill ( oldFill );
}