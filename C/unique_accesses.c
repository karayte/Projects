#include <stdio.h>
#include <malloc.h>

/*
   The task of this project is to create a program, which will process logs and count unique
   visits.
   Here we will record accesses to a WWW server of a user, which are identified by their unique
   ID represented by a number of Integer type ( 0 - 99999 ). The input can be the following:
   	+ 345
   	+ 586
   	? 0 5
   Where '+' and number means the user connected to the server and '?' and 2 numbers for 'from'
   and 'to' requires at the output the amount of unique accesses from all the accesses.
   The input of the program are accesses and queries, the output of the access will be
   information, given the time the user required the access ( the first, or #(order) visit ),
   and for the queries it will be ( the amount of unique accesses ) / ( the amount of all ).
   This program reports wrong inputs as well.
*/

// This function is used as a cleaning one and returns the given flag. It reports wrong input as well
int returnOutputFlag ( int ** arrayOfId, int ** arrayOfUniqueId, char ** inputLine, int flag )
{
    if ( flag == 0 )
        printf ( "Wrong input.\n" );
    free ( * inputLine );
    free ( * arrayOfUniqueId );
    free ( * arrayOfId );
    return flag;
}

// This function calculates and prints the number of unique id in given interval
void numberOfUniqueAccesses ( int from, int to, int ** arrayOfId )
{
    int countUniqueId = 0;
    int numberOfIdToSearch = to - from;

// In case we have only one id between "from" and "to" positions - given interval
    if ( numberOfIdToSearch == 0 )
        printf ( "> 1 / 1\n" );

// counting how many unique id we have between from and to
    else
    {

// the array for getting information how many unique users will be from position A to position B
        char * arrayForInterval = ( char * ) calloc ( 100000, sizeof ( char ) );
        numberOfIdToSearch ++;

// this cycle counts the number of unique id in the given interval
        for ( int i = 0 ; i < numberOfIdToSearch ; i++ )
        {

// In case of zero we have one more unique id
            if ( arrayForInterval [ ( * arrayOfId ) [ from + i ] ] == 0 )
            {
                arrayForInterval [ ( * arrayOfId ) [ from + i ] ] ++;
                countUniqueId  ++;
            }
        }
        free ( arrayForInterval );
        printf ( "> %d / %d\n", countUniqueId , numberOfIdToSearch );
    }
}

// This function calculates the interval to show how many unique accesses we had
int uniqueAccessesCounter ( int numberOfAccesses, int ** arrayOfId, int from, int to )
{
    if ( from < 0
      || to < from
      || to >= numberOfAccesses )
        return 0;
    numberOfUniqueAccesses ( from, to, arrayOfId );
    return 1;
}

/*
  This function adds a user to the array of users from the correct input
  and prints the number of accesses for each user
*/
int newAccess ( int ** arrayOfId, int ** arrayOfUniqueId, int * numberOfAccesses, int id )
{
    if ( id < 0
      || id > 99999
      || * numberOfAccesses > 999999 )
        return 0;

    ( * arrayOfId ) [ * numberOfAccesses ] = id;
    ( * numberOfAccesses ) ++;

    if ( ( * arrayOfUniqueId ) [ id ] == 0 )
    {
        printf ( "> the first visit\n" );
        ( * arrayOfUniqueId ) [ id ] ++;
    }
    else
    {
        ( * arrayOfUniqueId ) [ id ] ++;
        printf ( "> visit #%d\n", ( * arrayOfUniqueId ) [ id ] );
    }
    return 1;
}

int readInput ( char ** line, ssize_t * input, char * sign, int * firstNumber, int * secondNumber )
{
    ( * input ) = sscanf ( ( * line ), "%c %d %d", sign, firstNumber, secondNumber );
    return ( * input ) != -1;
}

int main ( void )
{
    printf ( "Requirements:\n" );
    char sign;
    int firstNumber = 0, secondNumber = 0;

    char * inputLine = NULL;
    size_t inputLineLength = 0;
    

// for the need of controlling the amount of accesses that must be smaller than 1 000 000
    int numberOfAccesses = 0;

// receiving the first sign from the input: '+' for access or '?' to determine unique id of users
    ssize_t input = getline ( & inputLine, & inputLineLength, stdin );

// the array for saving all the accesses from the input
    int * arrayOfId = ( int * ) calloc ( 1000000, sizeof ( int ) );

// the array for saving information about number of entry for every user
    int * arrayOfUniqueId = ( int * ) calloc ( 99999, sizeof ( int ) );

// receiving information from the input while it is not the end of file or an error
    while ( input != EOF )
    {
        if ( ! readInput ( & inputLine, & input, & sign, & firstNumber, & secondNumber ) )
            return returnOutputFlag ( & arrayOfId, & arrayOfUniqueId, & inputLine, 0 );

        if ( input == 2 && sign == '+' )
        {
            if ( ! newAccess ( & arrayOfId, & arrayOfUniqueId, & numberOfAccesses, firstNumber ) )
                return returnOutputFlag ( & arrayOfId, & arrayOfUniqueId, & inputLine, 0 );

        }
        else if ( input == 3 && sign == '?' )
        {
            if ( ! uniqueAccessesCounter ( numberOfAccesses, & arrayOfId, firstNumber, secondNumber ) )
                return returnOutputFlag ( & arrayOfId, & arrayOfUniqueId, & inputLine, 0 );

        }
        else
            return returnOutputFlag ( & arrayOfId, & arrayOfUniqueId, & inputLine, 0 );

        free ( inputLine );
        inputLine = NULL;
        input = getline ( & inputLine, & inputLineLength, stdin );
    }
    return returnOutputFlag ( & arrayOfId, & arrayOfUniqueId, & inputLine, 1 );
}
