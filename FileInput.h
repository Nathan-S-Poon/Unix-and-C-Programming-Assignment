
  
/*************************************************
*fileInput functions
*****************************************************/
 
#ifndef FILEINPUT_H
#define FILEINPUT_H
void readBoardFile(Board* board, char* fileName);
Ship* createShip(char location[], char direction[], int length, char* name);
void readMissileFile(LinkedList* list, char* fileName);
int checkBoardForError(int width, int height, char location[], 
                       char direction[], int length, char* name);
void stringToUpper(char* word);
#endif
 

#ifndef BOOLEAN
#define BOOLEAN
    #define FALSE 0
    #define TRUE !FALSE
#endif







