#include "string_ops.h"
#include "mem_ops.h"

#define array_elements(array) (sizeof(array) / sizeof *(array))

// filter text of form input
// TODO fix it 
char *filter_text_form(char *text_module)
{

	char newline[]={' ',' ','\n'};
	char newt[]={"\\t"};
	char new2[]={' '};
	char *clean_text=replace(text_module,"\\n",newline);
	char *clean_text2=replace(clean_text,newt,new2);
	char *clean_text3=replace(clean_text2," \\t","  ");

	XFREE(clean_text);
	XFREE(clean_text2);

	char *ret=rm_extra_slash(clean_text3);
	char *ret2=rm_extra_slash(ret);

//	XFREE(clean_text3);

	return ret2;
}





char *rm_extra_slash(char *str)
{
        char *out = str, *put = str;
	short i=0;

        while(*str != '\0')
        {
                if(*str == '\\')
			i++;

		if(i==2)
			i=0;
		else
                	*put=*str,put++;
	
		str++;
	        	
        }

        *put = '\0';

        return out;
}




char *deadspace(char *str)
{
        char *out = str, *put = str;

        for(; *str != '\0'; ++str)
        {
                if(*str != ' ' || *str != '_' || *str != '\n' || *str != '\r')
                        *put++ = *str;
        }

        *put = '\0';

        return out;
}


char  *ClearStr(char* charBuffer,int num) 
{
	char *tmp=(charBuffer + num);
	tmp[strlen(tmp)-2]='\0';

	return tmp;
}

// DFA to parse egg module
int parse_eggs(char** p, char** lex)
{
      char* marker;

    for (;;) {
    *lex = *p;
    
	{
		char yych;
		unsigned int yyaccept = 0;

		yych = (char)**p;
		switch (yych) {
		case 0x00:	goto yy4;
		case ':':	goto yy2;
		default:	goto yy6;
		}
yy2:
		yyaccept = 0;
		yych = (char)*(marker = ++*p);
		switch (yych) {
		case ':':	goto yy7;
		default:	goto yy3;
		}
yy3:

		{ continue; }

yy4:
		++*p;

		{ return END; }

yy6:
		yych = (char)*++*p;
		goto yy3;
yy7:
		yych = (char)*++*p;
		switch (yych) {
		case 'D':
		case 'd':	goto yy10;
		case 'M':
		case 'm':	goto yy12;
		case 'R':
		case 'r':	goto yy11;
		case 'T':
		case 't':	goto yy9;
		default:	goto yy8;
		}
yy8:
		*p = marker;
		switch (yyaccept) {
		case 0: 	goto yy3;
		case 1: 	goto yy24;
		case 2: 	goto yy58;
		}
yy9:
		yych = (char)*++*p;
		switch (yych) {
		case 'I':
		case 'i':	goto yy77;
		default:	goto yy8;
		}
yy10:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy60;
		default:	goto yy8;
		}
yy11:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy26;
		default:	goto yy8;
		}
yy12:
		yych = (char)*++*p;
		switch (yych) {
		case 'A':
		case 'a':	goto yy13;
		default:	goto yy8;
		}
yy13:
		yych = (char)*++*p;
		switch (yych) {
		case 'T':
		case 't':	goto yy14;
		default:	goto yy8;
		}
yy14:
		yych = (char)*++*p;
		switch (yych) {
		case 'C':
		case 'c':	goto yy15;
		default:	goto yy8;
		}
yy15:
		yych = (char)*++*p;
		switch (yych) {
		case 'H':
		case 'h':	goto yy16;
		default:	goto yy8;
		}
yy16:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy17;
		default:	goto yy8;
		}
yy17:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy18;
		default:	goto yy8;
		}
yy18:
		yych = (char)*++*p;
		switch (yych) {
		case '#':	goto yy19;
		default:	goto yy8;
		}
yy19:
		yych = (char)*++*p;
		switch (yych) {
		case '#':
		case ':':
		case '>':	goto yy8;
		default:	goto yy20;
		}
yy20:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case ' ':	goto yy22;
		case '#':
		case ':':	goto yy25;
		case '>':	goto yy8;
		default:	goto yy20;
		}
yy22:
		yyaccept = 1;
		marker = ++*p;
		yych = (char)**p;
		switch (yych) {
		case ' ':	goto yy22;
		case '#':
		case ':':	goto yy25;
		case '>':	goto yy24;
		default:	goto yy20;
		}
yy24:

		{ return MATCH; }

yy25:
		yych = (char)*++*p;
		goto yy24;
