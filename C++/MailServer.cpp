#ifndef __TESTING__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __TESTING__ */

/* The task is to implement the CMailServer class, which will simulate the operation of a simple mail server.
 * Each email is represented by an instance of the CMail class, containing a sender address, a recipient address,
 * and a message body. The mail server can send the message and can detect the content of sent and
 * received mail (outbox and inbox) for individual e-mail addresses.
 * Unfortunately, this task does not allow using containers from such libraries are <map> and <vector>, so I decided
 * to create my own version of these quite useful data types.
 * Note: all the method were created in-place cause their sizes are relatively small. */

bool cmpStr ( const char * str1, const char * str2 )
{
    return strcmp ( str1, str2 ) < 0;
}

class CMail
{
public:
    CMail ( void )
            : m_from ( nullptr ), m_to ( nullptr ), m_body ( nullptr )
    {
    }
    CMail ( const char * from, const char * to, const char * body )
            : m_from ( strdup ( from ) ), m_to ( strdup ( to ) ), m_body ( strdup ( body ) )
    {
    }
    CMail ( const CMail & mail )
            : m_from ( strdup ( mail . m_from ) ), m_to ( strdup ( mail . m_to ) ), m_body ( strdup ( mail . m_body ) )
    {
    }

    void freeAllCharStars ( void )
    {
        if ( m_from )
            free ( m_from );
        if ( m_to )
            free ( m_to );
        if ( m_body )
            free ( m_body );
    }

    ~ CMail ( void )
    {
        freeAllCharStars();
    }

    CMail & operator = ( const CMail & mail )
    {
        if ( this == & mail )
            return * this;
        freeAllCharStars();
        m_from = strdup ( mail . m_from );
        m_to = strdup ( mail . m_to );
        m_body = strdup ( mail . m_body );
        return * this;
    }

    bool operator == ( const CMail & x ) const
    {
        return strcmp ( m_from, x . m_from ) == 0 && strcmp ( m_to,x . m_to ) == 0 && strcmp ( m_body, x . m_body ) == 0;
    }
    friend ostream & operator << ( ostream & os, const CMail & m )
    {
        os << "From: " << m . m_from << ", To: " << m . m_to << ", Body: " << m . m_body;
        return os;
    }
    friend class CMailServer;
private:
    char * m_from, * m_to, * m_body;
};

template < typename T >
class CVector {
public:
    CVector ( void )
            : m_data ( nullptr ), m_size ( 0 ), m_capacity ( 0 )
    {
    }

    void allocateAndFulfillCVector ( const CVector < T > & arr )
    {
        m_data = new T [ m_capacity = arr . m_capacity ];
        m_size = arr . m_size;
        for ( size_t i = 0; i < m_size; i ++ )
            m_data [ i ] = arr [ i ];
    }
    CVector ( const CVector < T > & arr )
    {
        allocateAndFulfillCVector ( arr );
    }
    CVector & operator = ( const CVector < T > & arr )
    {
        if ( this == & arr )
            return * this;
        delete [] m_data;
        allocateAndFulfillCVector ( arr );
        return * this;
    }
    ~CVector ( void )
    {
        delete [] m_data;
    }

    T & operator [] ( size_t index )
    {
        return m_data [ index ];
    }

    const T & operator [] ( size_t index ) const
    {
        return m_data [ index ];
    }


    void push_back ( const T & value)
    {
        if ( m_size >= m_capacity )
            reallocateArray ( m_capacity * 2 );
        m_data [ m_size ++ ] = value;
    }

    void reallocateArray ( size_t capacity )
    {
        if ( capacity == 0 )
            capacity = 10;
        T * tmp = new T [ m_capacity = capacity ];
        for ( size_t i = 0; i < m_size; i ++ )
            tmp [ i ] = m_data [ i ];
        delete [] m_data;
        m_data = tmp;
    }

