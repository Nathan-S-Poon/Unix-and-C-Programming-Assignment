/**************************************************************************
*program: menu
*Date: 26/4/2020
*Description: contains code for UI menu and calls other functions to play
*game. Reads in file input from command line
*
*When user enters 1, game will start. When game is over, user will have to 
*option to select 1 and start the same game over. when user selects 2, a
*list of missiles will be displayed, when User selects 3 or 4, they will 
*be able to change the board layout or the missiles they get. 
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "menu.h"
#include "writeFiles.h"
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
    char*** boardArray;
    char*** displayArray;
    listFunc printPtr; 
    /*error variables*/
    int fileError, missError, shipError;
    /*board dimensions and missile number*/
    int height, width, origNumMissile; 
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
        fileError = readBoardFile(boardInfo, boardFile);
        missError = readMissileFile(missList, missileFile);  
        if((missError != 1)&&(fileError != 1))/*end if error*/
        { 
            origNumMissile = missList->count;/*retain orignal num of missiles*/
            width = boardInfo->width + 1;
            height = boardInfo->height + 1;  
   
            do /*loop until input is 0*/
            {
                /*create board to be displayed to player*/
                displayArray = create2DTemplate(height, width);            
                /*create 2d array of board info*/
                boardArray = create2DTemplate(height, width);
                shipError = createGameBoard(boardInfo, boardArray); 
               
                if(shipError == FALSE)/*end if error*/
                {
                    menuInt = menuInput();
                    switch(menuInt)
                    {
                        case 1:
                            printf("Game start\n");
                            playGame(displayArray, boardArray, boardInfo,
                                         missList, height, width);
                            /*free boards*/
                            free2DArray(boardArray, height, width);
                            free2DArray(displayArray, height, width);
                            resetBoard(boardInfo);
                            missList->count = origNumMissile;
                        break;
                        case 2:
                            printf("missile list\n");
                            printLinkedList(missList, printPtr);
                        break;
                        case 3:
                            writeBoard(); 
                        break;
                        case 4:
                            writeMissile();   
                        break;
                        case 0:
                            printf("end game\n");
                        break;
                    }
                }
            }while((menuInt != 0)&&(shipError == FALSE));
             /*free heaps*/
            freeLinkedList(missList, &free);
            freeLinkedList(boardInfo->shipList, &freeShip);
            free(boardInfo);
            free2DArray(boardArray, height, width);
            free2DArray(displayArray, height, width);   
        }
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
    char prompt[90] = "1 Play the Game\n2 List all missiles\n"; 
    char part2[53] = "3 create new board\n4 create new missile list\n0 Exit";
    char invalid[162] = "Error need to be 0, 1, 2, 3 or 4.\n";
    char outStr[162];
    num = -1;
    strncat(prompt, part2, 53);
    strncpy(outStr, prompt, 90);
    strncat(invalid, prompt, 162);
 
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf("%d", &num);
        strncpy(outStr, invalid, 162);
        
    }while((num < 0)||(num > 4));
    return num;
}

/************************************************************** 
*target: Outputs 'prompt' and gets user input for coordinates
of next target. If input is invalid then error message is 
*added to prompt. Loops until user enters a valid input
****************************************************************/
void targetInput(int target[], int height, int width, Missile* curMissile)
{
    char outStr[81];
    char prompt[38] = "Enter the coordinates for next target"; 
    char invalidRange[81] = "Error: needs to be within row and column. ";
    char input[5];
    char location[2] = "Z";/*need to store letter to convert into num before
                       passing to target*/
    strncpy(outStr, prompt, 38);
    strncat(invalidRange, prompt, 81);/*append error message for next loop*/      
    /*Initialise variables*/
    target[0] = -1;
    target[1] = -1;
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf("%s", input);
        if(strcmp(input, "help") == 0)
        {/*print out current missile description*/
            printf("%s\n", curMissile->description);
            strncpy(outStr, prompt, 81);/*set to exclude error message*/       
        }
        else/*get next coordinate*/
        {
            strncpy(outStr, invalidRange, 81);
            sscanf(input, "%c%d", location, &target[1]);
            stringToUpper(location); 
            target[0] = letterToNum(location);
        }
    }while((target[1] <= 0)||(target[1] >= height)||
           (target[0] <= 0)||(target[0] >= width));

}

 

/****************************************************************8
*playGame: displays board and missiles to player and prompts 
*player to input coordinates to shoot. Loops until either all
*ships are shot or player runs out of missiles
******************************************************************/
void playGame(char*** displayArray, char*** boardArray, Board* boardInfo,
             LinkedList* missList, int height, int width)
{
    int target[2];/*user input*/
    ListNode* nextMissile;
    Missile* curMissile;
 
    nextMissile = missList->head;/*nextMissile keeps track of current missile*/
    while((boardInfo->destroyed != boardInfo->numShips)
           &&(listLength(missList) > 0))
    {
        curMissile = (Missile*)(nextMissile->data);/*set curMissile to current*/
        /*print display*/
        printf("Current board\n");
        displayBoard(displayArray, boardArray, height, width);
        printf("Missiles left: %d\n", listLength(missList));
        printf("Current missile: %s\n", (curMissile->name));
        /*get coordinatesfrom player*/
        targetInput(target, height, width, curMissile);
        /*apply input to board*/
        curMissile->funcPtr(displayArray, boardArray, target, boardInfo);       
        (missList->count)--;/*delete a missile*/
        nextMissile = nextMissile->next;/*get next missile*/
        /*If all ships destroyed*/
        if(boardInfo->destroyed == boardInfo->numShips)
        { 
            printf("All ships have been destroyed\n");
        }
    }
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









