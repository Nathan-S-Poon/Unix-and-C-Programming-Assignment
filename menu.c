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
            /*create board to be displayed to player*/
            displayArray = create2DTemplate(height, width);            
    
            do /*loop until input is 0*/
            {
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
                            printf("Create Board File\n");
                        break;
                        case 4:
                            printf("Create Missile File\n");
                        break;
                        case 0:
                            printf("end game\n");
                        break;
                    }
                }
            }while((menuInt != 0)&&(shipError == FALSE));
            
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
    char* prompt = "1 Play the Game\n2 List all missiles\n0 Exit"; 
    char invalid[29] = " error need to be 0, 1 or 2.";
    char outStr[74];
    num = -1;
    strncpy(outStr, prompt, 45);
    do
    {
        strncpy(outStr, prompt, 45);
        printf("%s", outStr);
        printf("\n");
        scanf("%d", &num);
        strncat(invalid, prompt, 29);
        
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
    char invalidRange[82] = "Error: needs to be within row and column ";
    char* outStr;
    char location1[2];
    int location2;
    outStr = (char*)malloc(sizeof(char)); 
    strncpy(outStr, prompt, 38);  
    do
    {
        printf("%s", outStr);
        printf("\n");
        scanf(" %c%d", location1, &location2);
        strncat(invalidRange, prompt, 38);
        strncpy(outStr, invalidRange, 82);
   
        target[1] = location2;
        stringToUpper(location1); 
        target[0] = letterToNum(location1);
     
    }while((target[0] <= 0)||(target[0] >= height)||
           (target[1] <= 0)||(target[1] >= width));
    free(outStr);

/*    char location1[1];
    int location2;

    printf("Enter the coordinates for the next target\n");
    scanf(" %c%d", location1, &location2);
    
    target[1] = location2;
    stringToUpper(location1); 
    target[0] = letterToNum(location1);
   
    while((target[1] <= 0)||(target[1] >= height)||
         (target[0] <= 0)||(target[0] >= width));
    {
        printf("Error: needs to be within row and column\n");  
        printf("Enter the coordinates for the next target\n");
        scanf(" %c%d", location1, &location2);
        target[1] = location2;
        stringToUpper(location1); 
        target[0] = letterToNum(location1);
   
    } 
  */    

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
        displayBoard(displayArray, height, width);
        printf("Missiles left: %d\n", listLength(missList));
        printf("Current missile: %s\n", (curMissile->name));
        /*get coordinatesfrom player*/
        targetInput(target, height, width);
        /*apply input to board*/
        curMissile->funcPtr(displayArray, boardArray, target, boardInfo);       
        (missList->count)--;/*delete a missile*/
        nextMissile = nextMissile->next;/*get next missile*/
 
    }
    free(nextMissile);
    free(curMissile);

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









