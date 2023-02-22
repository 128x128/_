#ifndef REGEXP_H 
#define REGEXP_H

#include "hashmap.h"

// Reconginizer

#define L_BRCKT             0x28
#define R_BRCKT             0x29
#define L_RANGE             0x5B
#define R_RANGE             0x5D

#define IS_CLO(x)           (x == '*')
#define IS_ALT(x)           (x == '|')
#define IS_LBK(x)           (x == L_BRCKT)
#define IS_RBK(x)           (x == R_BRCKT)
#define IS_RAN(x)           (x == L_RANGE)
#define IS_NUM(x)           (x >= '0' && x <= '9')
#define IS_LOW(x)           (x >= 'a' && x <= 'z')
#define IS_UPP(x)           (x >= 'A' && x <= 'Z')
#define IS_SYM(x)           (IS_RAN(x) || IS_NUM(x) || IS_LOW(x) || IS_UPP(x))

#define INIT_NFA            (initPair(initList(), initList()))
#define NFA_STRUCT(x)       ((NFA*)x->data)
#define NFA_START(x)        ((state*)x->head)
#define NFA_END(x)          ((state*)x->tail)

#define REGEX_CLO_CASE(x)   (IS_CLO(x) ? REGEX_CONCAT        : REGEX_CLOSURE)
#define REGEX_ALT_CASE(x)   (IS_ALT(x) ? REGEX_CLO_CASE(x)   : REGEX_ALTERNATION)
#define REGEX_CLOSE_CASE(x) (IS_RBK(x) ? REGEX_ALT_CASE(x)   : REGEX_CLOSE)
#define REGEX_OPEN_CASE(x)  (IS_LBK(x) ? REGEX_CLOSE_CASE(x) : REGEX_OPEN)
#define REGEX_CASE(x)       (IS_SYM(x) ? REGEX_OPEN_CASE(x)  : REGEX_SYMBOL)

hashmap SYMBOL_TABLE[HASHMAP_SIZE];

//enum regexType 
//{
    //REGEX_SYMBOL,
    //REGEX_OPEN,
    //REGEX_CLOSE,
    //REGEX_ALTERNATION,
    //REGEX_CLOSURE,
    //REGEX_CONCAT,
//};

//typedef enum regexTransform
//{
    //ALTERNATION,
    //CONCAT,
    //CLOSURE,
//}
//regexTransform;

//idx nextOccurance(string x, byte c) 
//{
    //string y = x;
    //while(*y++!=c){}
    //return y-x;
//}

//regexTransform regexStatus = CONCAT;
//NFA* currNFA = NULL;
//NFA* prevNFA = NULL;

//NFA* alterationNFA(NFA* x, NFA* y) 
//{
    //NFA*  G = INIT_NFA;
    //initTransition(NFA_START(G), NFA_START(x), EPSILON);
    //initTransition(NFA_START(G), NFA_START(x), EPSILON);
    //initTransition(NFA_END(x), NFA_END(G), EPSILON);
    //initTransition(NFA_END(y), NFA_END(G), EPSILON);
    //return G;
//}

//NFA* concatNFA(NFA* x, NFA* y) 
//{
    //NFA* G = INIT_NFA;
    //initTransition(NFA_START(G), NFA_START(x), EPSILON);
    //initTransition(NFA_END(y), NFA_END(G), EPSILON);
    //return G;
//}

//NFA* closureNFA(NFA* x) 
//{
    //NFA* G = INIT_NFA;
    //initTransition(NFA_END(x), NFA_START(x), EPSILON);
    //initTransition(NFA_START(G), NFA_END(G), EPSILON);
    //return G;
//}

//NFA* constructNFA() 
//{
    //switch(regexStatus) 
    //{
	//case CONCAT:      currNFA = concatNFA(prevNFA, currNFA);
			  //break; 
	//case ALTERNATION: currNFA = alterationNFA(prevNFA, currNFA); 
			  //break; 
	//case CLOSURE:     currNFA = closureNFA(currNFA);
			  //break; 
	//default:          break;  
    //}
    //return currNFA;
//}

//string nextSymbol(string x) 
//{
    //while (IS_SYM(*x++)==0){}
    //NFA*         G = INIT_NFA;
    //idx          n = (IS_RAN(*x) ? nextOccurance(x, R_RANGE) : 1);
    //string symbol = (string)calloc(n+1, 1); memcpy(symbol, x, n);
    //INSERT_H(h64(symbol), &SYMBOL_TABLE, symbol);
    //initTransition(NFA_START(G), NFA_END(G), h64(symbol));

    //prevNFA = currNFA;
    //currNFA = G;
    //constructNFA();

    //return x + n;
//}


//NFA* initNFA(string x)
//{
    //while(*x)
    //{
	//switch(REGEX_CASE(*x)) 
	//{
	    //case REGEX_SYMBOL:      x            = nextSymbol(x);  
				    //break;
	    //case REGEX_OPEN:        currNFA = initNFA(++x); 
				    //break;
	    //case REGEX_ALTERNATION: regexStatus  = ALTERNATION; 
				    //break;
	    //case REGEX_CLOSURE:     regexStatus  = CLOSURE;
				    //break;
	    //case REGEX_CONCAT:      regexStatus  = CONCAT;
				    //break;
	    //case REGEX_CLOSE:       return NULL;
	    //default:                regexStatus  = CONCAT;
				    //x++;
	//}
    //}
    //return NULL;
//}

#endif
