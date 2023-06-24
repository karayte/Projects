#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/*
 *   The task is to create C/C++ functions that can transcode a file in UTF8 encoding to Fibonacci code and back.
 *   We store characters as numbers - indexes in the code table, today most often UNICODE. There are many characters
 * in UNICODE - 1048576+65536. If they were stored directly as binary numbers, 4 bytes would be needed to represent
 * one character. However, most characters have lower codes, e.g. values smaller than 128 (65536). This is used by
 * UTF-8 encoding, which, depending on the written value, stores the character more compactly in 1 to 4 bytes.
 * UTF-8 encoding for normal text files reduces the total amount of stored data, but it is not optimal. When designing
 * the UTF-8 encoding, a method was chosen where the codes of individual characters always end at the boundary of
 * the entire byte. This is handy for fast loading, but not optimal for length. Other codes have been developed for
 * memory-efficient storage of integers, such as the Fibonacci code, which is based on the Fibonacci sequence.
 *   Let us recall that the Fibonacci sequence is a sequence of integers, where each next number of the sequence is
 * created as the sum of the two previous numbers. The sequence has the form 1, 1, 2, 3, 5, 8, ...
 * The numbers in the Fibonacci code are stored as bit sequences, which indicate the presence of the corresponding
 * element of the Fibonacci sequence with the set bit.
 *   In this implementation we will use Fibonacci sequence, where there will not be subsequence consisting of 11... only.
 * So, for example, 11 will be represented as 1 0 0 1 0 1, which is 8 + 2 + 1 = 11.
 * Fibonacci code starts with 1 1 2 3 5 8 13 21 ......
 * To show the end of the symbol we will use 1 in the end of each Fibonacci code representing one symbol.
 * So, the sequence of symbols 01 001 101 will be written like 01100111011.
 *   LSB is on the right side.
 * */

/* This function fulfills given vector with Fibonacci sequence till stop number */
void createFibonacciSequence ( int stop_num, vector < int > & fib_num )
{
    int curr_fib_num = 3;
    size_t first_last = 1;
    size_t second_last = 2;
    fib_num . push_back ( 1 );
    fib_num . push_back ( 2 );
    while ( curr_fib_num <= stop_num )
    {
        fib_num . push_back ( curr_fib_num );
        curr_fib_num = fib_num [ first_last ] + fib_num [ second_last ];
        first_last ++;
        second_last ++;
    }
}

/* This function changes the line to its inverse version */
void inverseStrLine ( string & line )
{
    string new_fib_code = ""s;
    for ( size_t i = line . length() - 1; i < line . length(); i -- )
        new_fib_code . push_back ( line [ i ] );
    line . clear();
    line = new_fib_code;
}

/* This function is used to create a Fibonacci code from a number */
void createFibonacciCode ( int num, string & fib_code, vector < int > fib_num )
{
    bool beginning = true;
    fib_code . clear ();
    fib_code . push_back ( '1' );
    num ++;
    for ( size_t pos = fib_num . size () - 1; pos < fib_num . size (); pos -- )
    {
        if ( num < fib_num [ pos ] && beginning )
            continue;
        else if ( num >= fib_num [ pos ] )
        {
            fib_code . push_back ( '1' );
            num -= fib_num [ pos ];
            beginning = false;
        }
        else
            fib_code . push_back ( '0' );
    }
    inverseStrLine ( fib_code );
}

/* This function is used to create a string from a single byte. This is not necessary, but is quite useful for
 * debug purposes */
void createStringFromOneByte ( char byte, string & byte_str )
{
    byte_str . clear();
    for ( int i = 7; i >= 0; i -- )
    {
        if ( ( byte >> i ) & 1 )
            byte_str . push_back ( '1' );
        else
            byte_str . push_back ( '0' );
    }
}

/* This function is used to convert bytes represented by a string to decimal number */
void createIntegerFromString ( int & num, const string & byte )
{
    num = 0;
    int curr_pow = 1;
    for ( size_t i = byte . length () - 1; i < byte . length (); i -- )
    {
        if ( byte [ i ] == '1' )
            num += curr_pow;
        curr_pow *= 2;
    }
}

/* This function is used to create bytes for conversion Fibonacci codes to UTF - x or ASCII */
void createByteFromStringSizeEight ( char & byte, string & str )
{
    byte = '\0';

    for ( size_t ch = 0, i = str . length() - 1; ch < 8 or i < str . length(); ch ++, i -- )
    {
        if ( str [ i ] == '1' )
            byte |= 1 << ch;
    }
}

