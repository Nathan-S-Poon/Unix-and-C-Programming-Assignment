
  
/*************************************************
*fileInput functions
*****************************************************/
 
#ifndef FILEINPUT_H
#define FILEINPUT_H
int readBoardFile(Board* board, char* fileName);
Ship* createShip(int location[], char direction[], int length, char* name);
int readMissileFile(LinkedList* list, char* fileName);
int checkBoardForError(int width, int height, int location[], 
                       char direction[], int length, char* name);
void stringToUpper(char* word);
int letterToNum(char letter[]);
#endif
 






