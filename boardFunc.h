/*
header file for boardFile.c
*/


#ifndef BOARDFILE_H
#define BOARDFILE_H
Board* constructBoard();
void displayBoard(char** displayArray, int height, int width);
void create2DTemplate(char** array, int height, int width);
void createGameBoard(Board* boardFile, char** boardArray);
void addShipToBoard(int location[], char direction[], int length, char** board);
void free2DArray(char*** array, int length);
#endif




