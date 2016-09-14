#include "token_anti_csrf.h"
#include "../lib/sha256/sha256.h"
#include "mem_ops.h"
#include "../lib/BSD/strsec.h"
// this version uses timeout, at EULA version have session ;-]

char * make_hash(unsigned char hash[])
{
	short i=0;
	char tempValue[33]; // 32 hex digits + nullbyte

	for (i = 0; i < 16; ++i)
  		sprintf(tempValue+2*i,"%02x", hash[i]);

	char *ret=strdup(tempValue);
	ret[33]='\0';

	return ret;
}

char *time_hour_minutes(int num)
{
	char time_now[32];
	int h=0,m=0;
        time_t t = time(NULL);
        
	memset(time_now,0,31);

        h= (t / 360) % 24;  
        m= (t / 60) % 60;

        snprintf(time_now,32,"%02d:%02d", h, (m<=5)?(m+10)-num:m-num);

	char *time_tmp=strdup(time_now);

	return time_tmp;
}

void *entropy_clock(void)
{
	double cpu_time=0;
	struct timeval tv;

	cpu_time = ((double) clock()) / CLOCKS_PER_SEC;
	gettimeofday(&tv, NULL);
	srandom((getpid() << 16) ^ tv.tv_sec ^ tv.tv_usec ^ (int)cpu_time);

  	return NULL;	
}


char *rand_str(char *dst, int size)
{
	static const char text[] =	"abcdefghijklmnopqrstuvwxyz"                     
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	entropy_clock();

	int i, len = random() % (size - 1);
  
	if( !len ) 
		len=8;
 
	for ( i=0; i<len; ++i )
		dst[i] = text[random() % (sizeof text - 1)];


	return dst;
}

// have dragons here
char *gen_anticsrf_token(int num)
{
	unsigned char buf[32];
	SHA256_CTX2 ctx;
//secret key
 	char *key="FrozenWorld";
	unsigned char key_end[32];
	char *hour_minutes=time_hour_minutes(num);

	memset(key_end,0,31);
	memset(buf,0,31);

	snprintf((char *)key_end,32,"%s%s%s",salt,hour_minutes,key);

	sha256_init2(&ctx);
	sha256_update2(&ctx, key_end, 32);
	sha256_final2(&ctx, buf);

	char *out=make_hash(buf);

	free(hour_minutes);
	
	return out;
}

// check token 
bool csrf_token_check(char *token_input)
{
	char *tokens[4];
	int i=3;
	bool result=false;

// 4 minutes expire
	tokens[0]=gen_anticsrf_token(0);
	tokens[1]=gen_anticsrf_token(1);
	tokens[2]=gen_anticsrf_token(2);
	tokens[3]=gen_anticsrf_token(3);

	while(i!=-1)
	{
		if(strncmp(token_input,tokens[i],33) )
			result=true;
		XFREE(tokens[i]);
		i--;
	}

	return result;
}


