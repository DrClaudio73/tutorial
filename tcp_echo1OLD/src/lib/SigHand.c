/* SigHand.c
 * 
 * Copyright (C) 2002 Simone Piccardi
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*****************************************************************************
 *
 * File SigHand.c: define a set of functions for signal manipulation 
 *
 * Author: S. Piccardi Dec. 2002
 *
 *****************************************************************************/
#include <errno.h>       /* error definitions and routines */
#include <stdio.h>	 /* standard I/O library */
#include <signal.h>      /* signal constants, types and functions */
#include <sys/types.h>   /* primitive system data types */
#include <sys/wait.h> 	 /* process termination constants and functions */
#include <errno.h>       /* error definitions and routines */


#include <string.h>
#include "Gapil.h"
#include "macros.h"

/*
 * Function Signal
 * Initialize a signal handler.
 * To enable the signal handling a process we need to tell it to
 * kernel; this is done writing all needed info to a sigaction structure
 * named sigact, and then callind sigaction() system call passing the
 * information stored in the sigact structure variable.
 *
 * Input:  the signal to handle 
 *         the signal handler function
 * Return: the previous sigaction structure
 */
inline SigFunc * Signal(int signo, SigFunc *func) 
{
    struct sigaction new_handl, old_handl;
    new_handl.sa_handler = func;             /* set signal handler */
    /* clear signal mask: no signal blocked during execution of func */
    if (sigemptyset(&new_handl.sa_mask)!=0){ /* initialize signal set */
        return SIG_ERR;
    }
    new_handl.sa_flags=0;                    /* init to 0 all flags */
    /* change action for signo signal */
    if (sigaction(signo, &new_handl, &old_handl)){ 
        return SIG_ERR;
    }
    return (old_handl.sa_handler);
}

/*
 * Function SignalRestart
 * Initialize a signal handler.
 * To enable the signal handling a process we need to tell it to
 * kernel; this is done writing all needed info to a sigaction structure
 * named sigact, and then callind sigaction() system call passing the
 * information stored in the sigact structure variable.
 * This version enable BSD semantics with SA_RESTART
 *
 * Input:  the signal to handle 
 *         the signal handler function
 * Return: the previous sigaction structure
 */
inline SigFunc * SignalRestart(int signo, SigFunc *func) 
{
    struct sigaction new_handl, old_handl;
    new_handl.sa_handler = func;             /* set signal handler */
    new_handl.sa_flags = SA_RESTART;         /* restart system call */
    /* clear signal mask: no signal blocked during execution of func */
    if (sigemptyset(&new_handl.sa_mask)!=0){ /* initialize signal set */
        return SIG_ERR;
    }
    /* change action for signo signal */
    if (sigaction(signo, &new_handl, &old_handl)){ 
        return SIG_ERR;
    }
    return (old_handl.sa_handler);
}


/* 
 * Functions: HandSigCHLD
 * Generic handler for SIGCHLD signal
 * 
 * Simone Piccardi Dec. 2002
 */
void HandSigCHLD(int sig)
{
    int errno_save;
    int status;
    pid_t pid;
    /* save errno current value */
    errno_save = errno;
    /* loop until no */
    do {
	errno = 0;
	pid = waitpid(WAIT_ANY, &status, WNOHANG);
    char buffer[125];
    
	if (pid > 0) {
        sprintf(buffer,"child %d terminated with status %x\n", pid, status);
	    perror(buffer);
	}
    } while (pid > 0);
    /* restore errno value*/
    errno = errno_save;
    /* return */
    return;
}
