#ifndef __TESTING__
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * The task is to implement a set of functions for working with linked lists.
 * The linked list represents the list of employees in the company. The employees are represented by name
 * and reference to their representative. With such list of employees, we want to perform the following
 * operations: add a worker, copy the list and delete the list.
 * */

/* The structure represents one employee. Employees are stored in the form of a one-way chained linked list.
   The structure has the following components:
        mNext - link to the next employee in the linked list. The last employee in the list has the mNext reference
                set to NULL.
        mEmployer - link to employee - representative. The reference is either NULL ( then the shortcut does not exist )
                    or it refers to some linked list element. In an extreme case, it can refer to itself if, within
                    the savings, the employee is his own crowd.
        mName - ASCIIZ ( zero-terminated ) string indicating the name of the employee. */
typedef struct TEmployee
{
    struct TEmployee * mNext;
    struct TEmployee * MEmployer;
    char             * mName;
} TEMPLOYEE;

#endif /* __TESTING__ */

/* Implemented in std libraries strcpy() function does not work as expected for this task */
void mystrcpy ( char ** newName, const char * originalName )
{
    size_t size = strlen ( originalName );
    size_t i = 0;
    while ( i < size )
    {
        ( * newName ) [ i ] = originalName [ i ];
        i++;
    }
    ( * newName ) [ size ] = '\0';
}

/* This function is used to create and insert a new employee */
TEMPLOYEE * newEmployee ( const char * name, TEMPLOYEE * next )
{
    TEMPLOYEE * newEmployee = ( TEMPLOYEE * ) malloc ( sizeof ( * newEmployee ) );
    newEmployee -> mNext = next;
    newEmployee -> MEmployer = NULL;
    if ( name )
    {
        size_t size = strlen ( name ) + 1;
        newEmployee -> mName = ( char * ) malloc (size * sizeof ( * newEmployee -> mName ) );
        mystrcpy ( & ( newEmployee-> mName ), name );
    }
    else
        newEmployee -> mName = NULL;
    return newEmployee;
}

/* This function helps to clone the list and is responsible for cloning mNext employee in the list */
void cloneMNext ( TEMPLOYEE * head, TEMPLOYEE * n )
{
    if ( head )
    {
        if ( head -> mNext )
            n -> mNext = ( TEMPLOYEE * ) malloc ( sizeof ( * n -> mNext ) );
        else
            n -> mNext = NULL;
        n -> MEmployer = NULL;
        if ( head -> mName )
        {
            int size = ( int ) strlen ( head -> mName ) + 1;
            n -> mName = ( char * ) malloc ( size * sizeof ( char ) );
            mystrcpy ( & ( n -> mName ),  head -> mName );
        }
        else
            n -> mName = NULL;

        cloneMNext ( head -> mNext, n -> mNext );
    }
}

/* This function returns the number of elements ( employees ) between selected employees in the list */
int countElementsBetween ( TEMPLOYEE * head, TEMPLOYEE * tail )
{
    int count = 0;
    while ( head != tail )
    {
        head = head -> mNext;
        count++;
    }
    return count;
}

/* This function helps to clone the list and is responsible for cloning mEmployer employer in the list */
void cloneMEmployer ( TEMPLOYEE * start_head, TEMPLOYEE * start_new, TEMPLOYEE * head, TEMPLOYEE * ne )
{
    if ( head )
    {
        if ( head -> MEmployer )
        {
            TEMPLOYEE * tmp = start_head;
            int destination = countElementsBetween ( tmp, head -> MEmployer );
            tmp = start_new;
            for ( int i = 0; i < destination; i++ )
                tmp = tmp -> mNext;
            ne -> MEmployer = tmp;
        }
        cloneMEmployer ( start_head, start_new, head -> mNext, ne -> mNext );
    }
}

/* This function is used to deeply copy the usage of one list into another.
 * The first one remains unchanged */
TEMPLOYEE  * cloneList ( TEMPLOYEE * src )
{
    if ( src )
    {
        TEMPLOYEE * clone = ( TEMPLOYEE * ) malloc ( sizeof ( * clone ) );
        cloneMNext ( src, clone );
        cloneMEmployer ( src, clone, src, clone );
        return clone;
    }
    return NULL;
}

/* Cleaning function */
void freeList ( TEMPLOYEE * src )
{
    if ( src )
    {
        if ( src -> mName )
            free ( src -> mName );
        if( src -> mNext )
            freeList ( src -> mNext);
        free ( src );
    }
}

