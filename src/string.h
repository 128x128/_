#ifndef STRING_H 
#define STRING_H

///////////////////////////////////////////////////////////////////////////////
//                                STRING OPS                                 //
///////////////////////////////////////////////////////////////////////////////

typedef char* string;


string strnxt(string x, char needle) 
{
    while(*x++!=needle){} return --x;
}

string strclr(string x) 
{
    memset(x, 0x00, *(int*)strnxt(x, 0x01)); return NULL;
}

string strcpyn(string dst, string src, int n)
{
    memcpy(dst, src, n); dst[n] = 0x00; return src+n;
}

string strallocn(string x, int n) 
{
    string s = (string)calloc(1, n+1); memcpy(s, x, n); return s;
}

#endif
