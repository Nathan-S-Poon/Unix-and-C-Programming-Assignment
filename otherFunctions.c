/**********************************************************************
 *TO BE DELETED
 *Contains code that I will decide later to put somewhere else
 *
 *********************************************************************/


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

/**********************************************
 *free2DArray
 *frees a 2DArray 
 ***********************************************/
void free2DArray(char*** array, int length)
{

    for(ii = 0; ii < length; ii++)
    {
        free(array[ii]);
    } 
    free(array); 

}


/**********************************************
 *freeMissile
 *frees a missile struct
 ***********************************************/
void freeMissile(void* missile)
{
    Missile* temp;
    temp = (Missile*)missile;
    free((*missile).funcPtr);
    free(missile);
    free(temp);    

}

































