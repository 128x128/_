#ifndef TRIGON_H 
#define TRIGON_H

#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////
//                                Trigon                                     //
///////////////////////////////////////////////////////////////////////////////

typedef void*                 Trigon;
typedef void*                 def;
typedef void*                 ptr;


#define TRIGON(_)             (*((void**)   _))
#define I64(_)                (*((int64_t *)_))
#define U64(_)                (*((uint64_t*)_))
#define SIZEOF_VERTEX         8 
#define SIZEOF_TRIGON         SIZEOF_VERTEX * 3 
#define TRIGON_MALLOC         calloc(1, SIZEOF_TRIGON)

#define TRIGON_X(T)           (T + 0x00)
#define TRIGON_Y(T)           (T + 0x08)
#define TRIGON_Z(T)           (T + 0x10)

#define TRIGON_SET_V(V,_)     memcpy(V, _, SIZEOF_VERTEX)
#define TRIGON_SET_X(T,_)     TRIGON_SET_V(TRIGON_X(T), _)
#define TRIGON_SET_Y(T,_)     TRIGON_SET_V(TRIGON_Y(T), _) 
#define TRIGON_SET_Z(T,_)     TRIGON_SET_V(TRIGON_Z(T), _)
#define TRIGON_SET_E(V,X)     TRIGON_SET_V(V, &X)
#define TRIGON_SET(T,x,y,z)   TRIGON_SET_X(T,x);                               \
                              TRIGON_SET_Y(T,y);                               \
                              TRIGON_SET_Z(T,z);

#define TRIGON_CLR_V(V)       memset(V, 0x00, SIZEOF_VERTEX)

#define TRIGON_NEXT(T)        TRIGON(TRIGON_Y(T))
#define TRIGON_PREV(T)        TRIGON(TRIGON_X(T))
#define TRIGON_HEAD(L)        TRIGON(TRIGON_X(L))
#define TRIGON_TAIL(L)        TRIGON(TRIGON_Y(L))

#define TRIGON_ADD_BACK(L,T)  TRIGON_SET_E(TRIGON_X(T), TRIGON_TAIL(L));       \
			      TRIGON_SET_E(TRIGON_Y(TRIGON_TAIL(L)), T);       \
			      TRIGON_SET_E(TRIGON_Y(L), T);                    \
                              U64 ( TRIGON_Z(L) )++;
#define TRIGON_ADD_FRNT(L,T)  TRIGON_SET_E(TRIGON_Y(T), TRIGON_HEAD(L));       \
			      TRIGON_SET_E(TRIGON_X(TRIGON_HEAD(L)), T);       \
			      TRIGON_SET_E(TRIGON_X(L), T);                    \
                              U64 ( TRIGON_Z(L) )++;
#define TRIGON_DEL_BACK(L)    Trigon TAIL = TRIGON_TAIL(L);                    \
			      TRIGON_SET_E(TRIGON_Y(L), TRIGON_PREV(TAIL) );   \
			      TRIGON_CLR_V(TRIGON_Y(TRIGON_PREV(TAIL)));       \
			      TRIGON_CLR_V(TRIGON_X(TAIL));   \
                              U64(TRIGON_Z(L))--;
#define TRIGON_DEL_FRNT(L)    Trigon HEAD = TRIGON_HEAD(L);                    \
			      TRIGON_SET_E(TRIGON_X(L), TRIGON_NEXT(HEAD) );   \
			      TRIGON_CLR_V(TRIGON_X(TRIGON_NEXT(HEAD)));       \
			      TRIGON_CLR_V(TRIGON_Y(HEAD));                    \
                              U64(TRIGON_Z(L))--;

#define TRIGON_IS_EMPTY(T)    ( U64(TRIGON_Y(T)) && U64(TRIGON_X(T)) ) ? 0 : 1
#define TRIGON_IS_SINGLE(T)   ( U64(TRIGON_Y(T)) == U64(TRIGON_X(T)) ) ? 1 : 0

#define TRIGON_SET_LIST(L,T)  TRIGON_SET_E(TRIGON_X(L), T);                    \
                              TRIGON_SET_E(TRIGON_Y(L), T);                    \
                              U64(TRIGON_Z(L))++;
