#ifndef _TOKEN_ANTI_CSRF_H
#define _TOKEN_ANTI_CSRF_H
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

// random string
extern char salt[16];

char * make_hash(unsigned char hash[]);
int Base64encode_len(int len);
int Base64encode(char *encoded, const char *string, int len);

char *time_hour_minutes(int num);
void *entropy_clock(void);
char *rand_str(char *dst, int size);
char *gen_anticsrf_token(int num);
// check token 
bool csrf_token_check(char *token_input);

#endif
