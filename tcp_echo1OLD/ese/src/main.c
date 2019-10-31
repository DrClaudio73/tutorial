// main.c
#include <stdlib.h>
#include <stdio.h>
#include "myfunc.h"

int main(void) {

  int a=6;
  int b=3;

  printf("a=%d, b=%d\r\n",a,b);
  printf("a/b=%d",divi(a,b));
  printf("a^b=%f",potenz(a,b));

  return 0;
}
