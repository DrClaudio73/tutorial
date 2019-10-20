/* strtok example */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  char str[] ="+DTMF:5";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str,"+DTMF: ");
  int k=0;
  while (pch != NULL)
  {
    printf ("%d: %s ; %d\n",k++,pch,atoi(pch));
    pch = strtok (NULL, " ,.-");

  }
  return 0;
}