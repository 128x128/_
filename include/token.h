#ifndef TOKEN_H 
#define TOKEN_H

#include "buffer.h"

typedef enum TOKEN_TYPE {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EOF
} TOKEN_TYPE;

typedef struct token {
    TOKEN_TYPE type;
    buffer* lexeme; 
    int row;
    int col;
} token;

token* talloc(const char* lexeme) {
    token* t = (token*)malloc(sizeof(token));
    t->lexeme = NULL;
    return t; 
}

#endif
