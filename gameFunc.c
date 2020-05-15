/*******************************************************************8
 *Program: gameFunc
 *Date:14/4/2020
 *Description: stores funtions that are used when player is playing the
 *game
 *
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "gameFunc.h"
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
    if(answerBoard[row][col] == '0')/*set to shot if it matches answers*/
    {
        displayBoard[row][col] = '0';
        answerBoard[row][col] = '1';/*mark off as being destroyed*/
        /*checks if ship has been destroyed*/        
        checkShipDestroyed(board, col, row); 
    } 
    else if(answerBoard[row][col] == '#')
    {
        displayBoard[row][col] = 'X';
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
        if(answerBoard[ii][col] == '0')/*set to shot if it matches answers*/
        {
            displayBoard[ii][col] = '0';
            answerBoard[ii][col] = '1';
            checkShipDestroyed(board, col, ii); 
        } 
        else if(answerBoard[ii][col] == '#')
        {
            displayBoard[ii][col] = 'X';
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
        if(answerBoard[row][ii] == '0')/*set to shot if it matches answers*/
        {
            displayBoard[row][ii] = '0';
            answerBoard[row][ii] = '1';
            checkShipDestroyed(board, ii, row); 
        } 
        else if(answerBoard[row][ii] == '#')
        {
            displayBoard[row][ii] = 'X';
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
        {
            if(answerBoard[ii][jj] == '0')
            {
                displayBoard[ii][jj] = '0';
                answerBoard[ii][jj] = '1';
                checkShipDestroyed(board, jj, ii); 
            }
            else if(answerBoard[ii][jj] == '#')
            {
                displayBoard[ii][jj] = 'X';
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
    while((found != 1)&&(curNode != NULL))
    {
        curShip = (Ship*)curNode->data;/*get the current ship*/
        curCol = curShip->location[0];
        curRow = curShip->location[1];    
        ii = 1;
        /*loop through entire ship*/
        while((ii <= curShip->length)&&(found != 1))
        { 
            /*loop until every position in ship is compared*/
            if((curCol == col)&&(curRow == row))
            {
                (curShip->destroyed)++;
                if(curShip->destroyed == curShip->length)
                {/*if ship has been destroyed add to board and print message*/
                    printf("ship %shas been destroyed\n", curShip->name);
                    (board->destroyed)++; 
                }
                found++;    
            }
            /*get next part of the ship*/
            switch(curShip->direction[0])
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







 








