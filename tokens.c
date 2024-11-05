
#include "header.h"
#include <ctype.h>
#include <string.h>

//declare all the keywords as an array
char * keywords[] = {"const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned", "short", "long", "double", "char",                       "int", "float", "struct", "union", "enum", "void", "typedef", "goto", "return", "continue", "break", "if", "else",                         "for", "while", "do", "switch", "case", "default","sizeof" };

char buffer[100];//to temporarily store the tokens 

//function to validate the input file
Status validate_file(char *argv[])
{
    FILE *fp;
    //open the file in reading mode
    fp = fopen(argv[1],"r");

    if(fp == NULL)
    {
        printf("unable to open %s file\n", argv[1]);
        return failure;
    }
   
     printf("file opened successfully\n"); 
     fclose(fp);
     return success;

}

//to tokenize the contents of input file and print the  tokens
Status tokenize(char *argv[])
{
    FILE *fp = fopen(argv[1],"r");
        
    if( fp == NULL)
    {
        printf("Unable to open the file\n");
        return failure;
    }
    
    //looping till the end of the file
    while( ! feof(fp) )
    {
       
        char ch;  //variable to hold the characters
        int i = 0;
        TOKEN token;   //structure to hold the tokens
        
        //to extract the contents of file character by character till End of file
        while((ch = fgetc(fp)) != EOF)
        {
            //to skip whitespace and newline
            if(ch == ' ' || ch == '\n' )
                continue;
           
            //to print the preprocessor drective
            else if(ch == '#')
            {
                memset(buffer, 0, sizeof(buffer));  // Set all elements to 0
                i = 0;
                          
                while(ch != '\n')  //to scan all the contents till next line 
                {
                     buffer[i++] = ch;           
                     ch=fgetc(fp);
                } 
           
                token.token_name = buffer;
                token.token_type = PRE_P; 
 
            }
            
            //if the character is alphabet
            else if(isalpha(ch))
            {
                memset(buffer, 0, sizeof(buffer));  // Set all elements to 0
                i = 0;
                buffer[i++] = ch;
                
                //looping and finding the word
                while((isalnum(ch = fgetc(fp)) || ch == '_'))
                {
                     buffer[i++] = ch;
                }

                //to find if it is an array
                //to skip the spaces
                while(ch == ' ')
                {
                    ch = fgetc(fp);
                }
                
                //if array declaration is done                
                if(ch == '[' )
                {
                    buffer[i++] = ch; //storing the '[' to buffer

                    //looping till we find the end of array ']' and storing the contents inside buffer
                    while( (ch = fgetc(fp)) != ']')
                    {
                        buffer[i++] = ch;
                    }
                    buffer[i++] = ch;   //adding the ']' at the end

                    token.token_type = ARRAY;
                }
                               
                else
                {
                    ungetc(ch,fp);//to put the extra character which was read back to the stream

                    //checking if the word is a keyword or identifier
                    if(isKeyword(token.token_name))
                    {
                        token.token_type = KEYWORD;
                    }
                    else
                    {
                        token.token_type = IDENTIFIER;
                    }
                }

                token.token_name = buffer;    

           
              }

            //if the character is a number
            else if(isdigit(ch))
            {
                memset(buffer,0,sizeof(buffer));
                i = 0;
                buffer[i++] = ch;
                //looping to get the complete number
                while(isdigit(ch = fgetc(fp)) || ch == '.')
                {
                    buffer[i++] = ch;
                }
                ungetc(ch,fp); //to ut the character back to the stream
                token.token_name = buffer;
                //if the number has a decimal point making it a float constant
                if(strstr(token.token_name,".") != NULL)
                {
                      token.token_type = FLOAT_CONSTANT;
                }
                else
                {
                      token.token_type = INT_CONSTANT;
                }
            }
            
            //to print the character constant present within ' '
            else if(ch == '\'' )
            {
                 memset(buffer,0,sizeof(buffer));
                 i = 0;
                 buffer[i++] = ch;

                 while((ch = fgetc(fp)) != '\'')
                 {
                     buffer[i++] = ch;
                 }
                     buffer[i++] = ch; //to add the ' at the end
             
                 token.token_name = buffer;
                 token.token_type = CHAR_CONSTANT;

            }
            
            //to print string constants enclosed within " "
            else if(ch == '\"')
            { 
                  memset(buffer,0,sizeof(buffer));
                  i = 0;
                  buffer[i++] = ch;

                  while((ch = fgetc(fp)) != '\"')
                  {
                      buffer[i++] = ch;
                  }
                  buffer[i++] = ch;
              
                  token.token_name = buffer;
                  token.token_type = STRING_CONSTANT;
            }

            //to check for comments or '/' is used as operator
            else if(ch == '/')
            {
                long int pos = ftell(fp); //variable to hold the present positiom of file pointer
                
                //if it is not used as comment
                if(isComment(fp,&token) == 0)
                {
                      memset(buffer,0,sizeof(buffer));
                      i = 0;
                      buffer[i++] = ch;
                    
                      token.token_name = buffer;
                      token.token_type = OPERATOR;
                      fseek(fp,pos,SEEK_SET); // reseting the file pointer to the position to where it was before

                }
                 
            }

            //to print the operators
            else if(ch == '+' || ch == '-' || ch == '*' || ch == '%' || 
                    ch == '|' || ch == '&' || ch == '~' || ch == '=' || 
                    ch == '>' || ch == '<' )
            {
                
                    memset(buffer,0,sizeof(buffer));
                    i = 0;
                    buffer[i++] = ch;
               
                    token.token_name = buffer;
                    token.token_type = OPERATOR;
             }


            //to print special characters
            else if(isSpecialcharacter(ch))
            {
                    memset(buffer,0,sizeof(buffer));
                    i = 0; 
                    buffer[i++] = ch;
                    token.token_name = buffer;
                    
                    //if the character is open brace
                    if( ch == '(' || ch == '{' )
                    {
                        token.token_type = OPEN_BRACE;
                    }
                    //if the character is a closed brace
                    else if( ch == ')' || ch == '}' )
                    {
                        token.token_type = CLOSE_BRACE;
                    }
                    //if it is other special character
                    else
                    {
                        token.token_type = SPECIAL_CHAR;
                    }
            }
            
            //if some unknown token is found
            else
            {
                    memset(buffer,0,sizeof(buffer));
                    i = 0;
                    buffer[i++] = ch;
                    token.token_name = buffer;
                    token.token_type = UNKNOWN;
            }
                
            print_tokens(&token); //to print the type of token in every iteration

        }
        return success;

    }

}

