/* Gapil.h
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
 * File Gapil.h: 
 * Set of definition for service routines
 *
 * Author: S. Piccardi
 *
 * $Id: Gapil.h,v 1.12 2003/08/16 18:30:21 piccardi Exp $
 *
 *****************************************************************************/
#include <sys/sem.h>                           /* IPC semaphore declarations */
#include <sys/shm.h>                       /* IPC shared memory declarations */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>                               /* unix standard functions */
#include <fcntl.h>                          /* file control (lock) functions */
#include <signal.h>                          /* signal handling declarations */
#include <dirent.h>                              /* directory scan functions */
#include <stdio.h>	                     /* include standard I/O library */
/*
 * Definition of semun struct; used to implement a MutexXXXX API To
 * create a Mutex use an underlaying semaphore and init it; we put
 * here all the needed data structures
 */
/* use this definition, get from the man pages */
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun {
  int val;                    /* value for SETVAL */
  struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array for GETALL, SETALL */
  struct seminfo *__buf;      /* buffer for IPC_INFO */
};
#endif
/*
 * Mutex handling Functions
 */
/* Function MutexCreate: create a mutex. See Mutex.c */
int MutexCreate(key_t ipc_key);
/* Function MutexFind: get the mutex ID given fomr IPC key. See Mutex.c */
int MutexFind(key_t ipc_key);
/* Function MutexRead: read the current value of the mutex. See Mutex.c */
int MutexRead(int sem_id);
/* Function MutexLock: to lock a mutex/semaphore. See Mutex.c */
int MutexLock(int sem_id);
/* Function MutexUnlock: to unlock a mutex/semaphore. See Mutex.c */
int MutexUnlock(int sem_id);
/* Function MutexRemove: remove the mutex/semphore. See Mutex.c */
int MutexRemove(int sem_id);
/* Function CreateMutex: create a mutex (using file locking). See Mutex.c */
int CreateMutex(const char *path_name);
/* Function UnlockMutex: find a mutex (using file locking). See Mutex.c */
int FindMutex(const char *path_name);
/* Function LockMutex: acquire a mutex (using file locking). See Mutex.c */
int LockMutex(int fd);
/* Function UnlockMutex: release a mutex (using file locking). See Mutex.c */
int UnlockMutex(int fd);
/* Function ReadMutex: read a mutex (using file locking). See Mutex.c */
int ReadMutex(int fd);
/* Function RemoveMutex: remove a mutex (using file locking). See Mutex.c */
int RemoveMutex(const char *path_name);
/* 
 * Lock files function: to create and destroy lock files
 */
/* Function LockFile: create a lock file. See FileLock.c */
int LockFile(const char* path_name);
/* Function UnlockFile: remove a lock file. See FileLock.c */
int UnlockFile(const char* path_name);
/*
 * Signal Handling Functions
 */
typedef void SigFunc(int);
/* Function Signal: Initialize a signal handler. See SigHand.c */
SigFunc * Signal(int signo, SigFunc *func);
/* Function SignalRestart: restart system calls. See SigHand.c */
SigFunc * SignalRestart(int signo, SigFunc *func);
/* Function HandSigCHLD: to handle SIGCHILD. See SigHand.c */
void HandSigCHLD(int sig);
/* 
 * Socket/Files service functions
 */
/* Function FullRead: to read from a socket. See FullRead.c */
ssize_t FullRead(int fd, void *buf, size_t count);
/* Function FullWrite: to read from a socket. See FullWrite.c */
ssize_t FullWrite(int fd, const void *buf, size_t count);
/* Function full_fread: to read from a standard file. See full_fread.c */
size_t full_fread(FILE *file, void *buf, size_t count);
/* Function full_fwrite: to write from a standard file. See full_fwrite.c */
size_t full_fwrite(FILE *file, void *buf, size_t count);
/*
 * File miscellaneous
 */
/* Function dir_scan: simple scan for a directory. See dir_scan.c */
int dir_scan(char * dirname, int(*compute)(struct dirent *));
/* Function InitFile: secure create of a file with initial content */
ssize_t InitFile(int dirfd, const char *file, const char *buf, size_t size);
/* Function full_fread: to read from a standard file. See full_fread.c */
/*
 * Shared memory handling functions. See SharedMem.c
 */
/* Function ShmCreate: create a SysV shared memory */
void * ShmCreate(key_t ipc_key, int shm_size, int perm, int fill);
/* Function ShmFind: find an existing SysV shared memory */
void * ShmFind(key_t ipc_key, int shm_size);
/* Function ShmRemove: remove a SysV shared memory */
int ShmRemove(key_t ipc_key, void * shm_ptr);
/* Function CreateShm: create a POSIX shared memory */
void * CreateShm(char * shm_name, off_t shm_size, int perm, int fill);
/* Function FindShm: find an existing POSIX shared memory */
void * FindShm(char * shm_name, off_t shm_size);
/* Function RemoveShm: remove a POSIX shared memory */
int RemoveShm(char * shm_name);
/*
 * Socket creation functions. See corresponding .c
 */
int sockconn(char *host, char *serv, int prot, int type);
int sockbind(char *host, char *serv, int prot, int type);
int sockbindopt(char *host, char *serv, int prot, int type, int reuse);

/*
 * General purpose functions. See corresponding .c
 */
int endian(void);
int is_closing(int sock);