yy26:
		yych = (char)*++*p;
		switch (yych) {
		case 'F':
		case 'f':	goto yy28;
		case 'L':
		case 'l':	goto yy27;
		default:	goto yy8;
		}
yy27:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy45;
		default:	goto yy8;
		}
yy28:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy29;
		default:	goto yy8;
		}
yy29:
		yych = (char)*++*p;
		switch (yych) {
		case 'R':
		case 'r':	goto yy30;
		default:	goto yy8;
		}
yy30:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy31;
		default:	goto yy8;
		}
yy31:
		yych = (char)*++*p;
		switch (yych) {
		case 'N':
		case 'n':	goto yy32;
		default:	goto yy8;
		}
yy32:
		yych = (char)*++*p;
		switch (yych) {
		case 'C':
		case 'c':	goto yy33;
		default:	goto yy8;
		}
yy33:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy34;
		default:	goto yy8;
		}
yy34:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy35;
		default:	goto yy8;
		}
yy35:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy36;
		default:	goto yy8;
		}
yy36:
		yych = (char)*++*p;
		switch (yych) {
		case '(':	goto yy37;
		default:	goto yy8;
		}
yy37:
		yych = (char)*++*p;
		switch (yych) {
		case ')':	goto yy8;
		default:	goto yy39;
		}
yy38:
		++*p;
		yych = (char)**p;
yy39:
		switch (yych) {
		case ')':	goto yy40;
		case '>':	goto yy8;
		default:	goto yy38;
		}
yy40:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case ')':	goto yy40;
		case ':':	goto yy42;
		default:	goto yy8;
		}
yy42:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy43;
		default:	goto yy8;
		}
yy43:
		++*p;

		{ return REFERENCE; }

yy45:
		yych = (char)*++*p;
		switch (yych) {
		case 'V':
		case 'v':	goto yy46;
		default:	goto yy8;
		}
yy46:
		yych = (char)*++*p;
		switch (yych) {
		case 'A':
		case 'a':	goto yy47;
		default:	goto yy8;
		}
yy47:
		yych = (char)*++*p;
		switch (yych) {
		case 'N':
		case 'n':	goto yy48;
		default:	goto yy8;
		}
yy48:
		yych = (char)*++*p;
		switch (yych) {
		case 'C':
		case 'c':	goto yy49;
		default:	goto yy8;
		}
yy49:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy50;
		default:	goto yy8;
		}
yy50:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy51;
		default:	goto yy8;
		}
yy51:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy52;
		default:	goto yy8;
		}
yy52:
		yych = (char)*++*p;
		switch (yych) {
		case '(':	goto yy53;
		default:	goto yy8;
		}
yy53:
		yych = (char)*++*p;
		switch (yych) {
		case ')':
		case ':':
		case '>':	goto yy8;
		default:	goto yy54;
		}
yy54:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case ' ':	goto yy56;
		case ')':
		case ':':	goto yy59;
		case '>':	goto yy8;
		default:	goto yy54;
		}
yy56:
		yyaccept = 2;
		marker = ++*p;
		yych = (char)**p;
		switch (yych) {
		case ' ':	goto yy56;
		case ')':
		case ':':	goto yy59;
		case '>':	goto yy58;
		default:	goto yy54;
		}
yy58:

		{ return RELEVANCE; }

yy59:
		yych = (char)*++*p;
		goto yy58;
yy60:
		yych = (char)*++*p;
		switch (yych) {
		case 'S':
		case 's':	goto yy61;
		default:	goto yy8;
		}
yy61:
		yych = (char)*++*p;
		switch (yych) {
		case 'C':
		case 'c':	goto yy62;
		default:	goto yy8;
		}
yy62:
		yych = (char)*++*p;
		switch (yych) {
		case 'R':
		case 'r':	goto yy63;
		default:	goto yy8;
		}
yy63:
		yych = (char)*++*p;
		switch (yych) {
		case 'I':
		case 'i':	goto yy64;
		default:	goto yy8;
		}
yy64:
		yych = (char)*++*p;
		switch (yych) {
		case 'P':
		case 'p':	goto yy65;
		default:	goto yy8;
		}
yy65:
		yych = (char)*++*p;
		switch (yych) {
		case 'T':
		case 't':	goto yy66;
		default:	goto yy8;
		}
yy66:
		yych = (char)*++*p;
		switch (yych) {
		case 'I':
		case 'i':	goto yy67;
		default:	goto yy8;
		}
