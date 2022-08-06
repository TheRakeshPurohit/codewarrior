#include <stdio.h>
#include <string.h>
#include "BSD/strsec.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <pcre.h>

enum {
  TITLE,REFERENCE,DESCRIPTION,RELEVANCE,MATCH,PATH,LINES,LANG, END
};

char *filter_text_form(char *text_form_input);
char *rm_extra_slash(char *str);
int parse_viewcode(char** p, char** lex);
char  *ClearStr(char* charBuffer,int num);
int parse_eggs(char** p,char** lex);
bool match_test(const char *string,size_t string_len,const char *expression);
char *deadspace(char *str);
char *get_extension(char *extension);
char *get_relevance_icon(char *relevance);
int ishex(int x);
int URLdecode(const char *s, char *dec);
char *replace(const char *in, const char *pattern, const char *by);
