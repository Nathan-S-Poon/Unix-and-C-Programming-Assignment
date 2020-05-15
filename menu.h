/*
menu header file
*/

#ifndef MENU_H
#define MENU_H
int menuInput();
void playGame(char** displayArray, char** boardArray, Board* boardinfo,
             LinkedList* missList, int height, int width);
void printMissile(void* data);
void targetInput(int target[], int height, int width, Missile* curMissile);

#endif 








