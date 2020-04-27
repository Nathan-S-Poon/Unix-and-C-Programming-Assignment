/*****************************************************************************
*Program: FileInput
*Date: 10/4/2020
*Description: takes in two command line parameters that wil be the files to be
*read.
*TO DO: free void pointer? ship construct
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "gameFunc.h"
#include "boardFunc.h"
#include "FileInput.h"
/**********************************************************
*Input constructed board pointer and filename to extract board details
*from file and insert them into the board pointer 
*******************************************************/
void readBoardFile(Board* board, char* fileName)
{
    /*Variables*/
    FILE* f;
    char location[3];
    char direction[2];
    int length;
    char* name;        
    int errorFile;
    Ship* ship;
    /*intialise*/
    errorFile = FALSE;  
    name = (char*)malloc(50 * sizeof(char));
    /*Open file then check if file is null*/
    f = fopen(fileName, "r");
    if(f == NULL)
    {
        perror("Error: file is null");
    }
    else
    {
        /*Get height and weight from first line*/
        fscanf(f, "%d,%d", &((*board).width), &((*board).height));
        /*check width and height is valid*/
        if(((*board).width < 1)||((*board).width > 12)||
          ((*board).height < 1)||((*board).height) > 12)
        {            
            perror("size of board needs to be between 12X12 or 1X1\n");
        }
     
       do     
       {   
            /*initialise data as invalid, valid data will overwrite*/
            name = "";
            length = -1;
            direction[0] = 'a';
            strncpy(location, "XX", 3);                 
            /*read in file*/
            fgets(location, 3, f);
            fgets(direction, 2, f);
            fscanf(f, "%d", &length);
            fgets(name, 50, f);
            /*error handling*/
            errorFile = checkBoardForError((*board).width, (*board).height, 
            location, direction, length, name);           
            if(errorFile == FALSE)
            {
                fscanf(f, "%s %s %d %s", location, direction, &length, name);          
                /*insert values into a ship struct*/
                ship = (Ship*)malloc(sizeof(Ship));
                ship = createShip(location, direction, length, name);  
                /*store ship pointer into board's linkedlist*/        
                insertLast(board->shipList, ship);
                (*board).numShips++;/*count ship*/
            }
        }while((fgets(location, 3, f) == NULL)&&(errorFile == FALSE));            
 
        if(ferror(f))
        {
            perror("Error with reading file\n");
        }
        fclose(f);
    }
   /*Free heap*/
    free(name);
    free(ship); 
    name = NULL;
    ship = NULL;
}

/***********************************************************************
*create a ship
*changes location to num
***********************************************************************/
Ship* createShip(char location[], char direction[], int length, char* name)
{
    Ship* ship;
    int locationNum[2];
    locationNum[0] = location[0] - 16;
    locationNum[1] = location[1]; 
    ship = (Ship*)malloc(sizeof(Ship));   
    ship->location[0] = locationNum[0];
    ship->location[1] = locationNum[1];  
    ship->direction[0] = direction[0]; 
    ship->length = length; 
    ship->name = name;
    ship->destroyed = 0;
    return ship;
} 

/********************************************************8
 *checkBoardForError
 *checks if board inputs are valid. If invalid then return 
 *error message
 ********************************************************/
