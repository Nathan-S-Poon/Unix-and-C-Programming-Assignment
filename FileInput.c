/*****************************************************************************
*Program: FileInput
*Date: 10/4/2020
*Description: takes in two command line parameters that wil be the files to be
*read.
*TO DO: free void pointer? ship construct
*test without letter to Num
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
int readBoardFile(Board* board, char* fileName)
{
    /*Variables*/
    FILE* f;
    char location1[2];
    char direction[2];
    int length, location2;
    int location[2];
    char* name;        
    int errorFile;
    Ship* ship;
    /*intialise*/
    errorFile = FALSE;  
   /*Open file then check if file is null*/
    f = fopen(fileName, "r");
    if(f == NULL)
    {
        perror("Error: file is null");
    }
    else
    {
        /*Get height and weight from first line*/
        fscanf(f, "%d,%d\n", &((*board).width), &((*board).height));
        /*check width and height is valid*/
        if(((*board).width < 1)||((*board).width > 12)||
          ((*board).height < 1)||((*board).height) > 12)
        {            
            fprintf(stderr, "size of board needs to be between 12X12 or 1X1\n");
            errorFile = TRUE;
        }
        else
        {
 
       while((fgets(location1, 2, f) != NULL) &&(errorFile == FALSE))
       {   
                /*malloc name*/
                name = (char*)malloc(50 * sizeof(char));
 
                /*read in from file*/
                fscanf(f, "%d", &location2); 
                fgetc(f);/*spaces*/                
                fgets(direction, 2, f);
                fgetc(f);
                fscanf(f, "%d", &length);
                fgetc(f);
                fgets(name, 49, f); 

                /*convert location*/
                location[0] = letterToNum(location1);
                location[1] = location2;
               /*error handling*/
                errorFile = checkBoardForError((*board).width, (*board).height, 
                location, direction, length, name);  
    

                if(errorFile == TRUE)
                {
                    perror("file is invalid");
                }
                else
                {
                    /*insert values into a ship struct*/
                    ship = (Ship*)malloc(sizeof(Ship));
                    ship = createShip(location, direction, length, name);  
                    /*store ship pointer into board's linkedlist*/        
                    insertLast(board->shipList, ship);
                    (*board).numShips++;/*count ship*/
                }
 
     

         }
       /*  free(ship); 
         ship = NULL;
       */
    }
         if(ferror(f))
         {
             perror("Error with reading file\n");
         }
         fclose(f);
    
   /*Free heap*/
/*    free(name);
    name = NULL;*/
    }
    return errorFile;
}

/***********************************************************************
*create a ship
*changes location to num
***********************************************************************/
Ship* createShip(int location[], char direction[], int length, char* name)
{
    Ship* ship;
    ship = (Ship*)malloc(sizeof(Ship));   
    ship->location[0] = location[0];
    ship->location[1] = location[1];  
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
int checkBoardForError(int width, int height, int location[], 
                       char direction[], int length, char* name) 
{   /*variables*/ 
    int directValid, endOfLength, fileError; 
    char heightOrWidth;
    /*intialise*/
    directValid = FALSE;
    fileError = FALSE;
   
    stringToUpper(direction);
    /*check if location is within board*/
    if((location[0] < 1)||(location[0] > height)||(location[1] < 1)
      ||(location[1] > width)) 
    {
       fprintf(stderr, "location is invalid: needs to be within board"); 
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
        fprintf(stderr, "direction is invalid: needs to be N,S,W,E");
        fileError = TRUE; 
    }
    /*length is validated - needs to be positive and fit board*/ 
    endOfLength = 0;
    heightOrWidth = 'a';/*determines if ship extends height or width*/
    switch(direction[0])
    {
        case 'N': 
            endOfLength = location[0] + length - 1;    
            heightOrWidth = 'h';
        break;
        case 'S': 
            endOfLength = location[0] - length + 1;
            heightOrWidth = 'h';
        break;
        case 'E':
            endOfLength = location[1] - length + 1;
            heightOrWidth = 'w';
        break;
        case 'W':
            endOfLength = location[1] + length - 1;
            heightOrWidth = 'w';
        break;
    }
    if((heightOrWidth == 'h')&&((endOfLength < 1)||(endOfLength > height)))
    {
        fprintf(stderr, "length of boat extends past height of board");
        fileError = TRUE;
    }
    else if((heightOrWidth == 'w')&&((endOfLength < 1)||(endOfLength > width)))
    {
        fprintf(stderr, "length of boat extends past width of board");
        fileError = TRUE;
    }
    /*validate ship name - ensure it is at least one character*/     
    if(strcmp(name, "") == 0)
    {
        fprintf(stderr, "name of ship needs to be at least one character");
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
        fprintf(stderr, "Error: file is null");
    }
    else
    {
       while((fgets(missileFile, 7, f) != NULL)&&(invalid == FALSE))
        {
            /*get content of file and put into missile struct*/  
            fgetc(f);/*next line*/
            /*malloc description and missile*/
            description = (char*)malloc(sizeof(char));
            missile = (Missile*)malloc(sizeof(Missile)); 
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
                description = "Hits adn entire row";
            }
            else/*invalid*/
            {
                fprintf(stderr, "invalid missile file");
                invalid = TRUE;
            }   
            /*insert into missile struct then into linked list*/
            (*missile).funcPtr = funcPtr;
            missile->description = description;
            strncpy((*missile).name, missileFile, 7);
            insertLast(list, missile);
        
       }


        if(ferror(f))
        {
            perror("Error with reading file\n");
        }
        fclose(f);
    }  

    return invalid;
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

 /*****************************************
 *convert letter to num
 *Converts A-L to 1-12
 *****************************************/
int letterToNum(char letter[])
{
    int num;
    num = 0;
    switch(letter[0])
    {
        case 'A':
            num = 1;
        break;
        case 'B':
            num = 2;    
        break;
        case 'C':
            num = 3;
        break;
        case 'D':
            num = 4;
        break;
        case 'E':
            num = 5;
        break;
        case 'F':
            num = 6;
        break;
        case 'G':
            num = 7;
        break;
        case 'H':
            num = 8;
        break;
        case 'I':
            num = 9;
        break;
        case 'J':
            num = 10;
        break;
        case 'K':
            num = 11;
        break;
        case 'L':
            num = 12; 
        break;
 
    }
    return num;
}

/******************************************
*coordsConvert
*converts location coordinates from char to int
******************************************/
/*void coordsConvert(char locationChar[], int locationInt[])
{
    locationInt[1] = locationChar[1] - '0';  

} */























  
