#ifndef REGEXP_H 
#define REGEXP_H

#include "hashmap.h"
#include "trigon.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
//                            Reconginizer                                    //
////////////////////////////////////////////////////////////////////////////////

typedef char*               regexp;
typedef void*               NFA;

#define L_BRCKT             0x28
#define R_BRCKT             0x29 
#define L_RANGE             0x5B
#define R_RANGE             0x5D
#define MAX_SYMBOL_LEN      4096

#define IS_CLOSURE(x)       (x == '*')
#define IS_ALTERNATION(x)   (x == '|')
#define IS_LBRACKET(x)      (x == L_BRCKT)
#define IS_RBRACKET(x)      (x == R_BRCKT) 
#define IS_RANGE(x)         (x == L_RANGE)
#define IS_NUMBER(x)        (x >= '0' && x <= '9')
#define IS_LOWER(x)         (x >= 'a' && x <= 'z')
#define IS_UPPER(x)         (x >= 'A' && x <= 'Z')
#define IS_SYMBOL(x)        ( IS_RANGE(x) || IS_NUMBER(x)                      \
	                                  ||                                   \
	                      IS_LOWER(x) || IS_UPPER(x) )

#define EPSILON             0x00000000

typedef enum CHARTYPE 
{
    REGEX_RANGE,
    REGEX_SYMBOL,
    REGEX_OPEN,
    REGEX_CLOSE,
    REGEX_ALTERNATION,
    REGEX_CLOSURE,
    REGEX_CONCAT,
}
CHARTYPE;

typedef enum REGEX_OP 
{
    ALTERNATION,
    CONCAT,
    CLOSURE,
}
REGEX_OP;


//Trigon transitionNFA(NFA x, NFA y, uint64_t s)
//{
    //Trigon t = TRIGON_MALLOC;
    //TRIGON_SET(t,&x,&y,&s);
    //return t;
//}
//NFA alterationNFA(NFA x, NFA y) 
//{
    //NFA G = TRIGON_MALLOC;
    //transitionNFA(G,x,EPSILON);
    //transitionNFA(G,y,EPSILON);
    //return G;
//}

//NFA concatNFA(NFA x, NFA y) 
//{
    //NFA G = TRIGON_MALLOC;
    //transitionNFA(x,y,EPSILON);
    //return G;
//}

//NFA closureNFA(NFA* x) 
//{
    //NFA G = TRIGON_MALLOC;
    //return G;
//}

//NFA constructNFA(NFA x, NFA y, REGEX_OP z) 
//{
    //swtich(z) 
    //{
	//case ALTERNATION: return alterationNFA(x, y);
	//case CONCAT:      return concatNFA(x, y);
	//case CLOSURE:     return closureNFA(x, y);
    //}
    //return NULL;
//}


string addSymbol(hashmap symbols, regexp x) 
{
    hash64 h = h64(x);
    if ( isfree_h( symbols, h) ) 
    {
	string symbol = strallocn(x, strlen(x));
	insert_h( symbols, h, symbol );
    }
    return (string)inspect_h(symbols, h);
}

string nextSymbol(string dst, string src) 
{
    string x = src;
    if    ( !x                ) { return x;                                             }
    while ( *x&&!IS_SYMBOL(*x)) { x++;                                                  } 
    if    ( !(*x)             ) { return strclr( dst );                                 }
    if    ( IS_RANGE(*x)      ) { return strcpyn( dst, x, strnxt(x, R_RANGE) - x + 1 ); }
    if    ( IS_SYMBOL(*x)     ) { return strcpyn( dst, x, 1 );                          }
    return NULL;
}

Trigon loadSymbols(regexp x)
{
    hashmap     symbols = HASHMAP_ALLOC;
    char        symbol[MAX_SYMBOL_LEN]; 

    while(x)
    {
	x = nextSymbol(&symbol[0], x);
	addSymbol(symbols, &symbol[0]);
	//PSTR(symbol);
	//PNL;
    }

    return symbols;
}

NFA initNFA(regexp x)
{
    Trigon s = loadSymbols(x);
    //PLIST(s);
    
    //while(*x)
    //{
	//switch (chartype(*x)) 
	//{
	    //case REGEX_SYMBOL: PUSH(G, initSymbol(x)); break;
	    //case REGEX_OPEN:   PUSH(G, initNFA(x)); break;
	    //case REGEX_CLOSE:  return G;
	    //default: break;
	//}
	
    //}
    return NULL;
}

#endif
