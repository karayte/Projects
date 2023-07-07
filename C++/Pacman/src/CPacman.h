#ifndef CPACMAN_H
#define CPACMAN_H
#include <vector>
#include <chrono>

using namespace std;

#define NUM_OF_LIVES 1


/**
 * @brief A class to represent Pacman itself.
 *
 * This class is responsible for making movements of Pacman,
 * activating its Power Mode for eating spirits and handle
 * its score during the game process.
 */
class CPacman
{
public:
    CPacman ();

    /**
     * @brief Is responsible for representing Pacman death
     * by decreasing its amount of lives.
     */
    void die ();

    void setSign ( char sign );

    void increaseScore ( int value );

    void setPowerMode ( bool mode );

    /**
     * @brief Is used to check whether Pacman has its Power Mode still
     * active or not.
     * @return true in case the mode is active, otherwise false.
     */
    bool isPowerMode ();

    /**
     * @brief Is used to indicate whether Pacman is alive or not.
     * @return true in case Pacman is gone, otherwise false.
     */
    bool isDead () const;

    pair < size_t, size_t > getCoordinates () const;

    void setCoordinates ( const pair < size_t, size_t > & coordinates );

    char getSign () const;

    int getScore () const;

    /**
     * @brief Is used to bring back the beginning information
     * towards Pacman like decreasing score back to zero,
     * increasing amount of lives back to default value and
     * unable Power Mode.
     */
    void reset ();

private:
    int mScore;
    /**
     * @brief Is used to indicate whether Power Mode is active
     * or not. Has true when it is and false when it's down.
     */
    bool mPowerMode;
    pair < size_t, size_t > mCoordinates;
    /**
     * @brief Is used to measure time the Power Mode will be
     * active for.
     */
    chrono :: steady_clock :: time_point mBeginningOfPowerMode;
    char mSign;
    size_t mNumOfLives;
};

#endif //CPACMAN_H
