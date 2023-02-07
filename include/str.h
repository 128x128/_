#ifndef STR_H 
#define STR_H

typedef char string;

int str2int(string* n) {
    char* c=n; char* s=n; int l=0; int x=0;
    while(c>='0'&&c<='9'){c++;l++;}
    for(int i=0;i<l;i++){x+=(l*10*(s-'0'));s++;l--;}
    return x;
}
int str2tree(string* n, char s, char e) {
    return;
}

#endif
