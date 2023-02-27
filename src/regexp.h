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


Trigon nfa_transition(NFA x, NFA y, hash64 symbol)
{
    Trigon t = TRIGON_MALLOC;
    TRIGON_SET(t,&x,&y,&symbol);
    return t;
}
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

NFA nfa_symbol(hash64 symbol)
{
    Trigon G = TRIGON_MALLOC;
    Trigon x = TRIGON_MALLOC;
    Trigon y = TRIGON_MALLOC;
    TRIGON_SET_X(G, &x);
    TRIGON_SET_Y(G, &y);
    nfa_transition(x,y,symbol);
    return G;
}


string addsymbol(hashmap symbols, string x) 
{
    hash64 h = h64(x);
    if ( isfree_h( symbols, h) ) 
    {
	string symbol = strallocn(x, strlen(x));
	insert_h( symbols, h, symbol );
    }
    return (string)inspect_h(symbols, h);
}

string nxtsymbol(string src) 
{
    if    ( src == NULL || *src == 0x00 ) { return NULL;                       }
    while ( *src && !IS_SYMBOL(*src)    ) { src++;                             } 
    if    ( *src == 0x00                ) { return NULL;                       }
    return src;
}

string ldsymbol(string dst, string src) 
{
    src = nxtsymbol(src);
    if    ( src && IS_RANGE(*src)       ) { return strcpyd( dst, src, R_RANGE);}
    if    ( src && IS_SYMBOL(*src)      ) { return strcpyn( dst, src, 1 );     }
    return NULL;
}

Trigon ldsymbols(string x)
{
    string  current = x;
    hashmap symbols = HASHMAP_ALLOC;
    string  symbol  = (string)calloc(1, MAX_SYMBOL_LEN);

    while(current)
    {
	current = ldsymbol(symbol, current);
	if ( !current ) break;
	addsymbol(symbols, symbol);
	TRIGON_PUSH(symbols, nfa_symbol(symbol));
    }
    free(symbol);

    return symbols;
}

NFA initNFA(regexp x)
{
    void* s = ldsymbols(x);
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
