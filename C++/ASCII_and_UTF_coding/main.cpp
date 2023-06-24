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
void createFibonacciSequence ( int stopNum, vector < int > & fibNum )
{
    int currFibNum = 3;
    size_t firstLast = 1;
    size_t secondLast = 2;
    fibNum . push_back ( 1 );
    fibNum . push_back ( 2 );
    while ( currFibNum <= stopNum )
    {
        fibNum . push_back ( currFibNum );
        currFibNum = fibNum[ firstLast ] + fibNum[ secondLast ];
        firstLast ++;
        secondLast ++;
    }
}

/* This function changes the line to its inverse version */
void inverseStrLine ( string & line )
{
    string newFibCode = ""s;
    for ( size_t i = line . length ( ) - 1 ; i < line . length ( ) ; i -- )
        newFibCode . push_back ( line[ i ] );
    line . clear ( );
    line = newFibCode;
}

/* This function is used to create a Fibonacci code from a number */
void createFibonacciCode ( int num, string & fibCode, vector < int > fibNum )
{
    bool beginning = true;
    fibCode . clear ( );
    fibCode . push_back ( '1' );
    num ++;
    for ( size_t pos = fibNum . size ( ) - 1 ; pos < fibNum . size ( ) ; pos -- )
    {
        if ( num < fibNum[ pos ] && beginning )
            continue;
        else if ( num >= fibNum[ pos ] )
        {
            fibCode . push_back ( '1' );
            num -= fibNum[ pos ];
            beginning = false;
        }
        else
            fibCode . push_back ( '0' );
    }
    inverseStrLine ( fibCode );
}

/* This function is used to create a string from a single byte. This is not necessary, but is quite useful for
 * debug purposes */
void createStringFromOneByte ( char byte, string & byteStr )
{
    byteStr . clear ( );
    for ( int i = 7 ; i >= 0 ; i -- )
    {
        if ( ( byte >> i ) & 1 )
            byteStr . push_back ( '1' );
        else
            byteStr . push_back ( '0' );
    }
}

/* This function is used to convert bytes represented by a string to decimal number */
void createIntegerFromString ( int & num, const string & byte )
{
    num = 0;
    int currPow = 1;
    for ( size_t i = byte . length ( ) - 1 ; i < byte . length ( ) ; i -- )
    {
        if ( byte[ i ] == '1' )
            num += currPow;
        currPow *= 2;
    }
}

/* This function is used to create bytes for conversion Fibonacci codes to UTF - 8 or ASCII */
void createByteFromStringSizeEight ( char & byte, string & str )
{
    byte = '\0';

    for ( size_t ch = 0, i = str . length ( ) - 1 ; ch < 8 or i < str . length ( ) ; ch ++, i -- )
    {
        if ( str[ i ] == '1' )
            byte |= 1 << ch;
    }
}

/* This function helps to cut 8 bytes string ( representing bits ) from a larger string */
void splitStringByEight ( string & origStr, string & outStr )
{
    outStr . clear ( );
    for ( size_t i = 0 ; i < 8 ; i ++ )
        outStr . push_back ( origStr[ i ] );
    origStr . erase ( 0, 8 );
}

/* This function helps to write and check success for 1 byte */
bool writeOneByteToOutputFile ( ofstream & outputFile, string & byteStr )
{
    inverseStrLine ( byteStr );
    char byteToBeWritten;
    createByteFromStringSizeEight ( byteToBeWritten, byteStr );
    if ( ! outputFile . put ( byteToBeWritten ) || ! outputFile . good ( ) )
        return false;
    return true;
}

/* This function is responsible for correct writing of the output */
bool writingToOutputFile ( ofstream & outputFile, string & outputStr, bool & beginning, bool padding )
{
    // this line is used to store data which we cannot write to output ( yet )
    static string lineToBeSupplemented;
    bool returningInfo = true;
    // we need to clear the line we used for storage
    if ( beginning )
    {
        lineToBeSupplemented . clear ( );
        beginning = false;
    }
    lineToBeSupplemented . append ( outputStr );
    while ( true )
    {
        if ( lineToBeSupplemented . length ( ) < 8 )
        {
            if ( padding && lineToBeSupplemented . length ( ) != 0 )
            {
                for ( size_t i = lineToBeSupplemented . length ( ) ; i < 8 ; i ++ )
                    lineToBeSupplemented . push_back ( '0' );
                if ( ! writeOneByteToOutputFile ( outputFile, lineToBeSupplemented ) )
                    returningInfo = false;
            }
            break;
        }
        string byteStr = ""s;
        splitStringByEight ( lineToBeSupplemented, byteStr );
        if ( ! writeOneByteToOutputFile ( outputFile, byteStr ) )
        {
            returningInfo = false;
            break;
        }
    }
    return returningInfo;
}

