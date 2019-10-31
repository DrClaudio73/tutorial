#include <errno.h>       /* error definitions and routines */
#include <unistd.h>      /* unix standard library */

ssize_t FullWrite(int fd, const void *buf, size_t count) 
{
    size_t nleft;
    ssize_t nwritten;

    nleft = count;
    while (nleft > 0) {             /* repeat until no left */
	if ( (nwritten = write(fd, buf, nleft)) < 0) {
	    if (errno == EINTR) {   /* if interrupted by system call */
		continue;           /* repeat the loop */
	    } else {
		return(nwritten);   /* otherwise exit with error */
	    }
	}
	nleft -= nwritten;          /* set left to write */
	buf +=nwritten;             /* set pointer */
    }
    return (nleft);
}