/* This function helps to cut 8 bytes string ( representing bits ) from a larger string */
void splitStringByEight ( string & orig_str, string & out_str )
{
    out_str . clear ();
    for ( size_t i = 0; i < 8; i ++ )
        out_str . push_back ( orig_str [ i ] );
    orig_str . erase ( 0, 8 );
}

/* This function helps to write and check success for 1 byte */
bool writeOneByteToOutputFile ( ofstream & output_file, string & byte_str )
{
    inverseStrLine ( byte_str );
    char byte_to_be_written;
    createByteFromStringSizeEight ( byte_to_be_written, byte_str );
    if ( ! output_file . put ( byte_to_be_written ) || ! output_file . good() )
        return false;
    return true;
}

/* This function is responsible for correct writing of the output */
bool writingToOutputFile ( ofstream & output_file, string & output_str, bool & beginning, bool padding )
{
    // this line is used to store data which we cannot write to output ( yet )
    static string line_to_be_supplemented;
    bool returning_info = true;
    // we need to clear the line we used for storage
    if ( beginning )
    {
        line_to_be_supplemented . clear ();
        beginning = false;
    }
    line_to_be_supplemented . append ( output_str );
    while ( true )
    {
        if ( line_to_be_supplemented . length () < 8 )
        {
            if ( padding && line_to_be_supplemented . length () != 0 )
            {
                for ( size_t i = line_to_be_supplemented . length (); i < 8; i ++ )
                    line_to_be_supplemented . push_back ('0' );
                if ( ! writeOneByteToOutputFile (output_file, line_to_be_supplemented ) )
                    returning_info = false;
            }
            break;
        }
        string byte_str = ""s;
        splitStringByEight (line_to_be_supplemented, byte_str );
        if ( ! writeOneByteToOutputFile ( output_file, byte_str ) )
        {
            returning_info = false;
            break;
        }
    }
    return returning_info;
}

/* This function is used to extract number from coding like UTF - 16, UTF - 24, UTF - 32 */
void extractingNumFromUTFAndCodingIntoFibonacci ( string & whole_character, const vector < int > & fib_num )
{
    if ( whole_character . length () == 16 )
    {
        whole_character . erase ( 0, 3 );
        whole_character . erase ( 5, 2 );
    }
    else if ( whole_character . length () == 24 )
    {
        whole_character . erase ( 0, 4 );
        whole_character . erase ( 4, 2 );
        whole_character . erase ( 10, 2 );
    }
    else
    {
        whole_character . erase ( 0, 5 );
        whole_character . erase ( 3, 2 );
        whole_character . erase ( 9, 2 );
        whole_character . erase ( 15, 2 );
    }
    int num;
    createIntegerFromString ( num, whole_character );
    createFibonacciCode ( num, whole_character, fib_num );
}

/* These functions are used for validation of the bytes. During conversions or sending some of them maybe "broken".
 * Each UTF/ASCII symbol is checked individually */
bool checkingValidBitsForFourBytesCharacter ( const string & whole_character )
{
    return ( whole_character [ 0 ]  == '1' && whole_character [ 1 ]  == '1' && whole_character [ 2 ] == '1'
             && whole_character [ 3 ]  == '1' && whole_character [ 4 ]  == '0'
             && whole_character [ 8 ]  == '1' && whole_character [ 9 ]  == '0'
             && whole_character [ 16 ] == '1' && whole_character [ 17 ] == '0'
             && whole_character [ 24 ] == '1' && whole_character [ 25 ] == '0');
}

bool checkingValidBitsForThreeBytesCharacter ( const string & whole_character )
{
    return ( whole_character [ 0 ] == '1' && whole_character [ 1 ] == '1' && whole_character [ 2 ] == '1'
             && whole_character [ 3 ] == '0'
             && whole_character [ 8 ] == '1' && whole_character [ 9 ] == '0'
             && whole_character [ 16 ] == '1' && whole_character [ 17 ] == '0');
}

bool checkingValidBitsForTwoBytesCharacter ( const string & whole_character )
{
    return ( whole_character [ 0 ] == '1' && whole_character [ 1 ] == '1' && whole_character [ 2 ] == '0'
             && whole_character [ 8 ] == '1' && whole_character [ 9 ] == '0' );
}

