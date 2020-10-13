#include "mem_ops.h"

// based in OpenBSD reallocarray() function http://man.openbsd.org/reallocarray.3
void *xmallocarray (size_t nmemb, size_t size) 
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && nmemb > 0 && SIZE_MAX / nmemb < size) 
	{
		DEBUG("integer overflow block");
		exit(0);
	}

	void *ptr = malloc (nmemb*size);

	if (ptr == NULL) 
	{
		DEBUG("Error in memory size: %lu",size);
		exit(0);
	}
	return ptr;
}

// based in OpenBSD reallocarray() function http://man.openbsd.org/reallocarray.3
void *xreallocarray (void *ptr, size_t nmemb, size_t size) 
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && nmemb > 0 && SIZE_MAX / nmemb < size) 
	{
		DEBUG("integer overflow block");
		exit(0);
	}

	void *p = realloc (ptr, nmemb*size);

	if (p == NULL) 
	{
		
		DEBUG("Error in memory size: %lu",size);
		exit(0);
	}
	
	return p;
}


static void *xmalloc_fatal(size_t size) 
{
	DEBUG("Error in memory size: %lu",size);
	exit(0);
}

void *xmalloc (size_t size) 
{
	void *ptr = malloc (size);

	if (ptr == NULL) 
		return xmalloc_fatal(size);

	return ptr;
}

void *xcalloc (size_t mem, size_t size) 
{
	void *ptr = calloc (mem, size);

	if (ptr == NULL) 
		return xmalloc_fatal(mem*size);

	return ptr;
}

void *xrealloc (void *ptr, size_t size) 
{
	void *p = realloc (ptr, size);

	if (p == NULL) 
		return xmalloc_fatal(size);

	return p;
}

void xfree(void **ptr) 
{
	assert(ptr);
	if( ptr != NULL )
        {
		free(*ptr);
		*ptr=NULL;	
        }
	
}




