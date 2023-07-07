//============================================================================
// Name        : Server.cpp
// Author      : Karolina Zegeryte
// Version     : xx7
// Copyright   : read only
// Description : TCP server - beta version
//============================================================================

#include <iostream>

using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <unordered_map>
#include <vector>
#include <deque>
#include <wait.h> // waitpid()
#include <sstream>
#include <string>


#define BUFFER_SIZE 10240
#define TIMEOUT 1
#define TIMEOUT_RECHARGING 5
#define CLIENT_USERNAME_LENGTH 18
#define CLIENT_CONFIRMATION_LENGTH 5
#define CLIENT_OK_LENGTH 10
#define CLIENT_MESSAGE_LENGTH 98
#define DEFAULT_X (-1)
#define DEFAULT_Y (-1)

void getPairForKey ( int key, int & value, bool first )
{
    const static vector < pair < int, int > > all_pairs_for_keys = {
            { 23019, 32037 },
            { 32037, 29295 },
            { 18789, 13603 },
            { 16443, 29533 },
            { 18189, 21952 }
    };
    if ( first )
        value = all_pairs_for_keys[ key ] . first;
    else
        value = all_pairs_for_keys[ key ] . second;
}

void checkSending ( int c, const string & src )
{
    if ( send ( c, src . c_str ( ), src . length ( ), MSG_NOSIGNAL ) < 0 )
    {
        close ( c );
        throw exception ( );
    }
}

void serverLoginFailed ( int c )
{
    checkSending ( c, "300 LOGIN FAILED\a\b" );
    close ( c );
    throw exception ( );
}

void serverSyntaxError ( int c )
{
    checkSending ( c, "301 SYNTAX ERROR\a\b" );
    close ( c );
    throw exception ( );
}

void serverLogicError ( int c )
{
    checkSending ( c, "302 LOGIC ERROR\a\b" );
    close ( c );
    throw exception ( );
}

void serverKeyOutOfRangeError ( int c )
{
    checkSending ( c, "303 KEY OUT OF RANGE\a\b" );
    close ( c );
    throw exception ( );
}


class CInput
{
public :
    string readDataAndReturnNextStr ( int c, const string & stage );

    void receiveDataFromSocket ( int c, const string & stage );

    void addNewDataToInputQueue ( );

    void fulfillInputDeque ( int c, const string & stage );

    void initializeSockets ( int c );

    void initializeTimeout ( bool recharging );

    string recharge ( int c );


private :
    deque < string > m_input_queue;
    string m_resting_input = ""s;
    fd_set m_sockets;
    struct timeval m_timer;
};

void CInput::initializeSockets ( int c )
{
    FD_ZERO ( & m_sockets );
    FD_SET ( c, & m_sockets );
}

void CInput::fulfillInputDeque ( int c, const string & stage )
{
    while ( m_input_queue . empty ( ) )
    {
        if ( ( stage == "username" && m_resting_input . length ( ) > CLIENT_USERNAME_LENGTH )
             || ( stage == "ok message" && m_resting_input . length ( ) > CLIENT_OK_LENGTH )
             || ( stage == "message" && m_resting_input . length ( ) > CLIENT_MESSAGE_LENGTH ) )
            serverSyntaxError ( c );
        receiveDataFromSocket ( c, stage );
    }
}

string CInput::readDataAndReturnNextStr ( int c, const string & stage )
{
    fulfillInputDeque ( c, stage );

    if ( m_input_queue . front ( ) == "RECHARGING" )
    {
        m_input_queue . pop_front ( );
        if ( stage != "recharge" )
            return recharge ( c );
    }

    string top_message = m_input_queue . front ( );
    m_input_queue . pop_front ( );
    if ( stage == "username" && top_message . length ( ) > CLIENT_USERNAME_LENGTH
         || ( stage == "confirmation" &&
              ( top_message . find ( ' ' ) != string::npos || top_message . length ( ) > CLIENT_CONFIRMATION_LENGTH ) )
         || ( stage == "ok message" && ( top_message . length ( ) > CLIENT_OK_LENGTH ||
                                         ! isdigit ( top_message[ top_message . length ( ) - 1 ] ) ) ) )
        serverSyntaxError ( c );
    return top_message;
}

string CInput::recharge ( int c )
{
    string stage = "recharge";
    string str = readDataAndReturnNextStr ( c, stage );

    if ( str != "FULL POWER" )
        serverLogicError ( c );
    return readDataAndReturnNextStr ( c, stage );
}

void CInput::initializeTimeout ( bool recharging )
{
    if ( recharging )
        m_timer . tv_sec = TIMEOUT_RECHARGING;
    else
        m_timer . tv_sec = TIMEOUT;
    m_timer . tv_usec = 0;
}