/* This function works with UTF symbols and creating Fibonacci code for valid symbols */
bool nonASCIISymbol ( ifstream & input_file, ofstream  & output_file, string & whole_character,
                      const vector < int > & fib_num, bool & beginning )
{
    bool ok = false;
    if ( whole_character . length () == 16 )
        ok = checkingValidBitsForTwoBytesCharacter ( whole_character );
    else if ( whole_character . length () == 24 )
        ok = checkingValidBitsForThreeBytesCharacter ( whole_character );
    else if ( whole_character . length () == 32 )
        ok = checkingValidBitsForFourBytesCharacter ( whole_character );
    if ( ! ok )
        return false;

    extractingNumFromUTFAndCodingIntoFibonacci(whole_character, fib_num);
    if ( ! writingToOutputFile ( output_file, whole_character, beginning, false ) )
        return false;
    return true;
}

/* This function extracts exactly 1 byte from the input file */
bool getByteFromInputFile ( ifstream & inputFile, char & byte )
{
    if ( ! inputFile . read ( & byte, 1 ) || ! inputFile . good() )
        return false;
    return true;
}

/* This function helps to extract the whole UTF symbol, resp. the end of the symbol extracted before */
bool addingOneMoreByte ( ifstream & input_file, string & whole_character )
{
    char byte;
    string tmp_str = ""s;
    if ( ! getByteFromInputFile ( input_file, byte ) )
        return false;
    createStringFromOneByte ( byte, tmp_str );
    whole_character . append ( tmp_str );
    return true;
}

/* this function is used to read the whole UTF-x character by bytes */
bool getWholeCharacter ( ifstream & input_file, ofstream & output_file, const vector < int > & fib_num,
                         char & first_byte, bool & beginning )
{
    //10xxxxxx - is invalid UTF8 character, must be 11xxxxxx ...
    if ( ! ( first_byte & ( 256 >> ( 1 + 1 ) ) ? 1 : 0 ) )
        return false;
    string whole_character = ""s;
    createStringFromOneByte ( first_byte, whole_character );
    char byte;
    if ( ! getByteFromInputFile ( input_file, byte ) )
        return false;

    string tmp_str = ""s;
    createStringFromOneByte ( byte, tmp_str );
    whole_character . append ( tmp_str );
    size_t num_of_one = 2;
    while ( ( first_byte & ( 256 >> ( num_of_one + 1 ) ) ? 1 : 0 ) )
        num_of_one ++;

    if ( num_of_one >= 3 )
    {
        if ( ! addingOneMoreByte ( input_file, whole_character ) )
            return false;
    }
    if ( num_of_one >= 4 )
    {
        if ( ! addingOneMoreByte ( input_file, whole_character ) )
            return false;
    }

    return nonASCIISymbol ( input_file, output_file, whole_character, fib_num, beginning );
}

/* This function works with ASCII symbols and convert them to Fibonacci code */
bool byteASCII ( ofstream & output_file, char & one_byte, const vector < int > & fib_num, bool & beginning )
{
    int num = 0;
    string byte_str = ""s;
    createStringFromOneByte ( one_byte, byte_str );
    createIntegerFromString ( num, byte_str );
    string output_str = ""s;
    createFibonacciCode ( num, output_str, fib_num );
    if ( ! writingToOutputFile ( output_file, output_str, beginning, false ) )
        return false;
    return true;
}

/* Reading input function. It is easier to read input by bytes rather than by blocks */
bool readingByBytes ( ifstream & input_file, ofstream & output_file, vector < int > & fib_num, bool & beginning,
                      bool & problem_in_numbers )
{
    size_t first_bit;
    char one_byte;
    if ( ! getByteFromInputFile ( input_file, one_byte ) )
        return false;

    first_bit = ( one_byte & ( 256 >> 1 ) ? 1 : 0 ) ;
    if ( first_bit == 0 )
        return byteASCII ( output_file, one_byte, fib_num, beginning );
    else
    {
        if ( ! getWholeCharacter ( input_file, output_file, fib_num, one_byte, beginning ) )
        {
            problem_in_numbers = true;
            return false;
        }
        return true;
    }
}

/* A cleaning function which returns flags as well. True for success */
bool cleaning ( ifstream & input_file, ofstream & output_file, bool returning_flag, vector < int > & fib_num )
{
    input_file . close ();
    output_file . close ();
    fib_num . clear();
    return returning_flag;
}

