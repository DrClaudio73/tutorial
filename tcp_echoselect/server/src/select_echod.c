/* select_echod.c
 * 
 * Copyright (C) 2003 Simone Piccardi
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
/****************************************************************
 *
 * Program select_echod 
 * Elementary TCP server for echo service (port 7) using select
 *
 * Author: Simone Piccardi
 * Dec. 2003
 *
 * Usage: echod -h give all info
 *
 ****************************************************************/
/* 
 * Include needed headers
 */
#include <sys/types.h>   /* primitive system data types */
#include <unistd.h>      /* unix standard library */
#include <arpa/inet.h>   /* IP addresses conversion utilities */
#include <sys/socket.h>  /* socket constants, types and functions */
#include <stdio.h>	 /* standard I/O library */
#include <time.h>        /* date and time constants, types and functions */
#include <syslog.h>      /* syslog system functions */
#include <signal.h>      /* signal constants, types and functions */
#include <errno.h>       /* error definitions and routines */
#include <string.h>      /* C strings library */
#include <stdlib.h>      /* C standard library */

#define DEBUG

#include "macros.h"
#include "Gapil.h"

#define BACKLOG 10
#define MAXLINE 256
int demonize  = 1;  /* daemon use option: default is daemon */
int debugging = 0;  /* debug info printing option: default is no debug */

