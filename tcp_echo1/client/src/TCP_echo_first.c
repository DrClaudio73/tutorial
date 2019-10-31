/* TCP_echo_first.c
 * 
 * Copyright (C) 2001-2003 Simone Piccardi
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
 * Program TCP_echo_first.c (former ElemEchoTCPClient)
 * Simple TCP client for echo service (port 7)
 * First version, broken in many ways, that will be corrected later
 *
 * Author: Simone Piccardi
 * Jun. 2001
 *
 * Usage: echo -h give all info's
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
#include <string.h>	 /* C strings library */
#include <stdlib.h>	 /* C standard library */

#include "Gapil.h"
#include "macros.h"

#include <unistd.h>      /* unix standard library */
#include <errno.h>       /* error definitions and routines */

#define MAXLINE 256

void usage(void);
void ClientEcho(FILE * filein, int socket);

/* Program begin */
int main(int argc, char *argv[])
{
/* 
 * Variables definition  
 */
    int sock_fd, i;
    struct sockaddr_in serv_add;
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
	    return(1);
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
    if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("Socket creation error");
	return 1;
    }
    /* initialize address */
    memset((void *) &serv_add, 0, sizeof(serv_add)); /* clear server address */
    serv_add.sin_family = AF_INET;                   /* address type is INET */
    serv_add.sin_port = htons(7);                    /* echo port is 7 */
    /* build address using inet_pton */
    if ( (inet_pton(AF_INET, argv[optind], &serv_add.sin_addr)) <= 0) {
	perror("Address creation error");
	return 1;
    }
    /* extablish connection */
    if (connect(sock_fd, (struct sockaddr *)&serv_add, sizeof(serv_add)) < 0) {
	perror("Connection error");
	return 1;
    }
    /* read daytime from server */
    ClientEcho(stdin, sock_fd);
    /* normal exit */
    return 0;
}
/*
 * routine to print usage info and exit 
 */
void usage(void) {
    printf("Take daytime from a remote host \n");
    printf("Usage:\n");
    printf("  daytime [-h] [-v] [host in dotted decimal form] \n");
    printf("  -v	   set verbosity on\n");
    printf("  -h	   print this help\n");
    exit(1);
}

void ClientEcho(FILE * filein, int socket) 
{
    char sendbuff[MAXLINE+1], recvbuff[MAXLINE+1];
    int nread; 
    while (fgets(sendbuff, MAXLINE, filein) != NULL) {
	FullWrite(socket, sendbuff, strlen(sendbuff)); 
	nread = read(socket, recvbuff, strlen(sendbuff));
	recvbuff[nread] = 0;
	fputs(recvbuff, stdout);
    }
    return;
}
