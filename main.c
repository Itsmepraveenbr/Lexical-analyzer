/*
NAME : PRAVEEN B R
DESCRIPTION : LEXICAL ANALYZER PROJECT 
*/

#include <stdio.h>
#include "header.h"

void main(int argc, char *argv[])
{
    //to check the required number of CLA's are passed 
    if(argc == 2)
    {
        //validate the input file
        if( validate_file(argv) == success)
        {
             //tokenize the input file
            if(tokenize(argv) == success)
            {
                 printf("tokens printed successfully\n");

            }
            else
            {
        
                printf("unable to tokenize the file\n");
            }
        }
        else
        {
            printf("Read and validate file failed, please pass a valid .c file\n");

        }

    }
    //print the error and usage message if required number of arguments are not passed
    else
    {
        printf("Error please pass the required arguments\n");
        printf("usage : ./a.out filename.c\n");
    }
}
