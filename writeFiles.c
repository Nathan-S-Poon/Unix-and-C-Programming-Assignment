/***********************************************************************
*Program: writeFiles
*Description: contains functions to prompt user for input on new board 
*and missile files and functions to write to a new file
*
*
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "FileInput.h"
#include "writeFiles.h"

/**********************************************
*inputBoard
*gets user input for a new board
*
***********************************************/
void inputBoard(char* fileName)
{
    /*variables*/
    FILE* f;
    int numShips, width, height, shipEntries, error;
    /*user inputs*/
    int locationInt[2];
    char locationChar;
    char* name;
    int length;
    char direction; 
    shipEntries = 1;
    f = fopen(fileName, "w");
    
    do
    {
        error = FALSE;
        name = (char*)malloc(50*sizeof(char));
        /*get input on num of ships*/
        printf("input number of ships\n");
        scanf("%d", &numShips);
        /*get height and width*/
        printf("input height of board\n");
        scanf("%d", &height);
        printf("input width of board\n");
        scanf("%d", &width);
        if(numShips <= 0)
        {
            fprintf(stderr, "number of ships has to be greater than 0\n");
            error = TRUE;   
        }
        else if((width < 1)||(width > 12)||(height < 1)||(height) > 12)
        {            
            error = TRUE;
            fprintf(stderr, "size of board needs to be between 12X12 or 1X1\n");
        }
    }while(error);
    /*insert height then width for first line*/
    fprintf(f, "%d,%d\n", height, width);
    /*loop until number of entries is equal to the first num of ships
      or and error occurs*/
    while((shipEntries <= numShips)&&(!error))
    {
        error = TRUE;
        /*get all input*/
        printf("Answer the prompts to create ship %d\n", shipEntries);        
        printf("Enter the location of the head of the ship as a coordinate\n");
        scanf(" %c%d", &locationChar, &locationInt[1]);
        /*convert location into int*/
        locationChar = charToUpper(locationChar);
        locationInt[0] = letterToNum(locationChar);
        printf("Enter the direction that the ship is facing\n");
        scanf(" %c", &direction);
        printf("Enter the length of the ship\n");
        scanf("%d", &length);
        fgetc(stdin);
        printf("Enter a name for the ship\n");
        fgets(name, 50, stdin);
        /*check for errors*/
        direction = charToUpper(direction);
        error = checkBoardForError(width, height, locationInt, direction, 
                               length, name);
        /*if no errors then add to array and increment lines*/
        if(!error)
        {
            fprintf(f, "%c%d %c %d %s", locationChar, locationInt[1], 
                    direction, length, name);
            shipEntries++;
        }
    }
   
    free(name);
    fclose(f);
}


/**********************************************
*input missile
*gets user input for a new missile file
*
***********************************************/
void inputMissile(char* fileName)
{
    /*variables*/
    FILE* f;
    int numMissile, choice, missileCount;
    f = fopen(fileName, "w"); 
    missileCount = 1;
    numMissile = 0;
    /*get input on num of missiles*/
    do
    {   
        printf("Enter the number of missiles\n");
        scanf("%d", &numMissile);
        fflush(stdin);
    }while(numMissile <= 0);
    /*loop until missileCount == numMissiles*/
    while(missileCount <= numMissile)
    {
  
        /*User selects a missile*/
        printf("For missile %d, Select a missile\n", missileCount);
        printf("1 for Single\n");
        printf("2 for Splash\n");
        printf("3 for V-Line\n");
        printf("4 for H-Line\n");
        scanf("%d", &choice);
   
        if((choice >= 1) && (choice <= 4))
        { 
            switch(choice)
            {
                case 1:
                    fprintf(f, "SINGLE\n");
                break;
                case 2:
                    fprintf(f, "SPLASH\n");
                break;
                case 3:
                    fprintf(f, "V-LINE\n");
                break;
                case 4:
                    fprintf(f, "H-LINE\n");
                break; 
            }
            missileCount++;
        }
        else
        {
            printf("Error, not a valid input\n");
        } 
    }
    if(ferror(f))
    {
        perror("Error with writing to file\n");
    }
    fclose(f);
}


/**********************************************
*missileNameFile
*creates new missile file using user input
*
***********************************************/
void writeMissile()
{ 
    char* fileName;
    fileName = (char*)malloc(50*sizeof(char));
    /*get user input on name of file*/
    printf("Enter fileName\n");
    scanf("%s", fileName);
    if(strcmp(fileName, "\n") == 0)
    {
        fprintf(stderr, "missile file name must be at least one character\n");
    }
    else
    {
        inputMissile(fileName);
    }
    free(fileName); 
}

/**********************************************
*missileBoard
*creates new board file using user input
*
***********************************************/
void writeBoard()
{
    char* fileName;
    fileName = (char*)malloc(50*sizeof(char));
    /*get user input on name of file*/
    printf("Enter fileName\n");
    scanf("%s", fileName);
    if(strcmp(fileName, "\n") == 0)
    {
        fprintf(stderr, "board file name must be at least one character\n");
    }
    else
    {
        inputBoard(fileName);
    }
    free(fileName);
}










