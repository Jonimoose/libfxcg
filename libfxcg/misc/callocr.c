#include <stdlib.h>
#include <stddef.h>

void *
_calloc_r (struct _reent *ptr, size_t size, size_t len)
{
  return calloc (size, len);
}