#define TRIGON_DEL_LIST(L)    TRIGON_CLR_V(TRIGON_X(L));                       \
                              TRIGON_CLR_V(TRIGON_Y(L));                       \
                              U64(TRIGON_Z(L))--;

#define TRIGON_PUSH(L,T)      if   ( TRIGON_IS_EMPTY(L)  ) {                   \
                                     TRIGON_SET_LIST(L,T)  }                   \
                              else { TRIGON_ADD_BACK(L,T ) }
#define TRIGON_POP(L)         if   ( !TRIGON_IS_EMPTY(L) ) {                   \
                              if   ( TRIGON_IS_SINGLE(L) ) {                   \
				     TRIGON_DEL_LIST(L)  }                     \
			      else { TRIGON_DEL_BACK(L)  } } 
#define TRIGON_ENQUEUE(L,T)   if   ( TRIGON_IS_EMPTY(L)  ) {                   \
                                     TRIGON_SET_LIST(L,T)  }                   \
                              else { TRIGON_ADD_FRNT(L,T ) }
#define TRIGON_DEQUEUE(L)     if   ( !TRIGON_IS_EMPTY(L) ) {                   \
                              if   ( TRIGON_IS_SINGLE(L) ) {                   \
				     TRIGON_DEL_LIST(L)  }                     \
                              else { TRIGON_DEL_FRNT(L)  } } 
// alloc ops
def talloc()                  { return (Trigon)calloc(1,sizeof(void*)*3);  }

//standard ops
def tx(Trigon T)  { return T+0x00; }
def ty(Trigon T)  { return T+0x08; }
def tz(Trigon T)  { return T+0x10; }

def tclrx(Trigon T)           { memset(tx(T),0x00,8);   return T;     }
def tclry(Trigon T)           { memset(ty(T),0x00,8);   return T;     }
def tclrz(Trigon T)           { memset(tz(T),0x00,8);   return T;     }
def tsetx(Trigon T, ptr src)  { memcpy(tx(T),src, 8);   return T;     }
def tsety(Trigon T, ptr src)  { memcpy(ty(T),src, 8);   return T;     }
def tsetz(Trigon T, ptr src)  { memcpy(tz(T),src, 8);   return T;     }
def tcpyx(Trigon T, ptr dst)  { memcpy(dst,tx(T), 8);   return T;     }
def tcpyy(Trigon T, ptr dst)  { memcpy(dst,ty(T), 8);   return T;     }
def tcpyz(Trigon T, ptr dst)  { memcpy(dst,tz(T), 8);   return T;     }

def tclr(Trigon T)                   { tclrx(T); tclry(T);  return tclrz(T);   }
def tset(Trigon T,ptr x,ptr y,ptr z) { setx(T,x);tsety(T,y);return tsetz(T,z); }

// List ops
def lnext(Trigon T) { return ty(T); }
def lprev(Trigon T) { return tx(T); }
def lhead(Trigon T) { return tx(T); }
def ltail(Trigon T) { return ty(T); }

def lempty(Trigon L)  { return ( U64(ty(L)) && U64(tx(L)) ) ? 0 : 1; }
def lsingle(Trigon L) { return ( U64(ty(L)) == U64(tx(L)) ) ? 1 : 0; }
def lset(Trigon L, Trigon T) { tsetx(T); tsety(T); }
def ldel(Trigon L) { tclr(L); }
def lpair(Trigon x, Trigon y) { tsetx(y, x); tsety(x, y); }
def tpushback(Trigon L, Trigon T) { lpair(ltail(L), T) tsety(L, T);}
def tpushfrnt(Trigon L, Trigon T) { lpair(T, lhead(L)) tsetx(L, T);}


void push(Trigon L, void* x) 
{ 
    Trigon T = TRIGON_MALLOC; 
    TRIGON_SET_Z(T,x);
    TRIGON_PUSH(L,T)
}

void enqueue(Trigon L, void* x)  
{ 
    Trigon T = TRIGON_MALLOC; 
    TRIGON_SET_Z(T,x);
    TRIGON_ENQUEUE(L,T)
}

void pop(Trigon L) 
{ 
    TRIGON_POP(L)
}

void dequeue(Trigon L)  
{ 
    TRIGON_DEQUEUE(L)
}

#endif
