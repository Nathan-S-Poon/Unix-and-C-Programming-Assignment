/******************************************************************************
*Program: BoardFunc
*Date: 10/4/2020
*Description: contains functions for creating and using the board and ships
*
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
 *calls create2DTemplate to get board structure
 *then fills array through addShipToBoard
 ***********************************************/
int createGameBoard(Board* boardFile, char*** boardArray)
{ 
    /*variables*/
    ListNode* curNode;
    Ship* curShip;
    int error;
    error = FALSE;
    curNode = NULL;
    /*iterate through shipList and add coordinates to board*/
    curNode = boardFile->shipList->head;
    while((curNode != NULL)&&(!error))
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
char*** create2DTemplate(int height, int width)
{
    /*variables*/
    char*** array;
    int ii, jj, yAxis;
    char xAxis;
    xAxis = 'A';  
    yAxis = 1;
  
    /*malloc 2d array of strings*/
    array = (char***)malloc(height * sizeof(char**)); 
    for(ii = 0; ii < height; ii++)
    {
        array[ii] = (char**)malloc(width * sizeof(char*));
        /*malloc string in array*/
        for(jj = 0; jj < width; jj++)
        {
            array[ii][jj] = (char*)malloc(3*sizeof(char));
        } 
    }   
   
    /*fill boardArray corner*/    
    strncpy(array[0][0],":)", 3);
    /*fill X axis with letters*/ 
    for(ii = 1; ii < width; ii++)
    {
        sprintf(array[0][ii], "%c", xAxis);
        xAxis++;
    }
    /*fill y axis with numbers*/
    for(ii = 1; ii < height; ii++)
    {
        sprintf(array[ii][0], "%d", yAxis);
        yAxis++; 
    }
    
    for(ii = 1; ii < height; ii++)
    {
        for(jj = 1; jj < width; jj++)
        {
            strncpy(array[ii][jj], "#", 2);
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
void displayBoard(char*** displayArray, char*** answer, int height, int width)
{
    int ii, jj;
    char charDisplay, charAns;
    
    for(ii = 0; ii < height; ii++)
    {
        for(jj = 0; jj < width; jj++)
        {   
            /*extract char from string*/
            sscanf(displayArray[ii][jj], "%c", &charDisplay); 
            sscanf(answer[ii][jj], "%c", &charAns);
            /*print | 'char' | for each*/
            printf("|");
            /*print colour only if mono not defined*/
            #ifndef MONO 
            switch(charDisplay)
            {/*changes colour depenfing on type*/
                case '#':/*conditional debug to find ships*/
                    #ifdef DEBUG
                    if(charAns == '0')
                    {
                        printf(" %s%s%s |", MAGENTA, displayArray[ii][jj], RESET);    
                    }
                    else
                    {
                        printf(" %s%s%s |", CYAN, displayArray[ii][jj], RESET);    
                    }
                    #endif
                    #ifndef DEBUG
                    printf(" %s%s%s |", CYAN, displayArray[ii][jj], RESET);   
                    #endif      
                break;
                case 'X':
                    printf(" %s%s%s |", RED, displayArray[ii][jj], RESET);    
                break;
                case '0':
                    printf(" %s%s%s |", GREEN, displayArray[ii][jj], RESET);    
                break;
                default:
                    printf(" %s%s%s |", YELLOW, displayArray[ii][jj], RESET);    
                break;
            }
            #endif
            #ifdef MONO
                printf(" %s |", displayArray[ii][jj]);    
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
/***********************************************************************
*create a ship
*returns a ship pointer
***********************************************************************/
Ship* createShip(int location[], char direction, int length, char* name)
{
    Ship* ship;
    ship = (Ship*)malloc(sizeof(Ship));   
    ship->location[0] = location[0];
    ship->location[1] = location[1];  
    ship->direction = direction; 
    ship->length = length; 
    ship->name = name;
    ship->destroyed = 0;
    return ship;
}
 
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
int addShipToBoard(int location[], char direction, int length, char*** board)
{
    int ii, jj, endRow, endCol, error;    

    /*ii or jj boolean expression will not be true unless end values change*/
    endRow = 1000;/*where end of ship should be*/
    endCol = 1000; 
    ii = location[0];/*col = location but starting at 0*/
    jj = location[1];/*row*/
    error = FALSE;    

    if(strcmp(board[jj][ii], "0") == 0)/*If two ship locations are on same spot*/
    {
        fprintf(stderr, "error: ships intersect\n");
        error = TRUE;
    }
    strncpy(board[jj][ii],"0", 2);/*fill index == location with first part*/
    while((ii != endCol)&&(jj != endRow)&&(length > 1)&&(!error))
    {
        switch(direction)
        {
            case 'N':/*exit while loop when ii or jj == end of length*/
                /*add or take length minus the head of the ship*/
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
        if(strcmp(board[jj][ii], "0") == 0)
        {
            fprintf(stderr, "error: ships intersect\n");
            error = TRUE;
        } 
        strncpy(board[jj][ii], "0", 2);/*add board to position*/
    }
    return error;
}

/**********************************************
 *free2DArray
 *frees a 2DArray 
 ***********************************************/
void free2DArray(char*** array, int length, int width)
{
    int jj, ii;
    for(ii = 0; ii < length; ii++)
    {
        for(jj = 0; jj < width; jj++)
        {
           free(array[ii][jj]);
        }
        free(array[ii]);
    } 
    free(array); 

}

/*******************************************
*resetBoard
*sets board and ships to not destroyed 
*after game is completed and restarted 
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
*To be used in freeLinkedList
******************************/
void freeShip(void* data)
{
    Ship* ship;
    ship = (Ship*)data;
    free(ship->name);
    free(data);
}