yy67:
		yych = (char)*++*p;
		switch (yych) {
		case 'O':
		case 'o':	goto yy68;
		default:	goto yy8;
		}
yy68:
		yych = (char)*++*p;
		switch (yych) {
		case 'N':
		case 'n':	goto yy69;
		default:	goto yy8;
		}
yy69:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy70;
		default:	goto yy8;
		}
yy70:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy71;
		default:	goto yy8;
		}
yy71:
		yych = (char)*++*p;
		switch (yych) {
		case '(':	goto yy72;
		default:	goto yy8;
		}
yy72:
		yych = (char)*++*p;
		switch (yych) {
		case ')':
		case ':':	goto yy8;
		default:	goto yy74;
		}
yy73:
		++*p;
		yych = (char)**p;
yy74:
		switch (yych) {
		case ')':
		case ':':	goto yy75;
		case '>':	goto yy8;
		default:	goto yy73;
		}
yy75:
		++*p;

		{ return DESCRIPTION; }

yy77:
		yych = (char)*++*p;
		switch (yych) {
		case 'T':
		case 't':	goto yy78;
		default:	goto yy8;
		}
yy78:
		yych = (char)*++*p;
		switch (yych) {
		case 'L':
		case 'l':	goto yy79;
		default:	goto yy8;
		}
yy79:
		yych = (char)*++*p;
		switch (yych) {
		case 'E':
		case 'e':	goto yy80;
		default:	goto yy8;
		}
yy80:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy81;
		default:	goto yy8;
		}
yy81:
		yych = (char)*++*p;
		switch (yych) {
		case ':':	goto yy82;
		default:	goto yy8;
		}
yy82:
		yych = (char)*++*p;
		switch (yych) {
		case '(':	goto yy83;
		default:	goto yy8;
		}
yy83:
		yych = (char)*++*p;
		switch (yych) {
		case ')':
		case ':':	goto yy8;
		default:	goto yy85;
		}
yy84:
		++*p;
		yych = (char)**p;
yy85:
		switch (yych) {
		case ')':
		case ':':	goto yy86;
		case '>':	goto yy8;
		default:	goto yy84;
		}
yy86:
		++*p;

		{ return TITLE; }

	}


    }
}

// regex test 
bool match_test(const char *string,const char *expression)
{

	if (slre_match(expression, string, strlen(string), 0, 0, 0) > 0) 
  		return true;
	else 
		return false;
}




int parse_viewcode(char** p, char** lex)
{
    char* marker;

    for (;;) {
    *lex = *p;
    

	{
		char yych;

		yych = (char)**p;
		switch (yych) {
		case 0x00:	goto yy5;
		case 'l':	goto yy4;
		case 'p':	goto yy2;
		default:	goto yy7;
		}
yy2:
		yych = (char)*(marker = ++*p);
		switch (yych) {
		case 'a':	goto yy25;
		default:	goto yy3;
		}
yy3:

		{ continue; }

yy4:
		yych = (char)*(marker = ++*p);
		switch (yych) {
		case 'a':	goto yy10;
		case 'i':	goto yy8;
		default:	goto yy3;
		}
yy5:
		++*p;

		{ return END; }

yy7:
		yych = (char)*++*p;
		goto yy3;
yy8:
		yych = (char)*++*p;
		switch (yych) {
		case 'n':	goto yy18;
		default:	goto yy9;
		}
yy9:
		*p = marker;
		goto yy3;
yy10:
		yych = (char)*++*p;
		switch (yych) {
		case 'n':	goto yy11;
		default:	goto yy9;
		}
yy11:
		yych = (char)*++*p;
		switch (yych) {
		case 'g':	goto yy12;
		default:	goto yy9;
		}
yy12:
		yych = (char)*++*p;
		switch (yych) {
		case '=':	goto yy13;
		default:	goto yy9;
		}
yy13:
		yych = (char)*++*p;
		switch (yych) {
		case '&':	goto yy9;
		default:	goto yy14;
		}
yy14:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case '&':	goto yy16;
		default:	goto yy14;
		}
yy16:
		++*p;

		{ return LANG; }

yy18:
		yych = (char)*++*p;
		switch (yych) {
		case 'e':	goto yy19;
		default:	goto yy9;
		}
yy19:
		yych = (char)*++*p;
		switch (yych) {
		case 's':	goto yy20;
		default:	goto yy9;
		}
yy20:
		yych = (char)*++*p;
		switch (yych) {
		case '=':	goto yy21;
		default:	goto yy9;
		}
yy21:
		yych = (char)*++*p;
		switch (yych) {
		case '\n':	goto yy9;
		default:	goto yy22;
		}
yy22:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case '\n':	goto yy24;
		default:	goto yy22;
		}
yy24:

		{ return LINES; }

yy25:
		yych = (char)*++*p;
		switch (yych) {
		case 't':	goto yy26;
		default:	goto yy9;
		}
yy26:
		yych = (char)*++*p;
		switch (yych) {
		case 'h':	goto yy27;
		default:	goto yy9;
		}
yy27:
		yych = (char)*++*p;
		switch (yych) {
		case '=':	goto yy28;
		default:	goto yy9;
		}
yy28:
		yych = (char)*++*p;
		switch (yych) {
		case '&':	goto yy9;
		default:	goto yy29;
		}
yy29:
		++*p;
		yych = (char)**p;
		switch (yych) {
		case '&':	goto yy31;
		default:	goto yy29;
		}
yy31:
		++*p;

		{ return PATH; }

	}


    }
}


