/**************************************************************************
*program: menu
*Date: 26/4/2020
*Description: contains code for UI menu and calls other functions to play
*game. Reads in file input from command line
*
*
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "menu.h"
#include "structs.h"
#include "boardFunc.h"
#include "gameFunc.h"
#include "FileInput.h"
int main(int argc, char* argv[])
{
    /*variables*/
    int menuInt;
    Board* boardInfo;
    LinkedList* missList;
    char* boardFile;
    char* missileFile;
    char** boardArray;
    char** displayArray;
    listFunc printPtr; 
    /*board dimensions*/
    int height, width;
    int* target;/*user input*/
    /*intialise*/
    menuInt = -1;
    printPtr = &printMissile;  
    if(argc != 3)
    {
        perror("Error: needs three command line inputs");
    }
    else
    {
        /*allocate heap for Board adn for missile list*/
        boardInfo = constructBoard();
        missList = createLinkedList();
        
        /*get file inputs*/
        boardFile = argv[1];   
        missileFile = argv[2]; 
        readBoardFile(boardInfo, boardFile);
        readMissileFile(missList, missileFile);  
        width = boardInfo->width + 1;
        height = boardInfo->height + 1;      
        /*loop until input is 0*/
        do
        {
            menuInt = menuInput();
            switch(menuInt)
            {
                case 1:
                    printf("Game start\n");
                    /*create 2d array of board info*/
                    boardArray = create2DTemplate(height, width);
                    createGameBoard(boardInfo, boardArray); 
                    /*create board to be displayed to player*/
                    displayArray = create2DTemplate(height, width); 
                       
                    while((boardInfo->destroyed != 1)&&(missList != NULL))
                    {   /*print display*/
                        printf("Current board\n");
                        displayBoard(displayArray, height, width);
                        printf("Missiles left: %d\n", listLength(missList));
                        printf("Current missile: %s\n", 
                               ((Missile*)missList->head->data)->name);
                        /*get coordinatesfrom player*/
                        target = (int*)malloc(2*sizeof(int))
                        targetInput(target, height width);
                        /*apply input to board*/
                        ((Missile*)missList->head->data)->funcPtr(displayArray,
                        boardArray, target, boardInfo);
                    }

                break;
                case 2:
                    printf("missile list\n");
                    printLinkedList(missList, printPtr);                    
                break;
                case 0:
                    printf("end game\n");
                break;
            }
        }while(menuInt != 0);

    }

    return 0;
}


/************************************************************** 
*menuInput: Outputs 'prompt' and gets user input. If input is
*invalid then error message is added to prompt. Loops until 
*user enters a valid input between max and min.
****************************************************************/
int menuInput()
{
    int num;
    char* prompt = "1 Play the Game\n2 List all missiles\n0 Exit"; 
    char invalid[29] = " error need to be 0, 1 or 2.";
    char outStr[74];
    num = -1;
    strncpy(outStr, prompt, 45);
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf("%d", &num);
        strncat(invalid, prompt, 29);
        
    }while((num < 0)||(num > 2));
    return num;
}

/************************************************************** 
*target: Outputs 'prompt' and gets user input for coordinates
of next target. If input is invalid then error message is 
*added to prompt. Loops until user enters a valid input
****************************************************************/
void targetInput(int target[], int height int width)
{
    char* prompt = "Enter the coordinates for next target"; 
    char* invalidRange = " Error: needs to be within row and column";
    char* outStr;
    char location[3];    

    strncpy(outStr, prompt, 36);
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf("%s", location);
        strncat(invalidRange, prompt, 42);
        
        /*convert user input into two integers*/
        target[1] = location[1]; 
        target[0] = letterToNum(stringToUpper(location[0]));

    }while((target[0] <= 0)||(target[0] > height)||
           (target[1] <= 0)||(target[1] > width));
    return num;
}








/****************************************************************8
*playGame: displays board and missiles to player and prompts 
*player to input coordinates to shoot. Loops until either all
*ships are shot or player runs out of missiles
******************************************************************/
void playGame(char** displayBoard, char** answerBoard, LinkedList* missileList,
             int height, int width)
{
 






}


/***********************************
*printMissile
*prints missile name from missile struct
************************************/
void printMissile(void* data)
{
    Missile* missile;
    missile = (Missile*)data;
    printf("%s\n", missile->name);
}