/* This function is used to extract number from coding like UTF - 8 */
void extractingNumFromUTFAndCodingIntoFibonacci ( string & wholeCharacter, const vector < int > & fibNum )
{
    if ( wholeCharacter . length ( ) == 16 )
    {
        wholeCharacter . erase ( 0, 3 );
        wholeCharacter . erase ( 5, 2 );
    }
    else if ( wholeCharacter . length ( ) == 24 )
    {
        wholeCharacter . erase ( 0, 4 );
        wholeCharacter . erase ( 4, 2 );
        wholeCharacter . erase ( 10, 2 );
    }
    else
    {
        wholeCharacter . erase ( 0, 5 );
        wholeCharacter . erase ( 3, 2 );
        wholeCharacter . erase ( 9, 2 );
        wholeCharacter . erase ( 15, 2 );
    }
    int num;
    createIntegerFromString ( num, wholeCharacter );
    createFibonacciCode ( num, wholeCharacter, fibNum );
}

/* These functions are used for validation of the bytes. During conversions or sending some of them maybe "broken".
 * Each UTF/ASCII symbol is checked individually */
bool checkingValidBitsForFourBytesCharacter ( const string & wholeCharacter )
{
    return ( wholeCharacter[ 0 ] == '1' && wholeCharacter[ 1 ] == '1' && wholeCharacter[ 2 ] == '1'
             && wholeCharacter[ 3 ] == '1' && wholeCharacter[ 4 ] == '0'
             && wholeCharacter[ 8 ] == '1' && wholeCharacter[ 9 ] == '0'
             && wholeCharacter[ 16 ] == '1' && wholeCharacter[ 17 ] == '0'
             && wholeCharacter[ 24 ] == '1' && wholeCharacter[ 25 ] == '0' );
}

bool checkingValidBitsForThreeBytesCharacter ( const string & wholeCharacter )
{
    return ( wholeCharacter[ 0 ] == '1' && wholeCharacter[ 1 ] == '1' && wholeCharacter[ 2 ] == '1'
             && wholeCharacter[ 3 ] == '0'
             && wholeCharacter[ 8 ] == '1' && wholeCharacter[ 9 ] == '0'
             && wholeCharacter[ 16 ] == '1' && wholeCharacter[ 17 ] == '0' );
}

bool checkingValidBitsForTwoBytesCharacter ( const string & wholeCharacter )
{
    return ( wholeCharacter[ 0 ] == '1' && wholeCharacter[ 1 ] == '1' && wholeCharacter[ 2 ] == '0'
             && wholeCharacter[ 8 ] == '1' && wholeCharacter[ 9 ] == '0' );
}

/* This function works with UTF symbols and creating Fibonacci code for valid symbols */
bool nonASCIISymbol ( ifstream & inputFile, ofstream & outputFile, string & wholeCharacter,
                      const vector < int > & fibNum, bool & beginning )
{
    bool ok = false;
    if ( wholeCharacter . length ( ) == 16 )
        ok = checkingValidBitsForTwoBytesCharacter ( wholeCharacter );
    else if ( wholeCharacter . length ( ) == 24 )
        ok = checkingValidBitsForThreeBytesCharacter ( wholeCharacter );
    else if ( wholeCharacter . length ( ) == 32 )
        ok = checkingValidBitsForFourBytesCharacter ( wholeCharacter );
    if ( ! ok )
        return false;

    extractingNumFromUTFAndCodingIntoFibonacci ( wholeCharacter, fibNum );
    if ( ! writingToOutputFile ( outputFile, wholeCharacter, beginning, false ) )
        return false;
    return true;
}

/* This function extracts exactly 1 byte from the input file */
bool getByteFromInputFile ( ifstream & inputFile, char & byte )
{
    if ( ! inputFile . read ( & byte, 1 ) || ! inputFile . good ( ) )
        return false;
    return true;
}