/* This is the main function for extracting and converting symbols to Fibonacci codes */
bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    ifstream input_file;
    input_file . open ( inFile );
    if ( ! input_file . is_open () )
        return false;
    ofstream output_file;
    output_file . open ( outFile );
    if ( ! output_file . is_open () )
        return false;
    vector < int > fib_num;
    bool beginning = true;
    int max_from_four_bytes = 1114111;
    bool returning_info = true;
    createFibonacciSequence ( max_from_four_bytes, fib_num );
    bool problem_in_numbers = false;
    while ( true )
    {
        problem_in_numbers = false;
        bool ok = readingByBytes ( input_file, output_file, fib_num, beginning, problem_in_numbers );
        if ( ! ok )
        {
            if ( input_file . eof () )
            {
                string str = ""s;
                if ( problem_in_numbers )
                    returning_info = false;
                if ( writingToOutputFile ( output_file, str, beginning, true ) )
                    break;
            }
            returning_info = false;
            break;
        }
    }
    return cleaning (input_file, output_file, returning_info, fib_num);
}

/* ----------------------------------------------FIBONACCI---------------------------------------------*/

/* This function is used to cut first 8 elements of the binary_str and convert them into one byte */
void fibMoveEightBitsToByte ( string & binary_str, char & byte )
{
    string tmp_binary_str = ""s;
    for ( size_t i = 0; i < 8; i ++ )
        tmp_binary_str . push_back ( binary_str [ i ] );
    binary_str . erase ( 0, 8 );
    createByteFromStringSizeEight ( byte, tmp_binary_str );
}

/* This function is used for writing the output into file */
bool fibWritingToFile ( ofstream & output_file, string & binary_str )
{
    char byte_to_be_written;
    bool returning_info = true;
    while ( true )
    {
        if ( binary_str . length() >= 8 )
        {
            fibMoveEightBitsToByte ( binary_str, byte_to_be_written );
            if ( ! output_file . put ( byte_to_be_written ) || !output_file . good() )
            {
                returning_info = false;
                break;
            }
        }
        else
            break;
    }
    return returning_info;
}

/* This function is responsible for fulfilling future UTF - x symbols in "the middle" */
void fibFillingMiddleBytes ( string & src, string & dst )
{
    for ( size_t i = src . length() - 6 ; i < src . length(); i ++ )
        dst . push_back ( src [ i ] );
    src . erase ( src . length() - 6, 6 );
}

/* This function is responsible for fulfilling future UTF - x symbols in "at the beginning" */
void fibFillingBeginningBytesWithZeros ( string & src, string & dst, size_t amount_of_bytes )
{
    for ( size_t i = src . length(); i < amount_of_bytes; i ++ )
        dst . push_back ( '0' );
    dst . append ( src );
    src . clear();
}

/* This function creates UTF-32 symbol from Fibonacci code */
void fibCreatingUTFFourBytesFromFibInt ( string & binary_str )
{
    inverseStrLine ( binary_str );
    string beginning = "11110";
    string middle_first = "10";
    string middle_second = "10";
    string middle_third = "10";
    fibFillingMiddleBytes ( binary_str, middle_third );
    fibFillingMiddleBytes ( binary_str, middle_second );
    if ( binary_str . length() < 6 )
    {
        for ( size_t i = binary_str . length(); i < 6; i ++ )
            middle_first . push_back ( '0' );
        middle_first . append ( binary_str );
        binary_str . clear();
    }
    else
        fibFillingMiddleBytes ( binary_str, middle_first );
    fibFillingBeginningBytesWithZeros ( binary_str, beginning, 3 );
    beginning . append ( middle_first );
    beginning . append ( middle_second );
    beginning . append ( middle_third );
    binary_str = beginning;
}

/* This function creates UTF-24 symbol from Fibonacci code */
void fibCreatingUTFThreeBytesFromFibInt ( string & binary_str )
{
    inverseStrLine ( binary_str );
    string beginning = "1110";
    string middle_first = "10";
    string middle_second = "10";
    fibFillingMiddleBytes ( binary_str, middle_second );
    fibFillingMiddleBytes ( binary_str, middle_first );
    fibFillingBeginningBytesWithZeros ( binary_str, beginning, 4 );
    beginning . append ( middle_first );
    beginning . append ( middle_second );
    binary_str = beginning;
}

/* This function creates UTF-16 symbol from Fibonacci code */
void fibCreatingUTFTwoBytesFromFibInt ( string & binary_str )
{
    inverseStrLine ( binary_str );
    string beginning = "110";
    string middle = "10";
    fibFillingMiddleBytes ( binary_str, middle );
    fibFillingBeginningBytesWithZeros ( binary_str, beginning, 5 );
    beginning . append ( middle );
    binary_str = beginning;
}

