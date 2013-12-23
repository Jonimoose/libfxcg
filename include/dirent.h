#define MAX_PATH 1024

typedef struct dirent {
  /* name of current directory entry (a multi-byte character string) */
  char d_name[MAX_PATH + 1];

  /* file attributes */
  //WIN32_FIND_DATAA data;
} dirent;


typedef struct DIR {
  /* current directory entry */
  dirent current;

  /* is there an un-processed entry in current? */
  int cached;

  /* file search handle */
  int search_handle;

  /* search pattern (3 = zero terminator + pattern "\\*") */
  char patt[MAX_PATH + 3];
} DIR;
