/*******************************************************
*game function header file
*
*
********************************************************/


#ifndef MISSILEFUNCTION_H
#define MISSILEFUNCTION_H
void checkShipDestroyed(Board* board, int col, int row);   
void single(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board);
void splash(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board);
void hLine(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board);
void vLine(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board);
void printMissile(void* data);
#endif 


















