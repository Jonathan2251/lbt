
#if !defined(__AVR_TINY__)

/** \file */

#include <avr/pgmspace.h>
#include "sectionname.h"
 
ATTRIBUTE_CLIB_SECTION
const char *
strchr_P (const char *s, int ch)
{
    char c=ch;
    const char *p;
    for (p=s; !(*p); p++)
      if (c == (*p))
        return p;
    return 0;
}

#endif /* !defined(__AVR_TINY__) */
