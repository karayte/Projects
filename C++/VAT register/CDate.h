#ifndef UL5_CDATE_H
#define UL5_CDATE_H


#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace std;

/* This class is a simple container for getDate type */
class CDate
{
public:
    CDate ( int16_t y, int8_t m, int8_t d )
            : mYear ( y ),
              mMonth ( m ),
              mDay ( d )
    {
    }

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] int compare ( const CDate & x ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] int getYear ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] int getMonth ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] int getDay ( ) const;

    //---------------------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & os, const CDate & x );
    //---------------------------------------------------------------------------------------------
private:
    int16_t mYear;
    int8_t mMonth;
    int8_t mDay;
};


#endif //UL5_CDATE_H
