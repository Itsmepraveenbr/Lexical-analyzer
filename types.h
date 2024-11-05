#ifndef TYPES_H
#define TYPES_H


//to use as return type to tell the status of function
typedef enum
{
    success,
    failure
}Status;


//to check the type of token
typedef enum
{
    KEYWORD,
    IDENTIFIER,
    CHAR_CONSTANT,
    INT_CONSTANT,
    FLOAT_CONSTANT,
    STRING_CONSTANT,
    OPERATOR,
    SPECIAL_CHAR,
    OPEN_BRACE,
    CLOSE_BRACE,
    ARRAY,
    PRE_P,
    SL_COMMENT,
    ML_COMMENT,
    UNKNOWN
}TOKEN_TYPE;

#endif