//function to print the tokens based on their types
void print_tokens(TOKEN *token)
{
  
    switch(token->token_type)
    {
        case KEYWORD :
                    printf(" %s         : KEYWORD\n", token -> token_name);
                    break;

        case IDENTIFIER :
                    printf(" %s         : IDENTIFIER\n", token -> token_name);
                    break;

        case CHAR_CONSTANT :
                    printf(" %s         : CHARACTER CONSTANT\n", token -> token_name);
                    break;

        case INT_CONSTANT :
                    printf(" %s         : INTEGER CONSTANT\n", token -> token_name);
                    break;

        case FLOAT_CONSTANT :
                    printf(" %s         : FLOAT CONSTANT\n", token -> token_name);
                    break;

        case STRING_CONSTANT :
                    printf(" %s         : STRING CONSTANT\n", token -> token_name);
                    break;

        case OPERATOR :
                    printf(" %s         : OPERATOR\n", token -> token_name);
                    break;

        case SPECIAL_CHAR :
                    printf(" %s         : SPECIAL CHARACTER\n", token -> token_name);
                    break;
        
        case OPEN_BRACE :
                    printf(" %s         : OPEN BRACE\n", token -> token_name);
                    break;

         case CLOSE_BRACE :
                    printf(" %s         : CLOSE BRACE\n", token -> token_name);
                    break;

         case ARRAY :
                    printf(" %s         : ARRAY\n", token -> token_name);
                    break;

        case PRE_P :
                    printf(" %s         : PREPROCESSOR\n", token -> token_name);
                    break;

        case SL_COMMENT :
                    printf(" %s         : SINGLE LINE COMMENT\n", token -> token_name);
                    break;

        case ML_COMMENT :
                    printf(" %s         : MULTI LINE COMMENT\n", token -> token_name);
                    break;
 
        default :
                    printf(" %s          : unable to identify the token\n", token -> token_name);
                    break;

    }

}

//function to check if the word is a keyword or not
int isKeyword( char *word)
{
    for(int i = 0 ; i < 32 ; i++ )
    {
        if( strcmp(word,keywords[i]) == 0)
        {
            return 1;
        }  
    }
    
    return 0;
}

//function to check if the character is a special character
int isSpecialcharacter(char c)
{
   switch(c)
   {
       case ',':
       case '?':
       case ':':
       case ';':
       case '{':
       case '}':
       case '(':
       case ')':
       case '[':
       case ']':
       case '@': 
            return 1;
       default:
            return 0;
    }
} 
           
//to check for single line or multiline comment
int isComment(FILE *lfp, TOKEN * ltoken)
{  
    int i = 0;
    char ch;

    //if it is single line comment
    if((ch = fgetc(lfp)) == '/')
    {
         buffer[i++] = '/';
         buffer[i++] = '/';

         //to get all the contentes of a line
        while((ch = fgetc(lfp)) != '\n')
        {
            buffer[i++] = ch;
        }

        buffer[i] = '\0';
        ltoken->token_name = buffer;
        ltoken->token_type = SL_COMMENT;
        return 1;

    }

    //if it is a multiline comment
    else if( ch == '*')
    {
        buffer[i++] = '/';
        buffer[i++] = '*';

        //to get the contents inside the comment
        while((ch = fgetc(lfp)) != '/')
        {
            buffer[i++] = ch;
        }

        buffer[i++] = '/';
        ltoken->token_name = buffer;
        ltoken->token_type = ML_COMMENT;
        return 1;
    }
    //if it is not a comment return false
    else
    {
        return 0;
    } 


}
