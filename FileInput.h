
  
/*************************************************
*fileInput functions
*****************************************************/
 
#ifndef FILEINPUT_H
#define FILEINPUT_H
void readBoardFile(Board* board, char* fileName);
Ship* createShip(int location[], char direction[], int length, char* name);
void readMissileFile(LinkedList* list, char* fileName);
int checkBoardForError(int width, int height, int location[], 
                       char direction[], int length, char* name);
void stringToUpper(char* word);
int letterToNum(char letter[]);
#endif
 






