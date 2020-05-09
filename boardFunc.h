/*
header file for boardFile.c
*/


#ifndef BOARDFILE_H
#define BOARDFILE_H
Board* constructBoard();
void displayBoard(char** displayArray, int height, int width);
char** create2DTemplate(int height, int width);
int createGameBoard(Board* boardFile, char** boardArray);
int addShipToBoard(int location[], char direction[], int length, char** board);
void free2DArray(char** array, int length);
void resetBoard(Board* board);
#endif




