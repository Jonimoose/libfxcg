#include "stdafx.h"

#include <stdlib.h>
#include <string.h>

extern char *get_curr_cmd(void);
extern void update_curr_cmd(char *);
//extern char *get_cmd_str(int);

#define N 21

static char *buf[N];
static int i, j, k;

void
update_cmd_history(char *s)
{
	// reset history pointer

	k = i;

	// blank string?

	if (*s == 0)
		return;

	// no duplicates

	if (i != j && strcmp(s, buf[(i + N - 1) % N]) == 0)
		return;

	buf[i] = strdup(s);

	i = (i + 1) % N;

	if (i == j) {
		free(buf[j]);
		buf[j] = 0;
		j = (j + 1) % N;
	}

	k = i;
}

// k != i indicates curr cmd is historical

void
do_up_arrow(void)
{
	char *s;

	// save curr cmd if new input or change to historical input

	s = get_curr_cmd();

	if (*s) {
		if (k == i || strcmp(s, buf[k]) != 0) {
			update_cmd_history(s);
			k = (i + N - 1) % N;
		}
	}

	free(s);

	// retard history pointer

	if (k != j)
		k = (k + N - 1) % N;

	if (buf[k])
		update_curr_cmd(buf[k]);
	else
		update_curr_cmd("");
}

void
do_down_arrow(void)
{
	char *s;

	// save curr cmd if new input or change to historical input

	s = get_curr_cmd();

	if (*s) {
		if (k == i || strcmp(s, buf[k]) != 0) {
			update_cmd_history(s);
			k = (i + N - 1) % N;
		}
	}

	free(s);

	// advance history pointer

	if (k != i)
		k = (k + 1) % N;

	if (buf[k])
		update_curr_cmd(buf[k]);
	else
		update_curr_cmd("");
}

char *
get_cmd_history(void)
{
	int k, n;
	char *s, *t;

	// measure

	n = 0;
	k = j;
	while (k != i) {
		n += (int) strlen(buf[k]) + 2;
		k = (k + 1) % N;
	}

	s = (char *) malloc(n + 1);

	if (s == NULL)
		return NULL;

	// copy

	t = s;
	k = j;
	while (k != i) {
		strcpy(t, buf[k]);
		k = (k + 1) % N;
		t += strlen(t);
		*t++ = '\r';
		*t++ = '\n';
	}

	*t = 0;

	return s;
}