void CInput::receiveDataFromSocket ( int c, const string & stage )
{
    char input_str[BUFFER_SIZE];
    initializeSockets ( c );
    if ( stage == "recharge" )
        initializeTimeout ( true );
    else
        initializeTimeout ( false );
    if ( select ( c + 1, & m_sockets, nullptr, nullptr, & m_timer ) <= 0 || ! FD_ISSET ( c, & m_sockets ) )
        throw std::exception ( );
    ssize_t real_input_str_size = recv ( c, & input_str, BUFFER_SIZE - 2, 0 );
    if ( real_input_str_size <= 0 )
        serverSyntaxError ( c );
    m_resting_input += string { begin ( input_str ), ( size_t ) real_input_str_size };
    addNewDataToInputQueue ( );
}

void CInput::addNewDataToInputQueue ( )
{
    size_t pos = m_resting_input . find ( "\a\b" );

    while ( pos != string::npos )
    {
        m_input_queue . push_back ( m_resting_input . substr ( 0, pos ) );
        pos += 2;
        m_resting_input . erase ( 0, pos );
        pos = m_resting_input . find ( "\a\b" );
    }
}

void makeSumOfBytes ( const string & str, int & summary )
{
    summary = 0;
    for ( char i : str )
        summary += i;
}

void hashCalculating ( int key_id, int user, bool first, int & hash_key )
{
    int key;
    getPairForKey ( key_id, key, first );
    hash_key = ( user * 1000 + key ) % 65536;
}

void checkForDigitsOnly ( const string & str, int c )
{
    for ( const char i : str )
    {
        if ( ! isdigit ( i ) )
            serverSyntaxError ( c );
    }
}

void usernameRequest ( int c, const string & str, int & user, string & curr_state, string & stage, CInput & in )
{
    makeSumOfBytes ( str, user );
    checkSending ( c, "107 KEY REQUEST\a\b" );
    curr_state = "key id";
    stage = "undefined";
}

void keyIDRequest ( int c, const string & str, int & key_id, int & user, string & curr_state, string & stage )
{
    if ( str . size ( ) > 3 || str . empty ( ) || ! isdigit ( str[ 0 ] ) )
        serverSyntaxError ( c );
    key_id = stoi ( str );
    if ( key_id < 0 || key_id > 4 )
        serverKeyOutOfRangeError ( c );
    int hash_key = 0;
    hashCalculating ( key_id, user, true, hash_key );
    checkSending ( c, to_string ( hash_key ) + "\a\b" );
    curr_state = "confirmation";
    stage = "confirmation";
}

void confirmationRequest ( int c, const string & str, int key_id, int user, string & curr_state, string & stage )
{
    checkForDigitsOnly ( str, c );
    int hash_key = 0;
    hashCalculating ( key_id, user, false, hash_key );
    if ( hash_key != stoi ( str ) )
        serverLoginFailed ( c );
    checkSending ( c, "200 OK\a\b" );
    curr_state = "move";
    stage = "undefined";
}


bool beginServerWork ( int c, string & curr_state, CInput & in )
{
    int user;
    int key;
    string stage = "username";
    string str = in . readDataAndReturnNextStr ( c, stage );

    if ( curr_state == "username" )
    {
        usernameRequest ( c, str, user, curr_state, stage, in );
        str = in . readDataAndReturnNextStr ( c, stage );
    }
    if ( curr_state == "key id" )
    {
        keyIDRequest ( c, str, key, user, curr_state, stage );
        str = in . readDataAndReturnNextStr ( c, stage );
    }
    if ( curr_state == "confirmation" )
    {
        confirmationRequest ( c, str, key, user, curr_state, stage );
        return true;
    }
    close ( c );
    throw exception ( );

}

void receiveMessage ( int c, const string & message_exp, CInput & in, string & message )
{
    string str = in . readDataAndReturnNextStr ( c, message_exp );
    message = "message";
}

void getOKMessage ( int c, const string & stage_exp, CInput & in, string & message )
{
    string str = in . readDataAndReturnNextStr ( c, stage_exp );
    istringstream str_by_tokens { str };
    string token;
    str_by_tokens >> token;
    message = "ok message";
    if ( token == "OK" )
    {
        if ( str . find ( '.' ) != string::npos )
            serverSyntaxError ( c );
        return;
    }
    close ( c );
    throw exception ( );
}

void getOKMessageAndCoordinates ( int c, const string & stage_exp, CInput & in,
                                  pair < int, int > & coordinates, string & message )
{
    string str = in . readDataAndReturnNextStr ( c, stage_exp );
    istringstream str_by_tokens { str };
    string token;
    str_by_tokens >> token;
    message = "ok message";
    coordinates = { DEFAULT_X, DEFAULT_Y };
    if ( token == "OK" )
    {
        if ( str . find ( '.' ) != string::npos )
            serverSyntaxError ( c );
        str_by_tokens >> coordinates . first >> coordinates . second;
        return;
    }
    close ( c );
    throw exception ( );
}