/* This function creates ASCII symbol from Fibonacci code */
void fibCreatingASCIIFromFibInt ( string & binary_str )
{
    for ( size_t i = binary_str . length(); i < 8; i ++ )
        binary_str . push_back ( '0' );
    inverseStrLine ( binary_str );
}

/* This function is used to decode fibonacci number back into ASCII/UTF-x symbols */
bool fibFindingUTFReprForFibInt ( int fib_int, string & binary_str )
{
    binary_str . clear();
    // fibonacci number cannot be negative
    if ( fib_int < 0 )
        return false;
    if ( fib_int == 0 )
        binary_str . push_back ( '0' );
    else
    {
        while ( fib_int > 1 )
        {
            if ( fib_int % 2 )
                binary_str . push_back ( '1' );
            else
                binary_str . push_back ( '0' );
            fib_int /= 2;
        }
        binary_str . push_back ( '1' );
    }

    if ( binary_str . length () <= 7 )
        fibCreatingASCIIFromFibInt ( binary_str );
    else if ( binary_str . length() <= 11 )
        fibCreatingUTFTwoBytesFromFibInt ( binary_str );
    else if ( binary_str . length() <= 16 )
        fibCreatingUTFThreeBytesFromFibInt ( binary_str );
    else if ( binary_str . length() <= 21 )
        fibCreatingUTFFourBytesFromFibInt ( binary_str );
    else
        return false;
    return true;
}

/* This function is used to calculate int number which was coded into fibonacci one,
 * it also checks if decoded number is valid */
bool fibCheckForLimitsAndIntCalculating ( string & fib_str, int & num, const vector < int > & fib_num )
{
    num = 0;
    bool returning_info = true;
    for ( size_t i = 0; i < fib_str . length(); i ++ )
    {
        if ( fib_str [ i ] == '1' )
        {
            if ( fib_num . size () > i )
                num += fib_num [ i ];
            else
            {
                returning_info = false;
                break;
            }
        }
    }
    num --;
    if ( num > 1114111 || num < 0 )
        returning_info = false;

    return returning_info;
}

/* This function creates Fibonacci sequenced from the string fulfilled from the input.
 * To convert a symbol from Fibonacci code to ASCII/UTF-x we need to get it first due to the fact,
 * that the input is read by bytes and Fibonacci symbol maybe be shorter or longer than 8*k, k from Z+ */
bool fibCreateFibSeqFromString ( string & orig_str, string & fib_str_new, size_t end_for_fib_seq )
{
    fib_str_new . clear ();
    if ( end_for_fib_seq >= orig_str . length() )
        return false;
    for ( size_t i = 0; i < end_for_fib_seq; i ++ )
        fib_str_new . push_back ( orig_str [ i ] );
    orig_str . erase ( 0, end_for_fib_seq + 1 );
    return true;
}

/* This function searches for 11 = ending of the Fibonacci symbol */
bool fibSearchingForEnding ( string & fib_str, size_t & pos )
{
    bool returning_flag = false;
    pos = 0;
    for ( size_t i = 0; i < fib_str . length (); i ++ )
    {
        if ( fib_str [ i ] == '1' && i > 0 )
        {
            if ( fib_str [ i - 1 ] == '1' )
            {
                pos = i;
                returning_flag = true;
                break;
            }
        }
    }
    return returning_flag;
}

/* As the Fibonacci code can be padded with zeros in the end to become a whole byte,
 * this function checks for proper ending/padding */
bool fibCheckingLastBits ( string & fib_str )
{
    bool returning_info = false;
    if ( fib_str . length() > 7 )
        return returning_info;
    for ( char i : fib_str )
    {
        if ( i == '1' )
            return returning_info;
    }
    returning_info = true;
    return returning_info;
}

/* This function extracts bytes from the input till at least one whole Fibonacci symbol is found */
bool fibGettingBytesUntilFullFibNumIsFound ( ifstream & input_file, string & fib_str, size_t & ending_for_fib_seq )
{
    char byte;
    string tmp = ""s;
    bool returning_info = true;
    while ( ! fibSearchingForEnding ( fib_str, ending_for_fib_seq )
            && fib_str . length() < 40 )
    {
        tmp . clear();
        if ( ! getByteFromInputFile ( input_file, byte ) )
        {
            returning_info = false;
            break;
        }
        createStringFromOneByte ( byte, tmp );
        inverseStrLine ( tmp );
        fib_str . append ( tmp );
    }
    return returning_info;
}

