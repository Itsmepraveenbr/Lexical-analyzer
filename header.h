
#include "types.h"
#include<stdio.h>


//structure to temporarily store the token and its type
typedef struct
{
    char *token_name;
    TOKEN_TYPE token_type;

}TOKEN;


//function to validate the input file
Status validate_file(char *argv[]);

//function to tokenize the input file
Status tokenize(char *argv[]);

//function to print the tokens
void print_tokens(TOKEN *token);

//function to find the word is keyword or not
int isKeyword( char *word);

//function to find the token is special character
int isSpecialcharacter(char c);

//function to find the token is a comment
int isComment(FILE *lfp , TOKEN *ltoken);
