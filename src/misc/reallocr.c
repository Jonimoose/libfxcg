#include <stdlib.h>

void *
_realloc_r (struct _reent *ptr, void *old, int newlen)
{
  return realloc (old, newlen);
}