/* This part of the program is used for testing only */
#ifndef __TESTING__
int main ( int argc, char * argv [] )
{
    TEMPLOYEE * a, *b;
    char tmp[100];

    assert ( sizeof ( TEMPLOYEE ) == 3 * sizeof ( void * ) );
    a = NULL;
    a = newEmployee ( "Ivan", a );
    a = newEmployee ( "Johnny", a );
    a = newEmployee ( "Amelia", a );
    a = newEmployee ( "Greta", a );
    a -> MEmployer = a -> mNext;
    a -> mNext -> mNext -> MEmployer = a -> mNext -> mNext -> mNext;
    a -> mNext -> mNext -> mNext -> MEmployer = a -> mNext;
    assert ( a
             && ! strcmp (a -> mName, "Greta" )
             && a -> MEmployer == a -> mNext );
    assert ( a -> mNext
             && ! strcmp (a -> mNext -> mName, "Amelia" )
             && a -> mNext -> MEmployer == NULL );
    assert ( a -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mName, "Johnny" )
             && a -> mNext -> mNext -> MEmployer == a -> mNext -> mNext -> mNext );
    assert ( a -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mName, "Ivan" )
             && a -> mNext -> mNext -> mNext -> MEmployer == a -> mNext );
    assert (a -> mNext -> mNext -> mNext -> mNext == NULL );
    b = cloneList ( a );
    a = newEmployee ( "Daniel", a );
    a = newEmployee ( "Victoria", a );
    strncpy ( tmp, "Ivan", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    b -> mNext -> mNext -> mNext -> MEmployer = b -> mNext -> mNext;
    assert ( a
             && ! strcmp (a -> mName, "Ivan" )
             && a -> MEmployer == NULL );
    assert ( a -> mNext
             && ! strcmp (a -> mNext -> mName, "Victoria" )
             && a -> mNext -> MEmployer == NULL );
    assert ( a -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mName, "Daniel" )
             && a -> mNext -> mNext -> MEmployer == NULL );
    assert ( a -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mName, "Greta" )
             && a -> mNext -> mNext -> mNext -> MEmployer == a -> mNext -> mNext -> mNext -> mNext );
    assert ( a -> mNext -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mNext -> mName, "Amelia" )
             && a -> mNext -> mNext -> mNext -> mNext -> MEmployer == NULL );
    assert ( a -> mNext -> mNext -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mNext -> mNext -> mName, "Johnny" )
             && a -> mNext -> mNext -> mNext -> mNext -> mNext -> MEmployer == a -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext );
    assert ( a -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext -> mName, "Ivan" )
             && a -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext -> MEmployer == a -> mNext -> mNext -> mNext -> mNext );
    assert (a -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext -> mNext == NULL );
    assert ( b
             && ! strcmp (b -> mName, "Greta" )
             && b -> MEmployer == b -> mNext );
    assert ( b -> mNext
             && ! strcmp (b -> mNext -> mName, "Amelia" )
             && b -> mNext -> MEmployer == NULL );
    assert ( b -> mNext -> mNext
             && ! strcmp (b -> mNext -> mNext -> mName, "Johnny" )
             && b -> mNext -> mNext -> MEmployer == b -> mNext -> mNext -> mNext );
    assert ( b -> mNext -> mNext -> mNext
             && ! strcmp (b -> mNext -> mNext -> mNext -> mName, "Ivan" )
             && b -> mNext -> mNext -> mNext -> MEmployer == b -> mNext -> mNext );
    assert (b -> mNext -> mNext -> mNext -> mNext == NULL );
    freeList ( a );
    b -> mNext -> MEmployer = b -> mNext;
    a = cloneList ( b );
    assert ( a
             && ! strcmp (a -> mName, "Greta" )
             && a -> MEmployer == a -> mNext );
    assert ( a -> mNext
             && ! strcmp (a -> mNext -> mName, "Amelia" )
             && a -> mNext -> MEmployer == a -> mNext );
    assert ( a -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mName, "Johnny" )
             && a -> mNext -> mNext -> MEmployer == a -> mNext -> mNext -> mNext );
    assert ( a -> mNext -> mNext -> mNext
             && ! strcmp (a -> mNext -> mNext -> mNext -> mName, "Ivan" )
             && a -> mNext -> mNext -> mNext -> MEmployer == a -> mNext -> mNext );
    assert (a -> mNext -> mNext -> mNext -> mNext == NULL );
    assert ( b
             && ! strcmp (b -> mName, "Greta" )
             && b -> MEmployer == b -> mNext );
    assert ( b -> mNext
             && ! strcmp (b -> mNext -> mName, "Amelia" )
             && b -> mNext -> MEmployer == b -> mNext );
    assert ( b -> mNext -> mNext
             && ! strcmp (b -> mNext -> mNext -> mName, "Johnny" )
             && b -> mNext -> mNext -> MEmployer == b -> mNext -> mNext -> mNext );
    assert ( b -> mNext -> mNext -> mNext
             && ! strcmp (b -> mNext -> mNext -> mNext -> mName, "Ivan" )
             && b -> mNext -> mNext -> mNext -> MEmployer == b -> mNext -> mNext );
    assert (b -> mNext -> mNext -> mNext -> mNext == NULL );
    freeList ( b );
    freeList ( a );
    return 0;
}
#endif /* __TESTING__ */