/* This function helps to extract the whole UTF symbol, resp. the end of the symbol extracted before */
bool addingOneMoreByte ( ifstream & inputFile, string & wholeCharacter )
{
    char byte;
    string tmpStr = ""s;
    if ( ! getByteFromInputFile ( inputFile, byte ) )
        return false;
    createStringFromOneByte ( byte, tmpStr );
    wholeCharacter . append ( tmpStr );
    return true;
}

/* this function is used to read the whole UTF-8 character by bytes */
bool getWholeCharacter ( ifstream & inputFile, ofstream & outputFile, const vector < int > & fibNum,
                         char & firstByte, bool & beginning )
{
    //10xxxxxx - is invalid UTF-8 character, must be 11xxxxxx ...
    if ( ! ( firstByte & ( 256 >> ( 1 + 1 ) ) ? 1 : 0 ) )
        return false;
    string wholeCharacter = ""s;
    createStringFromOneByte ( firstByte, wholeCharacter );
    char byte;
    if ( ! getByteFromInputFile ( inputFile, byte ) )
        return false;

    string tmpStr = ""s;
    createStringFromOneByte ( byte, tmpStr );
    wholeCharacter . append ( tmpStr );
    size_t numOfOne = 2;
    while ( ( firstByte & ( 256 >> ( numOfOne + 1 ) ) ? 1 : 0 ) )
        numOfOne ++;

    if ( numOfOne >= 3 )
    {
        if ( ! addingOneMoreByte ( inputFile, wholeCharacter ) )
            return false;
    }
    if ( numOfOne >= 4 )
    {
        if ( ! addingOneMoreByte ( inputFile, wholeCharacter ) )
            return false;
    }

    return nonASCIISymbol ( inputFile, outputFile, wholeCharacter, fibNum, beginning );
}

/* This function works with ASCII symbols and convert them to Fibonacci code */
bool byteASCII ( ofstream & outputFile, char & oneByte, const vector < int > & fibNum, bool & beginning )
{
    int num = 0;
    string byteStr = ""s;
    createStringFromOneByte ( oneByte, byteStr );
    createIntegerFromString ( num, byteStr );
    string outputStr = ""s;
    createFibonacciCode ( num, outputStr, fibNum );
    if ( ! writingToOutputFile ( outputFile, outputStr, beginning, false ) )
        return false;
    return true;
}

/* Reading input function. It is easier to read input by bytes rather than by blocks */
bool readingByBytes ( ifstream & inputFile, ofstream & outputFile, vector < int > & fibNum, bool & beginning,
                      bool & problemInNumbers )
{
    size_t firstBit;
    char oneByte;
    if ( ! getByteFromInputFile ( inputFile, oneByte ) )
        return false;

    firstBit = ( oneByte & ( 256 >> 1 ) ? 1 : 0 );
    if ( firstBit == 0 )
        return byteASCII ( outputFile, oneByte, fibNum, beginning );
    else
    {
        if ( ! getWholeCharacter ( inputFile, outputFile, fibNum, oneByte, beginning ) )
        {
            problemInNumbers = true;
            return false;
        }
        return true;
    }
}

/* A cleaning function which returns flags as well. True for success */
bool cleaning ( ifstream & inputFile, ofstream & outputFile, bool returningFlag, vector < int > & fibNum )
{
    inputFile . close ( );
    outputFile . close ( );
    fibNum . clear ( );
    return returningFlag;
}

/* This is the main function for extracting and converting symbols to Fibonacci codes */
bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    ifstream inputFile;
    inputFile . open ( inFile );
    if ( ! inputFile . is_open ( ) )
        return false;
    ofstream outputFile;
    outputFile . open ( outFile );
    if ( ! outputFile . is_open ( ) )
        return false;
    vector < int > fib_num;
    bool beginning = true;
    int maxFromFourBytes = 1114111;
    bool returningInfo = true;
    createFibonacciSequence ( maxFromFourBytes, fib_num );
    bool problemInNumbers = false;
    while ( true )
    {
        problemInNumbers = false;
        bool ok = readingByBytes ( inputFile, outputFile, fib_num, beginning, problemInNumbers );
        if ( ! ok )
        {
            if ( inputFile . eof ( ) )
            {
                string str = ""s;
                if ( problemInNumbers )
                    returningInfo = false;
                if ( writingToOutputFile ( outputFile, str, beginning, true ) )
                    break;
            }
            returningInfo = false;
            break;
        }
    }
    return cleaning ( inputFile, outputFile, returningInfo, fib_num );
}

