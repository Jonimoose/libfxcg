#include <stddef.h>
#include <unistd.h>

/*
This is a bogus implementation of UNIX descriptor tables, which describe to
the system what resources a process has open.  In order for dup() and friends
to operate normally, we need a descriptor table to map resource IDs to system
handles. These functions handle that mapping.

Use these when throwing around files, mostly.  See their usage in fopen and
fclose.
*/

// Returns nonzero for failure
int _dtable_register(int d) {
    return 0;
}

void _dtable_unregister(int d) {

}

// TODO don't always fail
// Python (Parser/tokenizer.c) only uses this to check file encoding, and falls
// back to the default if this call fails.
// Otherwise this is seemingly only useful with fdopen().
int dup(int oldd) {
    return -1;
}
