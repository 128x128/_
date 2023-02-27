#ifndef STRING_H 
#define STRING_H

///////////////////////////////////////////////////////////////////////////////
//                                STRING OPS                                 //
///////////////////////////////////////////////////////////////////////////////

typedef char* string;


string strnxt(string x, char n) 
{
    while(*x++!=n){} return --x;
}

string strclr(string x) 
{
    return *x ? 0 : memset(x, 0x00, *(int*)strnxt(x, 0x01));
}

string strcpyn(string dst, string src, int n)
{
    memcpy(dst, src, n); dst[n] = 0x00; return src+n;
}

string strcpyd(string dst, string src, char n)
{
    return strcpyn( dst, src, strnxt(src, n)-src+1 );
}

string strallocn(string x, int n) 
{
    string s = (string)calloc(1, n+1); memcpy(s, x, n); return s;
}

#endif
