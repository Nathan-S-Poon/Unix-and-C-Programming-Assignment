/*****************************************************************************
*Program: FileInput
*Date: 10/4/2020
*Description: takes in two command line parameters that wil be the files to be
*read. Validates files before constructing the ship structs, board structs
*and LinkedList of missile structs. 
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "structs.h"
#include "missileFunc.h"
#include "boardFunc.h"
#include "FileInput.h"
/**********************************************************
*Input constructed board pointer and filename to extract board details
*from file and insert them into the board pointer 
*******************************************************/
int readBoardFile(Board* board, char* fileName)
{
    /*Variables*/
    FILE* f;
    char locationChar;
    char direction;
    int length, locationInt;
    int location[2];/*final storage of location*/
    char* name;        
    int errorFile, errorDimensions;
    Ship* ship;
    char checkNull;
    /*intialise*/
    errorDimensions = 0;
    errorFile = FALSE;  
   /*Open file then check if file is null*/
    f = fopen(fileName, "r");
    if(f == NULL)
    {
        fprintf(stderr, "Error: file is null\n");
        errorFile = TRUE;
    }
    else
    {
        /*Get height and weight from first line*/
        errorDimensions = fscanf(f, "%d,%d\n", &(board->width), &(board->height));
        /*check there is a height and width*/
        if(errorDimensions != 2)
        {
            fprintf(stderr, "Board file needs a height and width\n");
            errorFile = TRUE;
        }
        /*check width and height is valid*/
        else if(((*board).width < 1)||((*board).width > 12)||
          ((*board).height < 1)||((*board).height) > 12)
        {            
            fprintf(stderr, "size of board needs to be between 12X12 or 1X1\n");
            errorFile = TRUE;
        }
        else
        {
 
           while((fscanf(f, "%c%d %c %d", &locationChar, &locationInt, 
           &direction, &length) == 4)&&(!errorFile))
           {   
                /*malloc name*/
                name = (char*)malloc(50 * sizeof(char)); 
                /*read in from file*/     
                checkNull = fgetc(f); 
                /*get name if it is not null*/
                if(checkNull != '\n')
                {
                    fgets(name, 49, f); 
                }
                /*convert location and make direction uppercase*/
                locationChar = charToUpper(locationChar);
                location[0] = letterToNum(locationChar);
                location[1] = locationInt;
                direction = charToUpper(direction); 
               /*error handling*/
                errorFile = checkBoardForError((*board).width, (*board).height, 
                location, direction, length, name);  
    

                if(errorFile)
                {
                    perror("file is in invalid format\n");
                    free(name);
                }
                else
                {
                    /*insert values into a ship struct*/
                    ship = createShip(location, direction, length, name);  
                    /*store ship pointer into board's linkedlist*/        
                    insertLast(board->shipList, ship);
                    (*board).numShips++;/*count ship*/
                }     

            }/*check that there are ships*/
            if(board->numShips == 0)
            { 
                fprintf(stderr, "Error: no ships\n");
                errorFile = TRUE;
            }
        }
        if(ferror(f))
        {
            perror("Error with reading file\n");
            errorFile = TRUE;
        }
        fclose(f);
    
    
    }
    return errorFile;
}

 

/********************************************************8
 *checkBoardForError
 *checks if board inputs are valid. If invalid then return 
 *error message. 
 ********************************************************/
