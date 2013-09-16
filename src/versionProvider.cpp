#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "versionProvider.hpp"
void getVersion(char* buffer) {
  strcpy(buffer, __GIT_VERSION);
}
void getTimestamp(char* buffer) {
  strcpy(buffer, __GIT_TIMESTAMP);
}