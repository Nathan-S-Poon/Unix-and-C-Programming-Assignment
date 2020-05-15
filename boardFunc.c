/* *****************************************************************************
*Program: BoardFunc
*Date: 10/4/2020
*Description: contains gane and interface functions. 
*Display board
*missiles left
*name of current missile
*input next coordinate 
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "boardFunc.h"

/**********************************************
 *createGameBoard
 *takes contents of board and puts in a 2d array
 ***********************************************/
int createGameBoard(Board* boardFile, char** boardArray)
{ 
    /*variables*/
    ListNode* curNode;
    Ship* curShip;
    int error;
    error = FALSE;
    /*iterate through shipList and add coordinates to board*/
    curNode = boardFile->shipList->head;
    while((curNode != NULL)&&(error == FALSE))
    {     
        curShip = ((Ship*)curNode->data);       
        error = addShipToBoard(curShip->location, curShip->direction,
        curShip->length, boardArray);            
        curNode = curNode->next;
    }
    return error;
}

/**********************************************
 *create2DTemplate
 *creates 2D array to be used to make the board
 *containing the locations of ships and the board
 *that is displayed to player
 ***********************************************/
char** create2DTemplate(int height, int width)
{
    /*variables*/
    char** array;
    int ii, jj;
    char xAxis, yAxis;
    xAxis = 'A';  
    yAxis = '1';
  
    /*malloc 2d array of strings*/
    array = (char**)malloc(height * sizeof(char*)); 
    for(ii = 0; ii < height; ii++)
    {
        array[ii] = (char*)malloc(width * sizeof(char));
    }   
  
    /*fill boardArray corner*/    
    array[0][0] = ' ';
    /*fill X axis with letters*/ 
    for(ii = 1; ii < width; ii++)
    {
        array[0][ii] = xAxis; 
        xAxis++;
    }
    /*fill y axis with numbers*/
    for(ii = 1; ii < height; ii++)
    {
        array[ii][0] = yAxis;
        yAxis++; 
    }
    
    for(ii = 1; ii < height; ii++)
    {
        for(jj = 1; jj < width; jj++)
        {
            array[ii][jj] = '#';
        }
    }
    return array; 
}

/**********************************************
 *displayBoard
 *outputs 2D array with borders
 *boardArray keeps data on ship locations
 *displayArray is shown to player
 ** *********************************************/
void displayBoard(char** displayArray, char** answer, int height, int width)
{
    int ii, jj;
    
    
    for(ii = 0; ii < height; ii++)
    {
        for(jj = 0; jj < width; jj++)
        {   /*print | 'char' | for each*/
            printf("|");
            /*print colour only if mono not defined*/
            #ifndef MONO 
            switch(displayArray[ii][jj])
            {/*changes colour depenfing on type*/
                case '#':/*conditional debug to find ships*/
                    #ifdef DEBUG
                    if(answer[ii][jj] == '0')
                    {
                        printf(" %s%c%s |", MAGENTA, displayArray[ii][jj], RESET);    
                    }
                    else
                    {
                        printf(" %s%c%s |", CYAN, displayArray[ii][jj], RESET);    
                    }
                    #endif
                    #ifndef DEBUG
                    printf(" %s%c%s |", CYAN, displayArray[ii][jj], RESET);   
                    #endif      
                break;
                case 'X':
                    printf(" %s%c%s |", RED, displayArray[ii][jj], RESET);    
                break;
                case '0':
                    printf(" %s%c%s |", GREEN, displayArray[ii][jj], RESET);    
                break;
                default:
                    printf(" %s%c%s |", YELLOW, displayArray[ii][jj], RESET);    
                break;
            }
            #endif
            #ifdef MONO
                printf(" %c |", displayArray[ii][jj]);    
            #endif
        }
        printf("\n");
        for(jj = 0; jj < width; jj++)
        {/*prints a line between rows*/
            printf("+---+");
        }
        printf("\n");
    }



} 
/**********************************************
 *freeShip
 *frees ship 
 ***********************************************/


 
/**********************************************
 *constructBoard
 *creates a board struct 
 ***********************************************/
Board* constructBoard()
{
    Board* board;
    board = (Board*)malloc(sizeof(Board));
    board->shipList = createLinkedList();
    board->destroyed = 0;
    board->numShips = 0;
    return board;
}


/**********************************************
 *addShipToBoard
 *takes in the location, direction and length
 *of ship and adds to the board
 ***********************************************/
int addShipToBoard(int location[], char direction[], int length, char** board)
{
    int ii, jj, endRow, endCol, error;    

    /*ii or jj boolean expression will not be true unless end values change*/
    endRow = 1000;/*where end of ship should be*/
    endCol = 1000; 
    ii = location[0];/*col = location but starting at 0*/
    jj = location[1];/*row*/
    error = FALSE;    

    if(board[jj][ii] == '0')
    {
        fprintf(stderr, "error: ships intersect\n");
        error = TRUE;
    }
    board[jj][ii] = '0';/*fill index == location with first part*/
    while((ii != endCol)&&(jj != endRow)&&(length > 1)&&(error == FALSE))
    {
        switch(direction[0])
        {
            case 'N':/*exit while loop when ii or jj == end of length*/
                /*-1 for length - head of ship*/
                endRow = location[1] + length - 1; 
                jj++;/*increment towares endCol*/
            break;
            case 'S':
                endRow = location[1] - length + 1;
                jj--;
            break;
            case 'E':
                endCol = location[0] - length + 1;
                ii--;
            break;
            case 'W':
                endCol = location[0] + length - 1;
                ii++;
            break;
        } 
        if(board[jj][ii] == '0')
        {
            fprintf(stderr, "error: ships intersect");
            error = TRUE;
        } 
        board[jj][ii] = '0';
    }
    return error;
}

/**********************************************
 *free2DArray
 *frees a 2DArray 
 ***********************************************/
void free2DArray(char** array, int length)
{
    int ii;
    for(ii = 0; ii < length; ii++)
    {
        free(array[ii]);
    } 
    free(array); 

}

/*******************************************
*resetBoard
*sets board and ships to not destroyed 
*******************************************/
void resetBoard(Board* board)
{
    ListNode *node;
    board->destroyed = 0;/*set to none destroyed*/
    node = board->shipList->head;
    while(node != NULL)
    {
        ((Ship*)node->data)->destroyed = 0;
        node = node->next;
    }

}

/*****************************
*freeShip
*
******************************/
void freeShip(void* data)
{
    Ship* ship;
    ship = (Ship*)data;
    free(ship->name);
    free(data);
}












