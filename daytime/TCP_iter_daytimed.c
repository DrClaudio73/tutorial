/* TCP_iter_daytimed.c
 * 
 * Copyright (C) 2001 Simone Piccardi
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
 * Program daytimed: 
 * Elementary TCP server for daytime service (port 13)
 *
 * Author: Simone Piccardi
 * Apr. 2001
 *
 * Usage: daytimed -h give all info
 *
 ****************************************************************/
/* 
 * Include needed headers
 */
//#include <sys/types.h>   /* primitive system data types */
#include <unistd.h>      /* unix standard library */
#include <arpa/inet.h>   /* IP addresses conversion utilities */
#include <sys/socket.h>  /* socket constants, types and functions */
#include <stdio.h>	 /* standard I/O library */
#include <time.h>        /* date and time constants, types and functions */
#include <string.h>	 /* C strings library */
#include <stdlib.h>	 /* C standard library */

#define MAXLINE 80
#define BACKLOG 10
/* Program begin */
void usage(void);
int main(int argc, char *argv[])
{
/* 
 * Variables definition  
 */
    int list_fd, conn_fd;
    int i;
    struct sockaddr_in serv_add;
    char buffer[MAXLINE];
    time_t timeval;
    /*
     * Input section: decode parameters passed in the calling 
     * Use getopt function
     */
    opterr = 0;	 /* don't want writing to stderr */
    while ( (i = getopt(argc, argv, "h")) != -1) {
	switch (i) {
	/* 
	 * Handling options 
	 */ 
	case 'h':  
	    printf("Wrong -h option use\n");
	    usage();
	    return(0);
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
    /* create socket */
    if ( (list_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("Socket creation error");
	exit(-1);
    }
    /* initialize address */
    memset((void *)&serv_add, 0, sizeof(serv_add)); /* clear server address */
    serv_add.sin_family = AF_INET;                  /* address type is INET */
    serv_add.sin_port = htons(13);                  /* daytime port is 13 */
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);   /* connect from anywhere */
    /* bind socket */
    if (bind(list_fd, (struct sockaddr *)&serv_add, sizeof(serv_add)) < 0) {
	perror("bind error");
	exit(-1);
    }
    /* listen on socket */
    if (listen(list_fd, BACKLOG) < 0 ) {
	perror("listen error");
	exit(-1);
    }
    /* write daytime to client */
    while (1) {
	if ( (conn_fd = accept(list_fd, (struct sockaddr *) NULL, NULL)) <0 ) {
	    perror("accept error");
	    exit(-1);
	}
	timeval = time(NULL);
	snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&timeval));
	if ( (write(conn_fd, buffer, strlen(buffer))) < 0 ) {
	    perror("write error");
	    exit(-1);
	}
	close(conn_fd);
    }

    /* normal exit */
    exit(0);
}
/*
 * routine to print usage info and exit
 */
void usage(void) {
    printf("Simple daytime server\n");
    printf("Usage:\n");
    printf("  daytimed [-h] \n");
    printf("  -h	   print this help\n");
    exit(1);
}
