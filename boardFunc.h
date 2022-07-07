/*
header file for boardFile.c
*/

#ifndef COLOURS 
#define COLORS

#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define YELLOW "\033[01;33m"
#define MAGENTA "\033[1;35m"

#endif


#ifndef BOARDFILE_H
#define BOARDFILE_H
Board* constructBoard();
void displayBoard(char*** displayArray, char*** answer, int height, int width);
char*** create2DTemplate(int height, int width);
Ship* createShip(int location[], char direction, int length, char* name);
int createGameBoard(Board* boardFile, char*** boardArray);
int addShipToBoard(int location[], char direction, int length, char*** board);
void free2DArray(char*** array, int length, int width);
void resetBoard(Board* board);
void freeShip(void* data);
#endif




