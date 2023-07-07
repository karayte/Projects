#ifndef CGAMEMAP_H
#define CGAMEMAP_H
#include <unistd.h>
#include <vector>
#include <ncurses.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>

using namespace std;

/**
 * @brief A class for representation of the game field.
 *
 * Is responsible for handling all the changes and dynamic
 * representation during the game process.
 */
class CGameMap
{
public:
    /**
     * @brief Constructor to create an object of the class.
     * @param emptyCellSign represents a free field to step on.
     * @param wallLabyrinthSigh represents a piece of wall or
     * labyrinth.
     */
    CGameMap ( char emptyCellSign, char wallLabyrinthSigh );

    /**
     * @brief Is responsible for setting the game field from given map
     * and detection of incorrect map.
     * @param file represents the file current map is in.
     * @param allCoordinates is used to check the correctness of all
     * characters beginning coordinates.
     * @return false in case the map has not the shape of rectangle or square,
     * more than zero rows and more than zero columns, or in case map has
     * collisions in coordinates of the characters and walls or labyrinth.
     * True in case the map is correct.
     */
    bool readMapFromFile ( istream & file, const vector < pair < size_t, size_t > > & allCoordinates );

    /**
     * @brief Prints the current usage of the game field.
     */
    void printGameField () const;

    /**
     * @brief checks if the line from the input is correct, mainly if it
     * contains only signs of empty cell or pieces of wall and labyrinth.
     * @param row represents the input line.
     * @return false in case the input line contains forbidden signs.
     * True in case the input line contains only allowed signs.
     */
    bool isRowValid ( const string & row ) const;

    /**
     * @brief Is used to erase the current game field information to start
     * a new game.
     */
    void reset ();

    /**
     * @brief Is used to check if the coordinates of all the characters
     * do not have collisions with walls or labyrinth. Collisions in
     * characters coordinates are allowed but can influence the beginning
     * of the game process, especially if there is a collision in
     * coordinates of Pacman and a Spirit.
     * @param row represents y on the field.
     * @param column represents x on the field.
     * @return false in case a piece of wall or labyrinth has the same
     * coordinates as at least one of the characters, otherwise is true.
     */
    bool canStepOn ( size_t row, size_t column ) const;

    /**
     * @brief Uses method canStepOn to check the correctness of coordinates
     * of each character.
     * @param allCoordinates represents each character's coordinates.
     * @return the same value as the returning one from canStepOn.
     */
    bool checkAllCharactersCoordinates ( const vector < pair < size_t, size_t > > & allCoordinates ) const;

    /**
     * @brief Allows to change the game field during the game process.
     * @param sign represents the sign field place will be changed on.
     * @param coordinates represents which cell of the game field will
     * be changed.
     */
    void changeFieldSign ( char sign, const pair < size_t, size_t > & coordinates );

    /**
     * @brief A public variable responsible for storing the game field.
     */
    vector < string > mField;

private:
    size_t mGameFieldHeight;
    size_t mGameFieldWidth;
    char mEmptyCellSign;
    char mWallLabyrinthSigh;
};

#endif // CGAMEMAP_H