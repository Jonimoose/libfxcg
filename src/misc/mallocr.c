#include <stdlib.h>

void *
_malloc_r (struct _reent *ptr, int size)
{
  return malloc (size);
}
