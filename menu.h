/*
menu header file

*/

#ifndef MENU_H
#define MENU_H
int menuInput();
void playGame(char** displayBoard, char** answerBoard, LinkedList* missileList,
             int height, int width);
void printMissile(void* data);


#endif 








