/*
Contains structs
Ship struct
Board struct
Missile struct
missile function pointer
*/

#ifndef BOOLEAN
#define BOOLEAN
    #define FALSE 0
    #define TRUE !FALSE
#endif


#ifndef STRUCTSDEF
#define STRUCTSDEF
/******************************************
 *Ship - struct that contains ship's
 * locations, direction, lenght and name
 *****************************************/
typedef struct Ship
{
    char direction;/*should be stored as upper case*/
    int location[2];
    char* name;
    int length; 
    int destroyed;/*how many are destroyed*/   

} Ship;

/******************************************
 *Board - struct that contains array of 
 * Ships, num of ships, width and height
 *****************************************/
typedef struct Board
{
    int width;
    int height;
    int numShips;
    LinkedList* shipList; 
    int destroyed;

} Board;


/**************************************
*takes in displayboard, gameBoard, location
*height and width
**************************************/
typedef void(*MissileFunc)(char***, char***, int[], Board* board);

/**************************************
 *missile
 **************************************/
typedef struct Missile
{
    MissileFunc funcPtr;
    char name[7];
    char* description;
} Missile;
#endif