/* ----------------------------------------------FIBONACCI---------------------------------------------*/

/* This function is used to cut first 8 elements of the binaryStr and convert them into one byte */
void fibMoveEightBitsToByte ( string & binaryStr, char & byte )
{
    string tmpBinaryStr = ""s;
    for ( size_t i = 0 ; i < 8 ; i ++ )
        tmpBinaryStr . push_back ( binaryStr[ i ] );
    binaryStr . erase ( 0, 8 );
    createByteFromStringSizeEight ( byte, tmpBinaryStr );
}

/* This function is used for writing the output into file */
bool fibWritingToFile ( ofstream & outputFile, string & binaryStr )
{
    char byteToBeWritten;
    bool returningInfo = true;
    while ( true )
    {
        if ( binaryStr . length ( ) >= 8 )
        {
            fibMoveEightBitsToByte ( binaryStr, byteToBeWritten );
            if ( ! outputFile . put ( byteToBeWritten ) || ! outputFile . good ( ) )
            {
                returningInfo = false;
                break;
            }
        }
        else
            break;
    }
    return returningInfo;
}

/* This function is responsible for fulfilling future UTF - 8 symbols in "the middle" */
void fibFillingMiddleBytes ( string & src, string & dst )
{
    for ( size_t i = src . length ( ) - 6 ; i < src . length ( ) ; i ++ )
        dst . push_back ( src[ i ] );
    src . erase ( src . length ( ) - 6, 6 );
}

/* This function is responsible for fulfilling future UTF - 8 symbols in "at the beginning" */
void fibFillingBeginningBytesWithZeros ( string & src, string & dst, size_t amountOfBytes )
{
    for ( size_t i = src . length ( ) ; i < amountOfBytes ; i ++ )
        dst . push_back ( '0' );
    dst . append ( src );
    src . clear ( );
}

/* This function creates UTF-8 with 4 bytes symbol from Fibonacci code */
void fibCreatingUTFFourBytesFromFibInt ( string & binaryStr )
{
    inverseStrLine ( binaryStr );
    string beginning = "11110";
    string middleFirst = "10";
    string middleSecond = "10";
    string middleThird = "10";
    fibFillingMiddleBytes ( binaryStr, middleThird );
    fibFillingMiddleBytes ( binaryStr, middleSecond );
    if ( binaryStr . length ( ) < 6 )
    {
        for ( size_t i = binaryStr . length ( ) ; i < 6 ; i ++ )
            middleFirst . push_back ( '0' );
        middleFirst . append ( binaryStr );
        binaryStr . clear ( );
    }
    else
        fibFillingMiddleBytes ( binaryStr, middleFirst );
    fibFillingBeginningBytesWithZeros ( binaryStr, beginning, 3 );
    beginning . append ( middleFirst );
    beginning . append ( middleSecond );
    beginning . append ( middleThird );
    binaryStr = beginning;
}

/* This function creates UTF-8 with 3 bytes symbol from Fibonacci code */
void fibCreatingUTFThreeBytesFromFibInt ( string & binaryStr )
{
    inverseStrLine ( binaryStr );
    string beginning = "1110";
    string middleFirst = "10";
    string middleSecond = "10";
    fibFillingMiddleBytes ( binaryStr, middleSecond );
    fibFillingMiddleBytes ( binaryStr, middleFirst );
    fibFillingBeginningBytesWithZeros ( binaryStr, beginning, 4 );
    beginning . append ( middleFirst );
    beginning . append ( middleSecond );
    binaryStr = beginning;
}

/* This function creates UTF-8 with 2 bytes symbol from Fibonacci code */
void fibCreatingUTFTwoBytesFromFibInt ( string & binaryStr )
{
    inverseStrLine ( binaryStr );
    string beginning = "110";
    string middle = "10";
    fibFillingMiddleBytes ( binaryStr, middle );
    fibFillingBeginningBytesWithZeros ( binaryStr, beginning, 5 );
    beginning . append ( middle );
    binaryStr = beginning;
}

/* This function creates ASCII symbol from Fibonacci code */
void fibCreatingASCIIFromFibInt ( string & binaryStr )
{
    for ( size_t i = binaryStr . length ( ) ; i < 8 ; i ++ )
        binaryStr . push_back ( '0' );
    inverseStrLine ( binaryStr );
}