int checkBoardForError(int width, int height, int location[], 
                       char direction, int length, char* name) 
{   /*variables*/ 
    int endOfLength, fileError; 
    char heightOrWidth;
    /*intialise*/
    fileError = FALSE;
    
    /*check if location is within board*/
    if((location[1] < 1)||(location[1] > height)||(location[0] < 1)
      ||(location[0] > width)) 
    {
       fprintf(stderr, "location is invalid: needs to be within board\n"); 
       fileError = TRUE;
    }
    /*length is validated - needs to be positive and fit board*/ 
    endOfLength = 0;/*position of last part of ship*/
    heightOrWidth = 'a';/*determines if ship extends height or width*/
    length--;/*minus head of ship*/
    switch(direction)
    {
        case 'N': 
            endOfLength = location[1] + length;    
            heightOrWidth = 'h';
        break;
        case 'S': 
            endOfLength = location[1] - length;
            heightOrWidth = 'h';
        break;
        case 'E':
            endOfLength = location[0] - length;
            heightOrWidth = 'w';
        break;
        case 'W':
            endOfLength = location[0] + length;
            heightOrWidth = 'w';
        break;
        
    }
    if((heightOrWidth == 'h')&&((endOfLength < 1)||(endOfLength > height)))
    {
        fprintf(stderr, "length of boat extends past height of board\n");
        fileError = TRUE;
    }
    else if((heightOrWidth == 'w')&&((endOfLength < 1)||(endOfLength > width)))
    {
        fprintf(stderr, "length of boat extends past width of board\n");
        fileError = TRUE;
    }/*if direction is none of the above then it is invalid*/
    else if((heightOrWidth != 'h')&&(heightOrWidth != 'w'))
    {
        fprintf(stderr, "direction is invalid: needs to be N,S,W,E\n");
        fileError = TRUE; 
    }

    /*validate ship name - ensure it is at least one character*/     
    if(strcmp(name, "") == 0)
    {
        fprintf(stderr, "name of ship needs to be at least one character\n");
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
int readMissileFile(LinkedList* list, char* fileName)
{
    /*Variables*/
    FILE* f;
    char* description;
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
        fprintf(stderr, "Error: file is null\n");
        invalid = TRUE;
    }
    else
    {
        while((fgets(missileFile, 7, f) != NULL)&&(!invalid))
        {
            /*get content of file and put into missile struct*/  
            fgetc(f);/*next line*/
            /*assign funcPtr to function based on file input*/
            stringToUpper(missileFile);
            if(strcmp(missileFile, "SINGLE") == 0)
            {
                funcPtr = &single;
                description = "Hits a single tile";
            }
            else if(strcmp(missileFile, "SPLASH") == 0)
            {
                funcPtr = &splash;
                description = "Hits a 3x3 square";
            }
            else if(strcmp(missileFile, "V-LINE") == 0)
            {
                funcPtr = &vLine;
                description = "Hits an entire column";
            }
            else if(strcmp(missileFile, "H-LINE") == 0)
            {
                funcPtr = &hLine;
                description = "Hits an entire row";
            }
            else/*invalid*/
            {
                fprintf(stderr, "invalid missile file\n");
                fprintf(stderr, "needs to be single, splash, h-line or v-line\n");
                invalid = TRUE;
            } 
            if(!invalid)
            {  
                /*malloc missile*/
                missile = (Missile*)malloc(sizeof(Missile)); 
                /*insert into missile struct then into linked list*/
                (*missile).funcPtr = funcPtr;
                missile->description = description;
                strncpy((*missile).name, missileFile, 7);
                insertLast(list, missile);
            }
        }
        /*check that some missiles exist*/
        if(invalid)
        {
            fprintf(stderr, "Error, no missiles\n");
        }

        if(ferror(f))
        {
            perror("Error with reading file\n");
        }
        fclose(f);
    }  

    return invalid;
}

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

/********************************
 *charToUpper - converts to uppercase
 *Copied from prac 4
 *Used in fileInput
 *********************************/
char charToUpper(char word)
{
    /*variables*/
    int diff /*len*/;
   
    diff = 'a' - 'A';/*get diff between lower and upper*/
    
    if(word >= 97)
    {
        word = word - diff; 
    }
    return word;
}

 /*****************************************
 *convert letter to num
 *For converting A-L to 1-12
 *****************************************/
int letterToNum(char letter)
{
    int num;
    num = (int)letter;
    num = num - 64;
 
    return num;
}


















  
