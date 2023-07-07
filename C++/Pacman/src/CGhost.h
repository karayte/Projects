#ifndef CGHOST_H
#define CGHOST_H
#include <string>
#include <algorithm>
#include "CPacman.h"
#include <queue>
#include <random>
#include <map>
#include <set>

#define SCORE_FOR_DEAD_SPIRIT 200

using namespace std;

/**
     * @brief An abstract class to represent each type of the Spirits.
     *
     * This class is responsible for spirits movements and actions
     * between spirits and Pacman.
     */
class CGhost {
public:
    /**
     * @brief A public variable is responsible for storing the
     * information towards each sign of the spirits. Is used
     * for detection if the coordinates of the game field are
     * already taken by one of the spirits.
     */
    static set < char > mAllSignsOfSpirits;

    /**
     * @brief Constructor to create an object in s subclass.
     * @param coordinates represent the coordinates the spirit will have.
     * @param sign represents the sign of the spirit. Spirits may not have
     * the same sign.
     * @param pacman is used to control Pacman movements.
     * @param vulnerableSign represents the sign of the spirits when
     * Pacman has activated its Power Mode allowing eating spirits.
     */
    CGhost ( const pair < size_t, size_t > & coordinates, char sign, CPacman * pacman, char vulnerableSign,
             char deadSign, char wallLabyrinthSign );

    CGhost ( size_t row, size_t column, char sign, CPacman * pacman, char vulnerableSign, char deadSign,
             char wallLabyrinthSign);

    pair < size_t, size_t > getCoordinates () const;

    /**
     * @brief Is implemented in each subclass and is used for spirits'
     * movements and control if they step on the same place as Pacman
     * is at. It also modifies the game field in order to show the
     * dynamic changes on it.
     * @param fieldWidth is used to represent the maximum of x
     * coordinate value on the game field.
     * @param fieldHeight is used to represent the maximum of y
     * coordinate value on the game field.
     * @param field represents the game field.
     */
    virtual void move ( size_t fieldWidth, size_t fieldHeight, vector < string > & field ) = 0;

    /**
     * @brief Depending on whether the Power Mode of Pacman is active,
     * a spirit can be eaten by Pacman or Pacman by the spirit.
     */
    void action ();

    /**
     * @brief Changes the signs of all the spirits to the same vulnerable
     * one to show Pacman can eat a spirit and back to their usual signs.
     * @param mode is true to indicate the spirits are vulnerable and
     * can be eaten or is false to bring back theirs abilities to eat.
     */
    void switchVulnerableMode ( bool mode );

    char getSign () const;

    /**
     * @brief Makes the spirit disappear from the game field. Once the spirit
     * was eaten, it cannot influence game process.
     */
    void die ();

    /**
     * @brief Is used to maintain the field and store the previous sign of
     * the part of the game field a spirit stepped on.
     * @param field represents the game field.
     */
    void changeFieldAndSavePrevSign ( vector < string > & field );

    /**
     * @brief Is used to set the new coordinates of the spirit, bring
     * back the previous sign of the field and show spirit's new location
     * on the game field.
     * @param coordinates represents new coordinates of the spirit.
     * @param field represents the game field.
     */
    void setCoordinatesAndPrintPrevSign ( const pair < size_t, size_t > & coordinates, vector < string > & field );

    /**
     * @brief Finds mostly the closes part of the field a spirit should
     * step on to follow the Pacman.
     * @param start represents current position of the spirit.
     * @param target represents the position the spirit should go to.
     * @param field represents the game field.
     */
    pair < size_t, size_t > BFS ( pair < size_t, size_t > start, pair < size_t, size_t > target, const vector < string > & field ) const;

    /**
     * @brief Is responsible for saving spirits' lives when the Power Mode
     * of Pacman is active. It finds the first place the spirit can go to
     * to make increase the distance between the spirit and Pacman.
     * Uses Breadth-first search algorithm to find the best place to step
     * on.
     * @param field represents the game field.
     * @return new coordinates the spirit should be at.
     */
    void moveToAvoidPacman ( vector < string > & field );

    /**
     * @brief Checks if the place on the game field is free to step on.
     * @param row represents y coordinate on the game field.
     * @param column represents x coordinate on the game field.
     * @param field represents the game field.
     */
    bool canStepOn ( size_t row, size_t column, const vector < string > & field ) const;

protected:
    pair < size_t, size_t > mCoordinates;
    char mSign;
    char mPrevFieldSign;
    char mVulnerableModeSign;
    /**
     * @brief Is the same as an empty cell sign to show the spirit is gone.
     */
    char mDeadSign;
    char mWallLabyrinthSign;
    CPacman * mPacman;
    bool mVulnerableModeActivated;
    /**
     * @brief Is true when the spirit is gone and is false when its alive.
     */
    bool mIsDead;
    /**
     * @brief Is used to decide the direction the spirit can and should go.
     */
    vector < pair < int, int > > directions = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
};

#endif //CGHOST_H