/* This function is used to decode fibonacci number back into ASCII/UTF-8 symbols */
bool fibFindingUTFReprForFibInt ( int fibInt, string & binaryStr )
{
    binaryStr . clear ( );
    // fibonacci number cannot be negative
    if ( fibInt < 0 )
        return false;
    if ( fibInt == 0 )
        binaryStr . push_back ( '0' );
    else
    {
        while ( fibInt > 1 )
        {
            if ( fibInt % 2 )
                binaryStr . push_back ( '1' );
            else
                binaryStr . push_back ( '0' );
            fibInt /= 2;
        }
        binaryStr . push_back ( '1' );
    }

    if ( binaryStr . length ( ) <= 7 )
        fibCreatingASCIIFromFibInt ( binaryStr );
    else if ( binaryStr . length ( ) <= 11 )
        fibCreatingUTFTwoBytesFromFibInt ( binaryStr );
    else if ( binaryStr . length ( ) <= 16 )
        fibCreatingUTFThreeBytesFromFibInt ( binaryStr );
    else if ( binaryStr . length ( ) <= 21 )
        fibCreatingUTFFourBytesFromFibInt ( binaryStr );
    else
        return false;
    return true;
}

/* This function is used to calculate int number which was coded into fibonacci one,
 * it also checks if decoded number is valid */
bool fibCheckForLimitsAndIntCalculating ( string & fibStr, int & num, const vector < int > & fibNum )
{
    num = 0;
    bool returningInfo = true;
    for ( size_t i = 0 ; i < fibStr . length ( ) ; i ++ )
    {
        if ( fibStr[ i ] == '1' )
        {
            if ( fibNum . size ( ) > i )
                num += fibNum[ i ];
            else
            {
                returningInfo = false;
                break;
            }
        }
    }
    num --;
    if ( num > 1114111 || num < 0 )
        returningInfo = false;

    return returningInfo;
}

/* This function creates Fibonacci sequenced from the string fulfilled from the input.
 * To convert a symbol from Fibonacci code to ASCII/UTF-8 we need to get it first due to the fact,
 * that the input is read by bytes and Fibonacci symbol maybe be shorter or longer than 8*k, k from Z+ */
bool fibCreateFibSeqFromString ( string & origStr, string & fibStrNew, size_t endForFibSeq )
{
    fibStrNew . clear ( );
    if ( endForFibSeq >= origStr . length ( ) )
        return false;
    for ( size_t i = 0 ; i < endForFibSeq ; i ++ )
        fibStrNew . push_back ( origStr[ i ] );
    origStr . erase ( 0, endForFibSeq + 1 );
    return true;
}

/* This function searches for 11 = ending of the Fibonacci symbol */
bool fibSearchingForEnding ( string & fibStr, size_t & pos )
{
    bool returningFlag = false;
    pos = 0;
    for ( size_t i = 0 ; i < fibStr . length ( ) ; i ++ )
    {
        if ( fibStr[ i ] == '1' && i > 0 )
        {
            if ( fibStr[ i - 1 ] == '1' )
            {
                pos = i;
                returningFlag = true;
                break;
            }
        }
    }
    return returningFlag;
}

/* As the Fibonacci code can be padded with zeros in the end to become a whole byte,
 * this function checks for proper ending/padding */
bool fibCheckingLastBits ( string & fibStr )
{
    bool returningInfo = false;
    if ( fibStr . length ( ) > 7 )
        return returningInfo;
    for ( char i : fibStr )
    {
        if ( i == '1' )
            return returningInfo;
    }
    returningInfo = true;
    return returningInfo;
}

/* This function extracts bytes from the input till at least one whole Fibonacci symbol is found */
bool fibGettingBytesUntilFullFibNumIsFound ( ifstream & inputFile, string & fibStr, size_t & endingForFibSeq )
{
    char byte;
    string tmp = ""s;
    bool returningInfo = true;
    while ( ! fibSearchingForEnding ( fibStr, endingForFibSeq )
            && fibStr . length ( ) < 40 )
    {
        tmp . clear ( );
        if ( ! getByteFromInputFile ( inputFile, byte ) )
        {
            returningInfo = false;
            break;
        }
        createStringFromOneByte ( byte, tmp );
        inverseStrLine ( tmp );
        fibStr . append ( tmp );
    }
    return returningInfo;
}

