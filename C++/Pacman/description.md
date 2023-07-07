    Welcome to the description of the semester work called Pacman and created by Karolina Z. Below is the explanation of each parts of the
    program divided into paragraphs to make the information more organized and easy to get & find.
    The main point of this work is to create a game which reminds wellknown PACMAN, but has some different and exciting features instead of
    old one. If you have never played it or heard about it, please, follow this link: https://en.wikipedia.org/wiki/Pac-Man .
    
Map:
    Given map should follow some very important rules connected to the game implementation, which are:
        1) The first row contains 2 characters only with spaces between them :
            a) the first one represents an empty cell;
            b) the second one represents a cell for walls and labyrinths;
        2) Then seven rows contain one symbol and positive coordinates ( row column ) of the character(s):
            a) the first   row shows us symbol of   Pacman;
            b) the second  row shows us symbol of a spirit   which follows Pacman;
            c) the third   row shows us symbol of a spirit   which moves in straight directions;
            d) the fourth  row shows us symbol of a spirit   which is controlled by using keyboard and "WASD" buttons;
            e) the fifth   row shows us symbol of a coin     which can be eaten by Pacman and gives score of 10 points;
            f) the sixth   row shows us symbol of a berry    which can be eaten by Pacman and gives it the ability to eat spirits within 5
               seconds;
            g) the seventh row shows us symbol of a teleport which gives Pacman the opportunity to be randomly transferred to the other
               teleportation place;
            In case of coins and berries, there can be more than one pair of coordinates. If that is so, each pair should be written
            without any extra symbols, only numbers and spaces are allowed.
        3) After the eighth row the main game field goes. It should not contain the symbols mentioned before except empty cells and
           labyrinths. If it does or there are not all of the mentioned above characters and there are less or more than 8 rows right
           before field image, the game will not begin and the "Game over" sentence will appear with the leader board. We are expecting
           to receive a field with walls only.
        4) None of the characters can be initialized with the sign '?' - for vulnerable mode of spirits.
        5) By pressing the button "Backspace" the game will be finished, by pressing the button "Enter" new game will be started.
        6) None of the characters can have 0 in their coordinates at the beginning.
        7) A field must have a shape of rectangular or square.
        8) If the field will be bigger than terminal window, the game will be almost unplayable. Be aware to define a field according to
           your screen size.
        9) If given coordinates of each character will be the same as coordinates of walls or labyrinths, the game will not launch.
           Same way wrong coordinates will be treated.

Pacman:
    In this game Pacman has its own sign and can move in 4 directions:
        a) "UP"    by pressing up    key;
        b) "DOWN"  by pressing down  key;
        c) "LEFT"  by pressing left  key;
        d) "RIGHT" by pressing right key;
    It can eat coins and gain plus 10 score or berries and enable its power mode, which allows it to eat spirits, and gain plus 100 score.
    It can be eaten by the spirits and become dead. After pacman looses its life, the game will be over and you'll see your score on the
    leader's board. Pacman can use some of the teleports to appear in the other part of the field. It can happen by stepping on the
    teleport symbol.

Spirits:
    There are 3 types of spirits and all of them have their own sign ( can be repeated ):
        a) the first  one always follows Pacman;
        b) the second one tries to predict next step of Pacman and help the first one to catch it;
        c) the third  one moves randomly.
    Spirits can be vulnerable if Pacman eats a berry. In this case they can be eaten by Pacman within 5 seconds and their signs will be
    modified to the sign '?', so the player can see if there is a chance to eat a spirit or not. If the spirits are not in a "vulnerable"
    mode, they will hunt Pacman and may eat it if they catch it by stepping at the same place where Pacman is at the moment.
    Unfortunately, spirits cannot be teleported, as they are not flesh and blood made anymore.

Coins:
    These characters on the game field can increase Pacman's score and if it takes a coin, the coin will not appear again.

Berries:
    If Pacman eats a berry, it will not appear again, other features were mentioned above.

Teleports:
    If Pacman steps on a field with the mark of a teleport, pacman will be immediately transferred one cell near to another teleport cell.
    Teleports do not disappear. In case the teleport is hidden between the walls from 4 sides, then Pacman will be teleported no where
    and no one could find it. There must be at least 2 teleports, otherwise the game will not be launched.

Polymorphism:
    In this program polymorphism appears in the following situations:
        a) coins, berries and teleports have the same predecessor - abstract class named CGameEntity. As they have common features and
        actions, they can represented by CGameEntity object and play actions they need. It allows us to save code's space, get rid of
        unnecessary if closures and avoid repeated parts. For example, coin has an action of increasing score for Pacman, berry as its
        action set Pacman into its power mode and teleport has an action to transfer Pacman on a different location. These 3 characters
        also have common parameters which are stored in predecessor class.
        b) spirits can be also combined into one object type in this game using inheritance from abstract class called CGhost, as they
           all have the same moves and actions but realised their own ways. it's much easier to handle programming by using one object
           which definitively has methods you need rather than always ask if the object if a type of "x" and call some methods only it has.
    Reasons we need polymorphism here:
        a) it organizes the code and makes it more understandable and maintainable.
        b) it allows to reuse code. All common properties and behaviors can be defined in the CGameEntity or CGhost classes and reused in
           all subclasses.
        c) with polymorphism, we can write more flexible and dynamic code. For instance, we can maintain a list of CGameEntity pointers or
           references that can hold any game entities ( Coin, Berry, etc.). This way, we can loop through all game entities and call their
           functions without even knowing their actual class.
        d) it allows subclasses to have different implementations for the same method defined in the superclass.
    There can appear some other situations where we can use polymorphism and inheritance depending on the future implementation. This
    description is only a start point of the work which can be modified, replaced, deleted or newly created.
    
This variant of the program is not the final version of it, so there may be some changes in any of the classes or files, but the basics of
the code implementation were described almost fully.
Also a leader board will added to the implementation as well as the definition of various levels of difficulties as soon as possible.
Thank you for reading it :)    
