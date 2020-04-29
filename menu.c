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
    listFunc printPtr; 
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
        /*loop until input is 0*/
        do
        {
            menuInt = intInput();
            switch(menuInt)
            {
                case 1:
                    printf("Game start\n");
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
*intInput: Outputs 'prompt' and gets user input. If input is
*invalid then error message is added to prompt. Loops until 
*user enters a valid input between max and min.
****************************************************************/
int intInput()
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









