/* macros.h
 * 
 * Copyright (C) 2000-2002 Simone Piccardi
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
/*
 * Endianess conversion for int and short types
 */
#define BE32_TO_LE32(val) ({typeof (val) v= (val);  \
        (v>>24) | ((v>>8)&0xFF00) | (v<<24) | ((v<<8)&0xFF0000); } )
#define BE16_TO_LE16(val) ({typeof (val) v= (val);  (v>>8) | (v<<8); } )
/* 
 * Define a protected, right typed, no side effects macro for min and max
 */
#define min(x, y) ({typeof (x) x_ = (x); typeof (y) y_ = (y); \
                  x_ < y_ ? x_ : y_;}) 
#define max(x, y) ({typeof (x) x_ = (x); typeof (y) y_ = (y); \
                  x_ > y_ ? x_ : y_;}) 
/* 
 * debugging print definition
 */
#ifdef IS_DAEMON
#define report(fmt, args...) UserNotify(fmt,##args)
#else
#define report(fmt, args...) printf(fmt,##args)
#endif /* IS_DAEMON */

#ifdef DEBUG                     /* done only on debugging */
#define debug  report
#else
#define debug(fmt, arg...)
#endif /* DEBUG */


/*
 * Just to print an hex dump of a buffer,
 * ptr is the address, m is how many word per line
 * and l how many lines
 */
#define PRINT_BUF(ptr, l, m) ({ \
    int _i, _j; \
    unsigned short *_ptr= (unsigned short *)(ptr); \
    for (_i = 0; _i< (int) l; _i++) { \
	printf("Val[%d]=", m * _i); \
	for (_j = 0; _j < (int) m; _j++) { \
	    printf("%x ", *_ptr);\
	    _ptr++;\
	}\
	printf("\n"); \
    }\
})
