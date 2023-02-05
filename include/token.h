#ifndef TOKEN_H 
#define TOKEN_H

#include "object.h"

typedef enum TOKEN_T {
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  IDENTIFIER, STR, NUMBER,

  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  ENDFILE
} TOKEN_T;

typedef struct token {
    TOKEN_T type;
    char* lexeme; 
    object* literal; 
    int line;
} token;

token* initToken(char* lexeme) {
    token* t = (token*)malloc(sizeof(token));
    t->lexeme = lexeme;
    return t; 
}

#endif
