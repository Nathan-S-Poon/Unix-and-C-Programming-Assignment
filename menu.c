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
    char** boardArray;
    char** displayArray;
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
                            free2DArray(boardArray, height);
                            free2DArray(displayArray, height);
                            resetBoard(boardInfo);
                            missList->count = origNumMissile;
                        break;
                        case 2:
                            printf("missile list\n");
                            printLinkedList(missList, printPtr);
                        break;
                        case 3:
                            inputBoard(); 
                        break;
                        case 4:
                            missileNameFile();   
                        break;
                        case 0:
                            printf("end game\n");
                        break;
                    }
                }
            }while((menuInt != 0)&&(shipError == FALSE));
            
        }
    /*free heaps*/
    freeLinkedList(missList, &free);
    freeLinkedList(boardInfo->shipList, &free);
    free(boardInfo);
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
    char prompt[43] = "1 Play the Game\n2 List all missiles\n0 Exit"; 
    char invalid[72] = "Error need to be 0, 1 or 2.\n";
    char outStr[72];
    num = -1;
    strncpy(outStr, prompt, 45);
    strncat(invalid, prompt, 72);
 
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf("%d", &num);
        strncpy(outStr, invalid, 72);
        
    }while((num < 0)||(num > 4));
    return num;
}

/************************************************************** 
*target: Outputs 'prompt' and gets user input for coordinates
of next target. If input is invalid then error message is 
*added to prompt. Loops until user enters a valid input
****************************************************************/
void targetInput(int target[], int height, int width)
{
    char prompt[38] = "Enter the coordinates for next target"; 
    char invalidRange[81] = "Error: needs to be within row and column. ";
    char outStr[81];
    char location1[2];
    int location2; 
    strncpy(outStr, prompt, 38);  
    strncat(invalidRange, prompt, 38);
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf(" %c%d", location1, &location2);
        strncpy(outStr, invalidRange, 81);
   
        target[1] = location2;
        stringToUpper(location1); 
        target[0] = letterToNum(location1);
     
    }while((target[1] <= 0)||(target[1] >= height)||
           (target[0] <= 0)||(target[0] >= width));

}

 

/****************************************************************8
*playGame: displays board and missiles to player and prompts 
*player to input coordinates to shoot. Loops until either all
*ships are shot or player runs out of missiles
******************************************************************/
void playGame(char** displayArray, char** boardArray, Board* boardInfo,
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
        targetInput(target, height, width);
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