/* Subroutines declaration */
void usage(void);
void PrintErr(char * error);
/* Program beginning */
int main(int argc, char *argv[])
{
/* 
 * Variables definition  
 */
    int waiting = 0;
    int compat = 0;
    struct sockaddr_in s_addr, c_addr;
    socklen_t len;
    char buffer[MAXLINE];
    char fd_open[FD_SETSIZE];
    fd_set fset;
    int list_fd, fd;
    int max_fd, nread, nwrite;
    int i, n;
	int port_opt = 7;
    /*
     * Input section: decode parameters passed in the calling 
     * Use getopt function
     */
    opterr = 0;	 /* don't want writing to stderr */
    while ( (i = getopt(argc, argv, "hdicw:p:")) != -1) {
	switch (i) {
	/* 
	 * Handling options 
	 */ 
	case 'h':  
	    printf("Wrong -h option use\n");
	    usage();
	    return(0);
	    break;
	case 'i':
	    demonize = 0;
	    break;
	case 'c':
	    compat = 1;
	    break;
	case 'd':
	    debugging = 1;
	    break;
	case 'w':
	    waiting = strtol(optarg, NULL, 10);
	    break;
	case 'p':
	    port_opt = strtol(optarg, NULL, 10);
	    break;
	case '?':   /* unrecognized options */
	    printf("Unrecognized options -%c\n",optopt);
	    usage();
	default:    /* should not reached */
	    usage();
	}
    }
    /* ***********************************************************
     * 
     *		 Options processing completed
     *
     *		      Main code beginning
     * 
     * ***********************************************************/
    /* Main code begin here */
    if (compat) {                             /* install signal handler */
	Signal(SIGCHLD, HandSigCHLD);         /* non restarting handler */
    } else {
	SignalRestart(SIGCHLD, HandSigCHLD);  /* restarting handler */
    }
    /* create socket */
    if ( (list_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("Socket creation error");
	exit(1);
    }
    /* initialize address */
    memset((void *)&s_addr, 0, sizeof(s_addr));   /* clear server address */
    s_addr.sin_family = AF_INET;                  /* address type is INET */
    s_addr.sin_port = htons(port_opt);                   /* echo port is 7 */
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* connect from anywhere */
    /* bind socket */
    if (bind(list_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
	perror("bind error");
	exit(1);
    }
    /* release privileges and go daemon */
    if (setgid(65534) !=0) { /* first give away group privileges */
	perror("cannot give away group privileges");
	exit(1);
    }
    if (setuid(65534) !=0) { /* and only after user ... */
	perror("cannot give away user privileges");
	exit(1);
    }
    if (demonize) {          /* go daemon */
        openlog(argv[0], 0, LOG_DAEMON); /* open logging */
 	if (daemon(0, 0) != 0) {
	    perror("cannot start as daemon");
	    exit(1);
	}
    }
    /* main body */
    if (listen(list_fd, BACKLOG) < 0 ) {
	PrintErr("listen error");
	exit(1);
    }
    if (waiting) sleep(waiting);
    /* initialize all needed variables */
    memset(fd_open, 0, FD_SETSIZE);   /* clear array of open files */
    max_fd = list_fd;                 /* maximum now is listening socket */
    fd_open[max_fd] = 1;
    /* main loop, wait for connection and data inside a select */
    while (1) {    
	FD_ZERO(&fset);               /* clear fd_set */
	for (i = list_fd; i <= max_fd; i++) { /* initialize fd_set */
	    if (fd_open[i] != 0) FD_SET(i, &fset); 
	}
	while ( ((n = select(max_fd + 1, &fset, NULL, NULL, NULL)) < 0) 
		&& (errno == EINTR));         /* wait for data or connection */
	if (n < 0) {                          /* on real error exit */
	    PrintErr("select error");
	    exit(1);
	}
	/* on activity */
	debug("Trovati %d socket attivi\n", n);
	if (FD_ISSET(list_fd, &fset)) {       /* if new connection */
	    n--;                              /* decrement active */
	    len = sizeof(c_addr);             /* and call accept */
	    if ((fd = accept(list_fd, (struct sockaddr *)&c_addr, &len)) < 0) {
		PrintErr("accept error");
		exit(1);
	    }
		char client_addr[50];
		int client_port=ntohs(c_addr.sin_port);
		
		printf("new client from addr: %s, on port: %d\r\n", inet_ntop(AF_INET,(void*) &c_addr.sin_addr.s_addr, client_addr,sizeof(client_addr)),client_port);

	    debug("Connessione su fd %d restano %d socket attivi\n", fd, n);
	    fd_open[fd] = 1;                  /* set new connection socket */
	    if (max_fd < fd) max_fd = fd;     /* if needed set new maximum */
	    debug("max_fd=%d\n", max_fd);
		printf("fd_open[]= ");
		for (int yy=0;yy<FD_SETSIZE-1;yy++) {
			printf("%d ",fd_open[yy]);
		}
	}
	/* loop on open connections */
	i = list_fd;                  /* first socket to look */
	while (n != 0) {              /* loop until active */
	    i++;                      /* start after listening socket */
	    debug("restano %d socket, fd %d\n", n, fd);
	    if (fd_open[i] == 0) continue;   /* closed, go next */
	    if (FD_ISSET(i, &fset)) {        /* if active process it*/
		n--;                         /* decrease active */
		debug("dati su fd %d\n", i);
		nread = read(i, buffer, MAXLINE);     /* read operations */
		if (nread < 0) {
		    PrintErr("Errore in lettura");
		    exit(1);
		}
		if (nread == 0) {            /* if closed connection */
		    debug("fd %d chiuso\n", i);
		    close(i);                /* close file */
		    fd_open[i] = 0;          /* mark as closed in table */
		    if (max_fd == i) {       /* if was the maximum */
			while (fd_open[--i] == 0);    /* loop down */
			max_fd = i;          /* set new maximum */
			debug("nuovo max_fd %d\n", max_fd);
			break;               /* and go back to select */
		    }
		    continue;                /* continue loop on open */
		}
		nwrite = FullWrite(i, buffer, nread); /* write data */
		if (nwrite) {
		    PrintErr("Errore in scrittura");
		    exit(1);
		}
	    }
	}
    }
    /* normal exit, never reached */
    exit(0);
}
/*
 * routine to print usage info and exit
 */
void usage(void) {
    printf("Elementary echo server\n");
    printf("Usage:\n");
    printf("  echod [-h] \n");
    printf("  -h	   print this help\n");
    printf("  -d	   write debug info\n");
    printf("  -i	   use interactively\n");
    printf("  -c	   disable BSD semantics\n");
    printf("  -w N	   wait N sec. before calling accept\n");
    exit(1);
}
/*
 * routine to print error on stout or syslog
 */
void PrintErr(char * error) {
    if (demonize) {                       /* daemon mode */
	syslog(LOG_ERR, "%s: %m", error); /* log string and error message */
    } else {
	perror(error);
    }
    return;
}
