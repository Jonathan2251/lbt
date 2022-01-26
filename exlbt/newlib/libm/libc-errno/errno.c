/* The errno variable is stored in the reentrancy structure.  This
   function returns its address for use by the macro errno defined in
   errno.h.  */

#if 1
int errno;
#else
#include <errno.h>
#include <reent.h>

#ifndef _REENT_ONLY

int *
__errno ()
{
  return &_REENT->_errno;
}

#endif

#endif