int checkBoardForError(int width, int height, char location[], 
                       char direction[], int length, char* name) 
{   /*variables*/ 
    int directValid, endOfLength, fileError; 
    int locRow, locCol;
    char heightOrWidth;
    /*intialise*/
    directValid = FALSE;
    fileError = FALSE;
    locCol = location[0] - 16;
    locRow = location[1];
    
    stringToUpper(direction);
    printf("error checking");
    printf("location is %d %d", locCol, locRow);
    /*check if location is within board*/
    if((locRow < 1)||(locRow > height)||(locCol < 1)
      ||(locCol > width)) 
    {
       perror("location is invalid: needs to be within board"); 
       fileError = TRUE;
    }
    /*check if driection is nsew. set to false if not*/
    switch(direction[0])
    {
        case 'N': case 'S': case 'W': case 'E': 
            directValid = TRUE;
        break;
    }
    if(directValid == FALSE)
    {
        perror("direction is invalid: needs to be N,S,W,E");
        fileError = TRUE; 
    }
    /*length is validated - needs to be positive and fit board*/ 
    endOfLength = 0;
    heightOrWidth = 'a';/*determines if ship extends height or width*/
    switch(direction[0])
    {
        case 'N': 
            endOfLength = locRow + length - 1;    
            heightOrWidth = 'h';
        break;
        case 'S': 
            endOfLength = locRow - length + 1;
            heightOrWidth = 'h';
        break;
        case 'E':
            endOfLength = locCol - length + 1;
            heightOrWidth = 'w';
        break;
        case 'W':
            endOfLength = locCol + length - 1;
            heightOrWidth = 'w';
        break;
    }
    if((heightOrWidth == 'h')&&((endOfLength < 1)||(endOfLength > height)))
    {
        perror("length of boat extends past height of board");
        fileError = TRUE;
    }
    else if((heightOrWidth == 'w')&&((endOfLength < 1)||(endOfLength > width)))
    {
        perror("length of boat extends past width of board");
        fileError = TRUE;
    }
    /*validate ship name - ensure it is at least one character*/     
    if((strcmp(name, "") == 0))
    {
        perror("name of ship needs to be at least one character");
        fileError = TRUE;
    }
    return fileError;
}




/*********************************************************
*readMissileFile
*Taked in a list and the name of a file. Fills list with
*file contents.
*needs gameFunc functions
******************************************************/
void readMissileFile(LinkedList* list, char* fileName)
{
    /*Variables*/
    FILE* f;
    char missileFile[7];    
    int invalid;/*if 1 then file is invalid*/
    Missile* missile;
    MissileFunc funcPtr;
    /*initialise*/    
    invalid = FALSE;

    /*Open file then check if file is null*/
    f = fopen(fileName, "r");
    if(f == NULL)
    {
        perror("Error: file is null");
    }
    else
    {
        while((fgets(missileFile, 7, f) != NULL)&&(invalid == FALSE))
        {
            /*get content of file and put into missile struct*/  
            fgets(missileFile, 7, f);
            
            /*malloc funcPtr and missile*/
            missile = (Missile*)malloc(sizeof(Missile)); 
            /*assign funcPtr to function based on file input*/
            stringToUpper(missileFile);
            if(strcmp(missileFile, "SINGLE") == 0)
            {
                funcPtr = &single;
            }
            else if(strcmp(missileFile, "SPLASH") == 0)
            {
                funcPtr = &splash;
            }
            else if(strcmp(missileFile, "V-LINE") == 0)
            {
                funcPtr = &vLine;
            }
            else if(strcmp(missileFile, "H-LINE") == 0)
            {
                funcPtr = &hLine;
            }
            else/*invalid*/
            {
                perror("invalid missile file");
                invalid = TRUE;
            }   
            /*insert into missile struct then into linked list*/
            (*missile).funcPtr = funcPtr;
            strncpy((*missile).name, missileFile, 7);
            list->head->data = missile;
        }


        if(ferror(f))
        {
            perror("Error with reading file\n");
        }
        fclose(f);
    } 
    /*Free heap*/
    free(missile);
    /*free(funcPtr); Need to free??*/
    missile = NULL;
    funcPtr = NULL;

}


 /*****************************************
 *
 *
 *****************************************/
/********************************
 *stringToUpper - converts to uppercase
 *Copied from prac 4
 *Used in fileInput 
 *********************************/
void stringToUpper(char* word)
{
    /*variables*/
    int ii, diff /*len*/;
   
    diff = 'a' - 'A';/*get diff between lower and upper*/
    
    ii = 0;
    while(*(word + ii) != '\0')
    {
        if(*(word+ii) >= 97)
        {
            *(word + ii) = (*(word + ii)) - diff;
        }
        ii++; 
    }
   

    
}








 