/* As we do not have enough memory to store the whole output, we will write one decoded symbol to output file */
bool writingToOutputFileUntilFullFibNumIsFound ( ifstream & input_file, ofstream & output_file, string & fib_str,
                                                 size_t ending_for_fib_seq, const vector < int > & fib_num )
{
    bool returning_info = true;
    while ( ending_for_fib_seq )
    {
        int num_in_utf;
        string fib_str_to_be_written = ""s;
        if ( ! fibCreateFibSeqFromString ( fib_str, fib_str_to_be_written, ending_for_fib_seq ) )
        {
            returning_info = false;
            break;
        }
        if ( ! fibCheckForLimitsAndIntCalculating ( fib_str_to_be_written, num_in_utf, fib_num ) || !input_file . good() || !output_file . good()
             || input_file . fail() || output_file. fail())
        {
            returning_info = false;
            break;
        }
        if ( ! fibFindingUTFReprForFibInt ( num_in_utf, fib_str_to_be_written ) )
        {
            returning_info = false;
            break;
        }
        if ( ! fibWritingToFile ( output_file, fib_str_to_be_written ) || !input_file . good() || !output_file . good()
             || input_file . fail() || output_file. fail())
        {
            returning_info = false;
            break;
        }
        fibSearchingForEnding ( fib_str,ending_for_fib_seq );
    }
    return returning_info;
}

/* This function is used for reading from the input, checking for errors and writing to the output file.
 * Depending on weather we need to check for proper ending or extracting bytes and writing symbol to the output,
 * True or False is returned */
bool fibReadingByBytes ( ifstream & input_file, ofstream & output_file, const vector < int > & fib_num,
                         bool & beginning, bool end_check )
{
    static string fib_str;
    bool returning_info;
    if ( end_check )
        return fibCheckingLastBits ( fib_str );
    if ( beginning )
    {
        fib_str . clear();
        beginning = false;
    }

    string tmp = ""s;
    size_t ending_for_fib_seq = 0;

    if ( ! fibGettingBytesUntilFullFibNumIsFound ( input_file, fib_str, ending_for_fib_seq ) )
        returning_info = false;
    else
        returning_info = writingToOutputFileUntilFullFibNumIsFound ( input_file, output_file, fib_str,
                                                                     ending_for_fib_seq, fib_num );

    if ( ! input_file . good() || ! output_file . good() || fib_str . length() >= 39 )
        returning_info = false;
    return returning_info;
}

/* This function is used for conversion Fibonacci symbols to ASCII/UTF-x and checking for the correctness
 * of the processes and symbols */
bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
    ifstream input_file;
    input_file . open ( inFile );
    if ( ! input_file . is_open() )
        return false;
    ofstream output_file;
    output_file . open ( outFile );
    if ( ! output_file . is_open() )
        return false;

    vector < int > fib_num;
    int max_from_four_bytes = 1114113;
    createFibonacciSequence ( max_from_four_bytes, fib_num );
    bool ok;
    bool beginning = true;
    while ( true )
    {
        ok = fibReadingByBytes ( input_file, output_file, fib_num, beginning, false );
        if ( ! ok || ! input_file . good() || ! output_file . good() )
        {
            if ( input_file . eof() )
            {
                ok = fibReadingByBytes ( input_file, output_file, fib_num, beginning, true );
                if ( ok )
                    return cleaning ( input_file, output_file, true, fib_num );
            }
            return cleaning ( input_file, output_file, false, fib_num );
        }
    }
}

/* This function is used for checking the correctness of conversion. It takes two files:
 * the first one consists of symbols we have decoded ourselves and the second of has right symbols */
bool identicalFiles ( const char * file1, const char * file2 )
{
    ifstream firstFile  ( file1, ios::binary | ios::ate);
    ifstream secondFile ( file2, ios::binary | ios::ate);

    if ( ! firstFile . is_open() || ! secondFile . is_open() )
        return false;


    if ( firstFile . tellg() != secondFile . tellg() )
        return false;

    firstFile  . seekg ( 0 );
    secondFile . seekg ( 0 );

    char symFirstFile, symSecondFile;
    while ( firstFile . get ( symFirstFile ) && secondFile . get ( symSecondFile ) )
    {
        if ( symFirstFile != symSecondFile)
            return false;
    }
    return true;
}

int main ( )
{
    assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
    assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
    assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
    assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );

    return 0;
}
