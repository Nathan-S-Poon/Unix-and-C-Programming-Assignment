/*******************************************************************8
 *Program:missileFunc
 *Date:14/4/2020
 *Description: stores funtions relating to missile, including the 
 *functions for splash, single, v-line and h-line
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "missileFunc.h"
#include "boardFunc.h"

/**********************************
MISSILE FUNCTIONS
**********************************/

/********************************
*single
*hits a single tile where location
*is
**********************************/
void single(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board)
{
    /*variables*/
    int col, row;/*coordinates*/
    col = location[0];
    row = location[1];
    if(strcmp(answerBoard[row][col], "0") == 0)/*set to shot if it matches answers*/
    {
        strncpy(displayBoard[row][col], "0", 2);
        strncpy(answerBoard[row][col], "1", 2);/*mark off as being destroyed*/
        /*checks if ship has been destroyed*/        
        checkShipDestroyed(board, col, row); 
    } 
    else if(strcmp(answerBoard[row][col], "#") == 0)
    {
        strncpy(displayBoard[row][col], "X", 2);
    }

}

/********************************
*v-line
*hits an entire column
*
**********************************/
void vLine(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board)
{
    /*variables*/
    int col, ii;
    col = location[0]; 
    for(ii = 1; ii <= (*board).height; ii++)/*iterate through column*/
    {    
        if(strcmp(answerBoard[ii][col],"0") == 0)/*set to shot if it matches answers*/
        {
            strncpy(displayBoard[ii][col], "0", 2);
            strncpy(answerBoard[ii][col], "1", 2);
            checkShipDestroyed(board, col, ii); 
        } 
        else if(strcmp(answerBoard[ii][col], "#") == 0)
        {
            strncpy(displayBoard[ii][col], "X", 2);
        }
    }

}

/********************************
*h-line
*hits an entire row
*
**********************************/
void hLine(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board)
{
    /*variables*/
    int row, ii;
    row = location[1];
    for(ii = 1; ii <= (*board).width; ii++)/*iterate through row*/
    {    
        if(strcmp(answerBoard[row][ii], "0") == 0)/*set to shot if it matches answers*/
        {
            strncpy(displayBoard[row][ii], "0", 2);
            strncpy(answerBoard[row][ii], "1", 2);
            checkShipDestroyed(board, ii, row); 
        } 
        else if(strcmp(answerBoard[row][ii], "#") == 0)
        {
            strncpy(displayBoard[row][ii], "X", 2);
        }
    }
}

/********************************
*splash
*hits 3x3 square around location
*
**********************************/
void splash(char*** displayBoard, char*** answerBoard, int location[], 
           Board* board)
{
    /*variables*/
    int ii, jj, col, row;/*coordinates*/
    int startR, startC, endR, endC;
    col = location[0];
    row = location[1];
    /*set a start and end of a matrix around location*/
    startC = col - 1;
    startR = row - 1;
    endR = row + 1;
    endC = col + 1;
    /*move in matrix borders if outside the game board*/
    if(col == 1)
    {
        startC = 1;
    }   
    if(row == 1)
    {
        startR = 1;
    }
    if(endR > (*board).height)
    {
        endR--;
    }
    if(endC > (*board).width)
    {
        endC--;
    }
    /*loop through affected area of board*/
    for(ii = startR; ii <= endR; ii++)
    {
        for(jj = startC; jj <= endC; jj++)
        {   /*if ship is hit*/
            if(strcmp(answerBoard[ii][jj], "0") == 0)
            {
                strncpy(displayBoard[ii][jj], "0", 2);
                strncpy(answerBoard[ii][jj], "1", 2);
                checkShipDestroyed(board, jj, ii); 
            }
            else if(strcmp(answerBoard[ii][jj], "#") == 0)
            {
                strncpy(displayBoard[ii][jj], "X", 2);
            }
        }
    }
}


/*******************************************
*iterates for ship that has been hit
*checks if ship has been destroyed
*adds to num that have been destroyed
*Input: board, column and row that was hit 
*******************************************/
void checkShipDestroyed(Board* board, int col, int row)
{
    int found, ii, curCol, curRow;
    ListNode* curNode;
    Ship* curShip;
    found = 0; 
    curNode = board->shipList->head;
    /*while location hasn't been found and more nodes exists*/ 
    while((!found)&&(curNode != NULL))
    {
        curShip = (Ship*)curNode->data;/*get the current ship*/
        curCol = curShip->location[0];
        curRow = curShip->location[1];    
        ii = 1;
        /*loop through entire ship*/
        while((ii <= curShip->length)&&(!found))
        { 
            /*loop until every position in ship is compared*/
            if((curCol == col)&&(curRow == row))
            {
                (curShip->destroyed)++;
                if(curShip->destroyed == curShip->length)
                {/*if ship has been destroyed add to board and print message*/
                    printf("ship has been destroyed: %s", curShip->name);
                    (board->destroyed)++; 
                }
                found++;    
            }
            /*get next part of the ship*/
            switch(curShip->direction)
            { 
                case 'N': 
                    curRow++; 
                break;
                case 'S':
                    curRow--;
                break;
                case 'E':
                    curCol--;
                break;
                case 'W':
                    curCol++;
                break; 
            }
            ii++;
        }
        curNode = curNode->next;
    }
   
}




/***********************************
*printMissile
*prints missile name from missile struct
*To be used by printLinkedList
************************************/
void printMissile(void* data)
{
    Missile* missile;
    missile = (Missile*)data;
    printf("%s\n", missile->name);
}





 








