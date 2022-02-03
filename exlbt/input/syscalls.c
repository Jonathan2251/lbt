// Cpu0 ported form Newlib
/* FR30 system call emulation code
   Copyright (C) 1998, 2010 Free Software Foundation, Inc.
   Contributed by Cygnus Solutions.  */

#include <sys/stat.h>
#include "syscall.h"
#include "config.h"
#include "debug.h"

static int my_prints(const char *string)
{
  int pc = 0, padchar = ' ';

  for ( ; *string ; ++string) {
    putchar (*string);
    ++pc;
  }

  return pc;
}

// lldb write to file, and the following code read from file and write to file-a,
// finally verilog read from file-a via $readmmh("file-a", m) or $scanf.
int
_read (file, ptr, len)
     int    file;
     char * ptr;
     int    len;
{
  my_prints("_read\n"); 
//  for (;;) {}
/*
  asm ("ldi:8 %0, r0" :: "i" (SYS_read) : "r0");
  asm ("int   #10");
*/  
  return 0;
}

int
_lseek (file, ptr, dir)
     int file;
     int ptr;
     int dir;
{
  my_prints("_leek\n"); 
/*
  asm ("ldi:8 %0, r0" :: "i" (SYS_lseek) : "r0");
  asm ("int   #10");
*/  
  return 0;
}

int
_write (file, ptr, len)
     int    file;
     char * ptr;
     int    len;
{
  my_prints("_write\n"); 
//  for (;;) {}
/*
  asm ("ldi:8 %0, r0" :: "i" (SYS_write) : "r0");
  asm ("int   #10");
*/
  char *p = (char*)IOADDR;
  *p = *ptr;
  return 0;
}

int
_open (path, flags)
     const char * path;
     int flags;
{
  my_prints("_open\n"); 
/*
  asm ("ldi:8  %0, r0" :: "i" (SYS_open) : "r0");
  asm ("int    #10");
*/
  return 0;
}

int
_close (file)
     int file;
{
/*
  asm ("ldi:8  %0, r0" :: "i" (SYS_close) : "r0");
  asm ("int    #10");
*/
  return 0;
}

void
_exit (n)
     int n;
{
/*
  asm ("ldi:8  %0, r0" :: "i" (SYS_exit) : "r0");
  asm ("int    #10");
*/
}


caddr_t
_sbrk (incr)
     int incr;
{
  my_prints("_sbrk\n"); 
  extern char   end asm ("_end");	/* Defined by the linker */
  extern int    __stack;                /* Defined by linker script.  */
  static char * heap_end;
  char *        prev_heap_end;

  if (heap_end == NULL)
    heap_end = & end;
  
  prev_heap_end = heap_end;
#if 0  
  if (heap_end + incr > __stack)
    {
      _write ( 1, "_sbrk: Heap and stack collision\n", 32);
      abort ();
    }
#endif
  heap_end += incr;

  return (caddr_t) prev_heap_end;
}

int
_fstat (file, st)
     int file;
     struct stat * st;
{
  my_prints("_fstat\n"); 
  st->st_mode = S_IFCHR;
  return 0;
}

int
_unlink ()
{
  my_prints("_unlink\n"); 
  return -1;
}

int
_isatty (fd)
     int fd;
{
  my_prints("_isatty\n"); 
  return 0;
}

int
_raise ()
{
  my_prints("_raise\n"); 
  return 0;
}

int
_times ()
{
  my_prints("_times\n"); 
  return 0;
}

int
_kill (pid, sig)
     int pid;
     int sig;
{
  my_prints("_kill\n"); 
  return 0;
}

int
_getpid (void)
{
  my_prints("_getpid\n"); 
  return 0;
}