    // in the original vector this method does not exist, but in this task I find it quite useful
    void pop_front ( void )
    {
        if ( m_size == 0 )
            return;
        if ( m_size == 1 )
            -- m_size;
        else
        {
            T * tmp = new T [ m_capacity ];
            for ( size_t i = 1; i < m_size; i ++ )
                tmp [ i - 1 ] = m_data [ i ];
            delete [] m_data;
            m_data = tmp;
            m_size --;
        }
    }

    size_t size ( void ) const
    {
        return m_size;
    }

private:
    T * m_data;
    size_t m_size, m_capacity;
};

struct TNode
{
    char * m_key;
    CVector < CMail > m_value;
    TNode * m_left;
    TNode * m_right;
    TNode ()
            : m_key ( nullptr ), m_value ( CVector < CMail > () ), m_left ( nullptr ), m_right ( nullptr )
    {
    }
    TNode ( const char * key, const CVector < CMail > & value )
            : m_key ( strdup ( key ) ), m_value ( value ), m_left ( nullptr ), m_right ( nullptr )
    {
    }
    explicit TNode ( const TNode * node )
            : m_key ( strdup ( node -> m_key ) ), m_value ( node -> m_value ), m_left ( nullptr ), m_right ( nullptr )
    {
    }
    ~ TNode ()
    {
        if ( m_key )
            free (m_key );
    }
};

// In comparison with the original map, this one is used to store only one data type - CVector of CMail
class CMap
{
public:
    CMap ( void )
    : m_root ( nullptr )
    {
    }

    ~ CMap ( void )
    {
        recursiveDelete ( m_root );
    }

    CMap ( const CMap & map )
    {
        recursiveCopy ( m_root, map . m_root );
    }

    bool insert ( const char * key, const CMail & mail )
    {
        return inner_insert(m_root, key, mail);
    }

    CMap & operator = ( const CMap & map )
    {
        if ( this == & map )
            return * this;
        recursiveDelete ( m_root );
        recursiveCopy ( m_root, map . m_root );
        return * this;
    }

    CVector < CMail > find ( const char * key ) const
    {
        TNode * node = inner_find ( m_root, key);
        CVector < CMail > returning_arr;
        if ( node )
            returning_arr = node -> m_value;
        return returning_arr;
    }

private:
    TNode * m_root;

    bool inner_insert (TNode * & node, const char * key, const CMail & mail )
    {
        if ( ! node )
        {
            CVector < CMail > tmp;
            tmp . push_back ( mail );
            node = new TNode ( key, tmp );
            return true;
        }
        if ( cmpStr ( key, node -> m_key ) )
        {
            return inner_insert(node->m_left, key, mail);
        }
        else if ( cmpStr (node->m_key, key ) )
        {
            return inner_insert(node->m_right, key, mail);
        }
        else
        {
            node -> m_value . push_back (mail );
            return false;
        }
    }

    TNode * inner_find ( TNode * node, const char * key ) const
    {
        if ( ! node )
        {
            return nullptr;
        }
        if ( cmpStr ( key, node -> m_key ) )
        {
            return inner_find ( node -> m_left, key);
        }
        else if ( cmpStr ( node -> m_key, key ) )
        {
            return inner_find ( node -> m_right, key);
        }
        else
        {
            return node;
        }
    }

    void recursiveCopy (TNode *& root, const TNode * src )
    {
        if ( src )
        {
            root = new TNode ( src );
            recursiveCopy (root -> m_left, src -> m_left );
            recursiveCopy (root -> m_right, src -> m_right );
        }
    }

    void recursiveDelete ( TNode * node )
    {
        if  ( node )
        {
            recursiveDelete ( node -> m_left );
            recursiveDelete ( node -> m_right );
            delete node;
        }
    }
};

/* This class represents my version of iterator for CMailServer class.
 * It has some common features as a usual iterator has and some new as well. */
class CMailIterator
{
public:
    CMailIterator ( void ) = default;

    ~CMailIterator ( void ) = default;

    CMailIterator ( const CMailIterator & it )
    {
        m_box = it . m_box;
    }

    CMailIterator & operator = ( const CMailIterator & it )
    {
        m_box = it . m_box;
        return * this;
    }

