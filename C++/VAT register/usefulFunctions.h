#ifndef UL5_USEFULFUNCTIONS_H
#define UL5_USEFULFUNCTIONS_H

#include <string>
#include <unordered_map>
using namespace std;


/* This function was designed to convert any string to its lower case representation and
 * reduce the amount of spaces to 1 between each word */
void createToLowerStringReducedSpaces ( const string & src, string & dst );

/* This function is used to make a lower case representation of the str2 string.
 * In case we have encountered with a new name, it will be stored for future searches */
void replaceOrAdd ( const string & str1, string & str2 );


#endif //UL5_USEFULFUNCTIONS_H
