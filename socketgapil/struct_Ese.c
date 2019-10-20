#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct esemptio {
int a;
char b;
} esemptio_st;

int main(){
    struct esemptio Mia;
    struct esemptio Mia2;
    Mia.a=1;
    Mia.b ='a';
    printf("Mia.a = %d , Mia.b = %c\r\n",Mia.a, Mia.b);
    printf("Mia2.a = %d , Mia2.b = %c\r\n",Mia2.a, Mia2.b);
    Mia2=Mia;
    printf("Mia2.a = %d , Mia2.b = %c\r\n",Mia2.a, Mia2.b);
    
    return 0;
}