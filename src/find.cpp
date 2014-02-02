// returns 1 if expr p contains expr q, otherweise returns 0

#include "stdafx.h"
#include "defs.h"

int
find(U *p, U *q)
{
	int i;

	if (equal(p, q))
		return 1;

	if (istensor(p)) {
		for (i = 0; i < p->u.tensor->nelem; i++)
			if (find(p->u.tensor->elem[i], q))
				return 1;
		return 0;
	}

	while (iscons(p)) {
		if (find(car(p), q))
			return 1;
		p = cdr(p);
	}

	return 0;
}
