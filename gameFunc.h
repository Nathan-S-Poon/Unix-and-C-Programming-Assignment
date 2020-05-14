/*******************************************************
*game function header file
*
*
********************************************************/


#ifndef GAMEFUNCTION_H
#define GAMEFUNCTION_H
void checkShipDestroyed(Board* board, int col, int row);   
void single(char** displayBoard, char** answerBoard, int location[], 
           Board* board);
void splash(char** displayBoard, char** answerBoard, int location[], 
           Board* board);
void hLine(char** displayBoard, char** answerBoard, int location[], 
           Board* board);
void vLine(char** displayBoard, char** answerBoard, int location[], 
           Board* board);
#endif 


