char *get_extension(char *extension)
{
	char *language=xmalloc(16);
	size_t extension_size=strlen(extension);

	memset(language,0,15);

	if(strnstr(extension,".c",extension_size)||strnstr(extension,"cpp",extension_size))
	{
		strlcat(language,"cpp",16);
		goto etx_end;
	}

	if(strnstr(extension,".java",extension_size)||strnstr(extension,"jad",extension_size)||strnstr(extension,"class",extension_size))
	{	
		strlcat(language,"java",16);
		goto etx_end;
	}

	if(strnstr(extension,".php",extension_size))
	{
		strlcat(language,"php",16);
		goto etx_end;
	}

	if(strnstr(extension,".rb",extension_size))
	{
		strlcat(language,"ruby",16);
		goto etx_end;
	}

	if(strnstr(extension,"js",extension_size)||strnstr(extension,"html",extension_size))
	{
		strlcat(language,"javascript",16);
		goto etx_end;
	}

	if(strnstr(extension,".asp",extension_size)||strnstr(language,".cs",extension_size))
	{
		strlcat(language,"bash",16);
		goto etx_end;
	}
	
	if(strnstr(extension,".py",extension_size))
	{
		strlcat(language,"python",16);
		goto etx_end;
	}

	etx_end:
	return language;
}


char *get_relevance_icon(char *relevance)
{
	char *icon_alert=xmalloc(sizeof(char)*16);
	size_t relevance_size=strlen(relevance);

	memset(icon_alert,0,15);

	if(strnstr(relevance,"High",relevance_size)||strnstr(relevance,"high",relevance_size))
	{
		strlcat(icon_alert,"Heat.png",16);
		goto icon_end;
	}

	if(strnstr(relevance,"Low",relevance_size)||strnstr(relevance,"low",relevance_size))
	{
		strlcat(icon_alert,"Snow.png",16);
		goto icon_end;
	}

	if(strnstr(relevance,"Medium",relevance_size)||strnstr(relevance,"medium",relevance_size))
	{
		strlcat(icon_alert,"Shuriken.png",16);
		goto icon_end;
	}

	icon_end:
	return icon_alert;
}

int ishex(int x)
{
	return	(x >= '0' && x <= '9')	||
		(x >= 'a' && x <= 'f')	||
		(x >= 'A' && x <= 'F');
}
 
int URLdecode(const char *s, char *dec)
{
	char *o;
	const char *end = s + strlen(s);
	int c;
 
	for (o = dec; s <= end; o++) 
	{
		c = *s++;

		if (c == '+') 
			c = ' ';
		else if (c == '%' && (	!ishex(*s++)	||
					!ishex(*s++)	||
					!sscanf(s - 2, "%2x", &c)))
			return -1;
 
		if (dec) 
			*o = c;
	}
 
	return o - dec;
}

// audit thuis, change strcpy to strlcpy...
char *replace(const char *in, const char *pattern, const char *by)
{
    size_t outsize = strlen(in) + 1;
    char *res = xmallocarray(outsize,sizeof(char));
    size_t resoffset = 0;
    char *needle;

    while ((needle = strstr(in, pattern))) 
    {
        memcpy(res + resoffset, in, needle - in);
        resoffset += needle - in;
        in = needle + strlen(pattern);
        outsize = outsize - strlen(pattern) + strlen(by);
        res = xreallocarray(res, outsize,sizeof(char));
        memcpy(res + resoffset, by, strlen(by));
        resoffset += strlen(by);
    }

    strlcpy(res + resoffset, in,outsize);

    return res;
}
