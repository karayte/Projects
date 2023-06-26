#include "usefulFunctions.h"

/* This map is used to store all known names of the companies.
 * As the name must be different from the others by its symbols/letters,
 * we see the name Ackee is the same as ackEE and so on. To find it relatively
 * quickly, we use this map */
unordered_map < string, string > gArrayOfValidNames;

void createToLowerStringReducedSpaces ( const string & src, string & dst )
{
    bool lastCharWasSpace = true;
    dst . clear ( );
    dst . reserve ( src . length ( ) );
    for ( const auto & c : src )
    {
        if ( isspace ( c ) )
        {
            if ( ! lastCharWasSpace )
                dst . push_back ( ' ' );
            lastCharWasSpace = true;
        }
        else
        {
            dst . push_back ( ( char ) tolower ( static_cast < unsigned char > ( c ) ) );
            lastCharWasSpace = false;
        }
    }

    if ( ! dst . empty ( ) && dst . back ( ) == ' ' )
        dst . pop_back ( );
}

// ----------------------------------------------------------------------------------------

void replaceOrAdd ( const string & str1, string & str2 )
{
    if ( gArrayOfValidNames . find ( str1 ) != gArrayOfValidNames . end ( ) )
        str2 = gArrayOfValidNames[ str1 ];
    else
    {
        createToLowerStringReducedSpaces ( str1, str2 );
        gArrayOfValidNames[ str1 ] = str2;
    }
}