/* As we do not have enough memory to store the whole output, we will write one decoded symbol to output file */
bool writingToOutputFileUntilFullFibNumIsFound ( ifstream & inputFile, ofstream & outputFile, string & fibStr,
                                                 size_t endingForFibSeq, const vector < int > & fibNum )
{
    bool returningInfo = true;
    while ( endingForFibSeq )
    {
        int numInUtf;
        string fibStrToBeWritten = ""s;
        if ( ! fibCreateFibSeqFromString ( fibStr, fibStrToBeWritten, endingForFibSeq ) )
        {
            returningInfo = false;
            break;
        }
        if ( ! fibCheckForLimitsAndIntCalculating ( fibStrToBeWritten, numInUtf, fibNum )
             || ! inputFile . good ( ) || ! outputFile . good ( )
             || inputFile . fail ( ) || outputFile . fail ( ) )
        {
            returningInfo = false;
            break;
        }
        if ( ! fibFindingUTFReprForFibInt ( numInUtf, fibStrToBeWritten ) )
        {
            returningInfo = false;
            break;
        }
        if ( ! fibWritingToFile ( outputFile, fibStrToBeWritten ) || ! inputFile . good ( ) || ! outputFile . good ( )
             || inputFile . fail ( ) || outputFile . fail ( ) )
        {
            returningInfo = false;
            break;
        }
        fibSearchingForEnding ( fibStr, endingForFibSeq );
    }
    return returningInfo;
}

/* This function is used for reading from the input, checking for errors and writing to the output file.
 * Depending on weather we need to check for proper ending or extracting bytes and writing symbol to the output,
 * True or False is returned */
bool fibReadingByBytes ( ifstream & inputFile, ofstream & outputFile, const vector < int > & fibNum,
                         bool & beginning, bool endCheck )
{
    static string fibStr;
    bool returningInfo;
    if ( endCheck )
        return fibCheckingLastBits ( fibStr );
    if ( beginning )
    {
        fibStr . clear ( );
        beginning = false;
    }

    string tmp = ""s;
    size_t endingForFibSeq = 0;

    if ( ! fibGettingBytesUntilFullFibNumIsFound ( inputFile, fibStr, endingForFibSeq ) )
        returningInfo = false;
    else
        returningInfo = writingToOutputFileUntilFullFibNumIsFound ( inputFile, outputFile, fibStr,
                                                                    endingForFibSeq, fibNum );

    if ( ! inputFile . good ( ) || ! outputFile . good ( ) || fibStr . length ( ) >= 39 )
        returningInfo = false;
    return returningInfo;
}

/* This function is used for conversion Fibonacci symbols to ASCII/UTF-8 and checking for the correctness
 * of the processes and symbols */
bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
    ifstream inputFile;
    inputFile . open ( inFile );
    if ( ! inputFile . is_open ( ) )
        return false;
    ofstream outputFile;
    outputFile . open ( outFile );
    if ( ! outputFile . is_open ( ) )
        return false;

    vector < int > fibNum;
    int maxFromFourBytes = 1114113;
    createFibonacciSequence ( maxFromFourBytes, fibNum );
    bool ok;
    bool beginning = true;
    while ( true )
    {
        ok = fibReadingByBytes ( inputFile, outputFile, fibNum, beginning, false );
        if ( ! ok || ! inputFile . good ( ) || ! outputFile . good ( ) )
        {
            if ( inputFile . eof ( ) )
            {
                ok = fibReadingByBytes ( inputFile, outputFile, fibNum, beginning, true );
                if ( ok )
                    return cleaning ( inputFile, outputFile, true, fibNum );
            }
            return cleaning ( inputFile, outputFile, false, fibNum );
        }
    }
}

/* This function is used for checking the correctness of conversion. It takes two files:
 * the first one consists of symbols we have decoded ourselves and the second of has right symbols */
bool identicalFiles ( const char * file1, const char * file2 )
{
    ifstream firstFile ( file1, ios::binary | ios::ate );
    ifstream secondFile ( file2, ios::binary | ios::ate );

    if ( ! firstFile . is_open ( ) || ! secondFile . is_open ( ) )
        return false;


    if ( firstFile . tellg ( ) != secondFile . tellg ( ) )
        return false;

    firstFile . seekg ( 0 );
    secondFile . seekg ( 0 );

    char symFirstFile, symSecondFile;
    while ( firstFile . get ( symFirstFile ) && secondFile . get ( symSecondFile ) )
    {
        if ( symFirstFile != symSecondFile )
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
