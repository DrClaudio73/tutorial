/* fgets example */
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

int main(void)
{
   FILE * pFile;
   char mystring [100];
   char mystring2 [101];
   int k=0;
   
   pFile = fopen ("myfile.txt" , "r");
   if (pFile == NULL) perror ("Error opening file");
   else {
    while(1){
     if ( fgets (mystring , 100 , pFile) != NULL ){
       snprintf(mystring2,strlen(mystring),"%s",mystring);
       //strncpy (mystring2, mystring, strlen(mystring)-1 );
       //mystring2[strlen(mystring2)-1]=0;
       printf("%d: ",++k);
       puts (mystring2);
       for (int i=0;i<strlen(mystring);i++){
           if (mystring[i]==10){
            printf("\\n");
           } else {
            printf("mystring[%d]: %c\tmystring2[%d]: %c\r\n",i,mystring[i],i,mystring2[i]);
           }
       }
     } else {
        break;    
     }
    }
    fclose (pFile);
   }
   return 0;
}