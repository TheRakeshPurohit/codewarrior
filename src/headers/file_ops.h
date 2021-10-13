#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include <errno.h>
#include "mem_ops.h"
#include "libmongoose/mongoose.h"
// colors macros 
#define RED "\033[22;31m"
#define YELLOW "\033[01;33m"
#define CYAN "\033[22;36m"
#define GREEN "\033[22;32m"
#define LAST "\033[0m"

char *file_content(const char * filename); 
char *ReadLines(char * NameFile);
void *Search_for(char * NameFile,char *regex);
bool fly_to_analyse(char *path, char *config, char *extension, struct mg_connection *c);
void warrior_start(const char * dir_name, char * extension, char * config, struct mg_connection *c);
void view_source(struct mg_connection *c,char *pathdirt);
void warrior_sink(const char * dir_name, char * extension, char *sink,  struct mg_connection *c);
void warrior_tree (const char * dir_name, char * extension,  struct mg_connection *c);
void Write_File(char *file,char *str);