void tryToMove ( int c, const string & direction )
{
    static const unordered_map < string, string > map_od_directions =
            {
                    { "FORWARD", "102 MOVE\a\b" },
                    { "LEFT",    "103 TURN LEFT\a\b" },
                    { "RIGHT",   "104 TURN RIGHT\a\b" }
            };
    auto dir = map_od_directions . find ( direction );
    if ( dir == map_od_directions . end ( ) )
    {
        close ( c );
        throw exception ( );
    }
    checkSending ( c, dir -> second );
}

void reportIfNoOkMessage ( const string & receive, int c )
{
    if ( receive != "ok message" )
    {
        close ( c );
        throw exception ( );
    }
}

void getDirection ( int c, pair < int, int > & coordinates_before, pair < int, int > & coordinates_future,
                    string & direction )
{
    if ( coordinates_before . first == DEFAULT_X && coordinates_before . second == DEFAULT_Y
         || coordinates_before . first == coordinates_future . first &&
            coordinates_before . second == coordinates_future . second )
    {
        direction = "UNDEFINED";
        return;
    }
    else if ( coordinates_before . first == coordinates_future . first &&
              coordinates_before . second - 1 == coordinates_future . second )
    {
        direction = "DOWN";
        return;
    }
    else if ( coordinates_before . first - 1 == coordinates_future . first &&
              coordinates_before . second == coordinates_future . second )
    {
        direction = "LEFT";
        return;
    }
    else if ( coordinates_before . first == coordinates_future . first &&
              coordinates_before . second + 1 == coordinates_future . second )
    {
        direction = "UP";
        return;
    }
    else if ( coordinates_before . first + 1 == coordinates_future . first &&
              coordinates_before . second == coordinates_future . second )
    {
        direction = "RIGHT";
        return;
    }
    close ( c );
    throw std::exception ( );
}

void calculateNextDirection ( string & dir, int c )
{
    static const unordered_map < string, string > map_of_directions =
            {
                    { "DOWN",  "RIGHT" },
                    { "UP",    "LEFT" },
                    { "LEFT",  "DOWN" },
                    { "RIGHT", "UP" }
            };
    const auto direction = map_of_directions . find ( dir );
    if ( direction == map_of_directions . end ( ) )
    {
        close ( c );
        throw std::exception ( );
    }
    dir = direction -> second;
}

void turn ( int c, string orig_direction, const string & goal_direction, CInput & in )
{
    while ( orig_direction != goal_direction )
    {
        tryToMove ( c, "LEFT" );
        string message;
        getOKMessage ( c, "ok message", in, message );
        reportIfNoOkMessage ( message, c );
        calculateNextDirection ( orig_direction, c );
    }
}

void makeMoveWithDirection ( int c, const string & direction, CInput & in )
{
    tryToMove ( c, direction );
    pair < int, int > coordinates;
    string message;
    getOKMessageAndCoordinates ( c, "ok message", in, coordinates, message );
    reportIfNoOkMessage ( message, c );
}

void solveObstacle ( int c, pair < int, int > & coordinates, CInput & in )
{
    vector < string > vec_of_directions;
    string direction;
    if ( abs ( coordinates . first ) == 1 || abs ( coordinates . second ) == 1 )
    {
        vec_of_directions = { "LEFT", "FORWARD", "RIGHT" };
        direction = "FORWARD";
    }
    else
    {
        vec_of_directions = {
                "LEFT", "FORWARD", "RIGHT", "FORWARD",
                "FORWARD", "RIGHT", "FORWARD"
        };
        direction = "LEFT";
    }

    for ( const auto & dir : vec_of_directions )
        makeMoveWithDirection ( c, dir, in );

    tryToMove ( c, direction );
    string message;
    getOKMessageAndCoordinates ( c, "ok message", in, coordinates, message );
    reportIfNoOkMessage ( message, c );
}

void moving ( int c, CInput & in, pair < int, int > & coordinates, bool first )
{
    while ( first ? coordinates . first : coordinates . second )
    {
        tryToMove ( c, "FORWARD" );
        string message;
        pair < int, int > tmp_coordinates;
        getOKMessageAndCoordinates ( c, "ok message", in, tmp_coordinates, message );
        reportIfNoOkMessage ( message, c );
        if ( coordinates == tmp_coordinates )
            solveObstacle ( c, coordinates, in );
        else
        {
            coordinates = tmp_coordinates;
        }
    }
}

class CRobot
{
public:
    CRobot ( )
            : m_coordinates ( DEFAULT_X, DEFAULT_Y ), m_message ( "UNDEFINED" )
    {
    }

    void firstMoveInitial ( int c, CInput & in );

