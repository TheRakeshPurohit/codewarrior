#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "BSD/strsec.h"
#include <sys/ptrace.h>
#include <stdlib.h>

void is_stay_debug();
bool path_is_valid(char * path);
bool extension_is_valid(char * extension);
bool module_is_valid(char * module);
bool url_viewcode_valid(char* test); 