    explicit CMailIterator ( const CVector < CMail > & arr )
    {
        m_box = arr;
    }

    // These two operators are used to check if the iterator is valid before using it.
    explicit operator bool ( void ) const
    {
        return m_box . size() != 0;
    }

    bool operator ! ( void ) const
    {
        return m_box . size() == 0;
    }

    // According to the given information from the test server, iterator will be checked for validation before using it.
    const CMail & operator * ( void ) const
    {
        return m_box [ 0 ];
    }

    /* In comparison with usual iterator, this one no longer needs previous data information.
       It returns the next email in the mail box */
    CMailIterator & operator ++ ( void )
    {
        m_box . pop_front();
        return * this;
    }
private:
    CVector < CMail > m_box;
};


class CMailServer
{
public:
    CMailServer ( void )
    {
    }

    CMailServer ( const CMailServer & src )
            : m_inbox ( src . m_inbox ), m_outbox ( src . m_outbox )
    {
    }

    CMailServer & operator = ( const CMailServer & src )
    {
        if ( this == & src )
            return * this;
        m_inbox = src . m_inbox;
        m_outbox = src . m_outbox;
        return * this;
    }

    ~CMailServer ( void ) = default;

    void sendMail ( const CMail & m )
    {
        m_inbox . insert ( m . m_to, m );
        m_outbox . insert ( m . m_from, m );
    }

    /* Here is a simple and modified version of iterator for mail ( in / out ) boxes.
     * It returns current usage of the box. In case the box was modified later,
     * these changes would not influence the usage of the iterator. */
    CMailIterator outbox ( const char * email ) const
    {
        return CMailIterator ( m_outbox . find ( email ) );
    }

    CMailIterator inbox ( const char * email ) const
    {
        return CMailIterator ( m_inbox . find ( email ) );
    }

private:
    CMap m_inbox, m_outbox;
};

#ifndef __TESTING__
bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
    ostringstream oss;
    oss << m;
    return oss . str () == str;
}

int main ( void )
{
    char from[100], to[100], body[1024];

    assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
    CMailServer s0;
    s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "thomas", sizeof ( to ) );
    strncpy ( body, "another important mail", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "alice", sizeof ( to ) );
    strncpy ( body, "deadline notice", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );
    s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
    s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
    CMailIterator i0 = s0 . inbox ( "alice" );
    assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i0 );

    CMailIterator i1 = s0 . inbox ( "john" );
    assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
    assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
    assert ( ! ++i1 );

    CMailIterator i2 = s0 . outbox ( "john" );
    assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ! ++i2 );

    CMailIterator i3 = s0 . outbox ( "thomas" );
    assert ( ! i3 );

    CMailIterator i4 = s0 . outbox ( "steve" );
    assert ( ! i4 );

    CMailIterator i5 = s0 . outbox ( "thomas" );
    s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
    assert ( ! i5 );

    CMailIterator i6 = s0 . outbox ( "thomas" );
    assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
    assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
    assert ( ! ++i6 );

    CMailIterator i7 = s0 . inbox ( "alice" );
    s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
    assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i7 );

    CMailIterator i8 = s0 . inbox ( "alice" );
    assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ! ++i8 );

    CMailServer s1 ( s0 );
    s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
    s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
    CMailIterator i9 = s0 . inbox ( "alice" );
    assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ! ++i9 );

    CMailIterator i10 = s1 . inbox ( "alice" );
    assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
    assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
    assert ( ! ++i10 );

    CMailServer s2;
    s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
    CMailIterator i11 = s2 . inbox ( "alice" );
    assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
    assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
    assert ( ! ++i11 );

    s2 = s0;
    s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
    s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
    CMailIterator i12 = s0 . inbox ( "alice" );
    assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
    assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
    assert ( ! ++i12 );

    CMailIterator i13 = s2 . inbox ( "alice" );
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
    assert ( ! ++i13 );

    return EXIT_SUCCESS;
}
#endif /* __TESTING__ */