    void passColumns ( int c, CInput & in );

    void passRows ( int c, CInput & in );

    void pickUpFinalMessage ( int c, CInput & in );

private:
    pair < int, int > m_coordinates;
    string m_message;
};

void CRobot::firstMoveInitial ( int c, CInput & in )
{
    pair < int, int > tmp_coordinates = { DEFAULT_X, DEFAULT_Y };
    string message_from_receive = "undefined";
    vector < string > vec_of_directions = { "RIGHT", "FORWARD", "LEFT", "FORWARD", "UNDEFINED" };
    for ( const auto & dir : vec_of_directions )
    {
        tryToMove ( c, dir );
        getOKMessageAndCoordinates ( c, "ok message", in, m_coordinates, message_from_receive );
        reportIfNoOkMessage ( message_from_receive, c );
        getDirection ( c, tmp_coordinates, m_coordinates, m_message );
        if ( m_message != "UNDEFINED" )
            break;
        tmp_coordinates = m_coordinates;
    }
}

void CRobot::passColumns ( int c, CInput & in )
{
    if ( m_coordinates . second > 0 )
    {
        if ( m_message != "DOWN" )
            turn ( c, m_message, "DOWN", in );
        m_message = "DOWN";
    }
    else if ( m_coordinates . second < 0 )
    {
        if ( m_message != "UP" )
            turn ( c, m_message, "UP", in );
        m_message = "UP";
    }
    else
        return;
    moving ( c, in, m_coordinates, false );
}

void CRobot::passRows ( int c, CInput & in )
{
    if ( m_coordinates . first > 0 )
    {
        if ( m_message != "LEFT" )
            turn ( c, m_message, "LEFT", in );
        m_message = "LEFT";
    }
    else if ( m_coordinates . first < 0 )
    {
        if ( m_message != "RIGHT" )
            turn ( c, m_message, "RIGHT", in );
        m_message = "RIGHT";
    }
    else
        return;
    moving ( c, in, m_coordinates, true );
}

void CRobot::pickUpFinalMessage ( int c, CInput & in )
{
    pair < int, int > goal_coordinates = { 0, 0 };
    if ( m_coordinates == goal_coordinates )
    {
        checkSending ( c, "105 GET MESSAGE\a\b" );
        string message;
        receiveMessage ( c, "message", in, message );
        if ( message == "message" )
        {
            checkSending ( c, "106 LOGOUT\a\b" );
            return;
        }
    }
    close ( c );
    throw exception ( );
}

bool startMoving ( int & c, string & curr_state, CInput & in )
{
    if ( curr_state != "move" )
        return false;
    CRobot robot;
    robot . firstMoveInitial ( c, in );
    robot . passColumns ( c, in );
    robot . passRows ( c, in );
    robot . pickUpFinalMessage ( c, in );
    return true;
}

int main ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
        cerr << "Usage: server port" << endl;
        return - 1;
    }

    // Creation of the final socket
    int l = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( l < 0 )
    {
        perror ( "Cannot create a socket: " );
        return - 1;
    }

    int port = stoi ( argv[ 1 ] );
    if ( port == 0 )
    {
        cerr << "Usage: server port" << endl;
        close ( l );
        return - 1;
    }

    struct sockaddr_in address;
    bzero ( & address, sizeof ( address ) );
    address . sin_family = AF_INET;
    address . sin_port = htons ( port );
    address . sin_addr . s_addr = htonl ( INADDR_ANY );

    int returning_value = 0;

    // Connecting sockets
    if ( bind ( l, ( struct sockaddr * ) & address, sizeof ( address ) ) < 0 )
    {
        perror ( "Problem with bind(): " );
        close ( l );
        return - 1;
    }

    // Marking socket as passive
    if ( listen ( l, 10 ) < 0 )
    {
        perror ( "Problem with listen()!" );
        close ( l );
        return - 1;
    }

    struct sockaddr_in remoted_address;
    socklen_t size;
    while ( true )
    {
        // Waiting for the connection
        int c = accept ( l, ( struct sockaddr * ) & remoted_address, & size );
        if ( c < 0 )
        {
            perror ( "Problem with accept()!" );
            close ( l );
            return - 1;
        }
        pid_t pid = fork ( );
        if ( pid == 0 )
        {
            close ( l );
            string state = "username";
            CInput input;
            while ( true )
            {
                try
                {
                    beginServerWork ( c, state, input );
                    if ( ! startMoving ( c, state, input ) )
                    {
                        close ( c );
                        returning_value = - 1;
                        break;
                    }
                }
                catch ( ... )
                {
                    returning_value = - 1;
                    break;
                }
            }
            close ( c );
            break;
        }
        int status = 0;
        waitpid ( 0, & status, WNOHANG );
        close ( c );
    }
    close ( l );
    return returning_value;
}