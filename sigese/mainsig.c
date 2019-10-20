#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

     void sigproc(int);

     void quitproc(int);

     int main(){
      signal(SIGINT, sigproc);
      signal(SIGQUIT, quitproc);
      printf("CTRL-C disabled use ctrl-\\ to quit \n");
      for(;;){} /* loop infinito */ 
    }
    
    void sigproc(int s){
         signal (SIGINT, sigproc); /* */
      /* NOTA: alcune versioni di UNIX resetteranno "signal" al valore
         di default dopo ogni chiamata. Scosi', per rispettare la 
         portabilita', facciamo un reset di "signal" ogni volta */

      printf("You have pressed CTRL-C %d\n",s);
     }
   
     void quitproc(int s){
        printf("ctrl-\\ pressed to quit %d\n",s);
        exit(0); /* normale status di uscita */
     }