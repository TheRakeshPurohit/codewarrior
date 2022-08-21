#include "file_ops.h"
#include "mem_ops.h"
#include "string_ops.h"
#include "../lib/libmongoose/mongoose.h"
#include "html_entities.h"
#include "../lib/BSD/strsec.h"
#include "validate.h"
#define MAX_file_len 1000000

bool file_exists (char *filename) 
{
	struct stat   buffer;   
	return (stat (filename, &buffer) == 0);
}

char *file_content(const char * filename) 
{
    FILE *fp = fopen(filename, "rb");
    size_t file_size;
    long pos=0;
    char *file_contents;

    	if (!fp)
        	return NULL;

    fseek(fp, 0L, SEEK_END);
    pos = ftell(fp);

    	if (pos <= 0) 
		{
        	fclose(fp);
        	return NULL;
    	}
    file_size = pos*(sizeof(char)+2);
    rewind(fp);
    file_contents = xmallocarray((file_size),sizeof(char));
    memset(file_contents,'\0',file_size-1);

    	if (!file_contents) 
		{
        	fclose(fp);
        	return NULL;
    	}

    	if (fread(file_contents, file_size, 1, fp) < 1) 
		{
        	if (ferror(fp)) 
			{
            		fclose(fp);
            		XFREE(file_contents);
            		return NULL;
        	}
    	}

   	fclose(fp);
    return file_contents;
}

//read lines of file
char *ReadLines(char * NameFile)
{
	FILE * fh;
	static char buffer[MAX_file_len];

	memset(buffer,'\0',MAX_file_len-1);

	fh = fopen(NameFile, "rb");

		if( fh == NULL )
		{

			DEBUG("error in to open() file");
			perror("Error ");
			exit(-1); 	 
			
		}

		if(fseek(fh, 0L, SEEK_END)==0)
		{
	    		size_t s = ftell(fh);
	    		rewind(fh);

	    		if ( s < MAX_file_len )
	    		{
	      			if(!fread(buffer, s, 1, fh))
					DEBUG("error \n");
	    		}
		}

	 
		if( fclose(fh) == EOF )
		{
			DEBUG("Error in close() file %s",NameFile);
			exit(1);
		}

	fh=NULL;

	char *tmp=buffer;
	
	return tmp;
}




// search matchs in file
void *Search_for(char * NameFile,char *regex)
{
    long int count=1;
	size_t LineSize=0,CounterSize;
	char *lineBuffer=xcalloc(1,1);
 	char *linescount=xcalloc(1,1);
	char tmpline[3528],counter[8],line[3127];
	FILE * arq;

	memset(tmpline,0,3527);

	arq = fopen(NameFile, "r");

// todo think implement fcntl() ,toctou mitigation...
		if( arq == NULL )
		{
			DEBUG("error in to open() file"); 	 
			exit(1);
		}

	memset(line,'\0',3126);

		while( fgets(line,sizeof line,arq) )  
		{
			size_t len=strlen(line);

			if(match_test(line,len,regex)==true)
			{
				
			 	LineSize+=3128+512;
				lineBuffer=xreallocarray(lineBuffer,LineSize,sizeof(char));
				snprintf(tmpline,3528,"Line: %ld -  %s",count,line);
				strlcat(lineBuffer,tmpline,LineSize);
				CounterSize+=strlen(linescount)+9;
				linescount=xreallocarray(linescount,CounterSize,sizeof(char));
				snprintf(counter,8,"%ld,",count); // add number of lines in URL by param example: 1,21,11,56..
				strlcat(linescount,counter,CounterSize);
				memset(counter,0,7);
			}
			LineSize=0;
			CounterSize=0;	
			count++;
		}

	 
		if( fclose(arq) == EOF )
		{
			DEBUG("Error in close() file %s",NameFile);
			exit(1);
		}

	arq=NULL;

// anti XSS
	char *sanitize=html_entities(lineBuffer);
	char *pack[2];
	size_t san_len=strlen(sanitize);

		if(strlen(linescount)>=2)
		{
			pack[0]=strdup(sanitize);
			pack[1]=strdup(linescount);
		} else {
			pack[0]=NULL;
			pack[1]=NULL;
		}

	void *back=xmalloc( (2*sizeof(char *))*CounterSize+san_len );

	memcpy(back, &pack, sizeof(pack));



	return back;
}

/*
 * Open Module egg, and search by matchs and construct the report
 *
 * */
bool fly_to_analyse(char *path, char *config, char * extension, struct mg_connection *c)
{
	bool report_status=false;
	int result=0;
	size_t sz=0;
	char *language=get_extension(extension);
	char *p=ReadLines(config);
	char *last=p,*report=NULL;
	char *title,*description,*reference,*match,*relevance;	
	char title2[128],description2[512],reference2[512],match2[1024],relevance2[512];	
			
		while(!result && strlen(last)>16)
		{
			switch (parse_eggs(&p, &last)) 
			{
				case TITLE:
						sz = p - last;
						memset(title2,0,127);
						snprintf(title2,128,"%.*s", (int)sz, last);
						title=strdup(ClearStr(title2,10));
					break;

				case DESCRIPTION:
					
	 					sz = p - last;
						memset(description2,0,511);
						snprintf(description2,512,"%.*s", (int)sz, last);
						description=strdup(ClearStr(description2,16));
					break;

				case REFERENCE:

						sz = p - last - 2;
						memset(reference2,0,511);
						snprintf(reference2,512,"%.*s", (int)sz, last);
						reference=strdup(ClearStr(reference2,14));
					break;


				case RELEVANCE:

						sz = p - last;
						memset(relevance2,0,511);
						snprintf(relevance2,512,"%.*s", (int)sz, last);
						relevance=strdup(ClearStr(relevance2,14));
					break;

				case MATCH:
						sz = p - last;
						memset(match2,0,1023);
						snprintf(match2,1023,"%.*s", (int)sz, last);
						match=strdup(ClearStr(match2,10));

						char **arg=(char **)Search_for(path,match);
						char **result2=arg;

	// TODO* need validate before print out
						if(result2[0]!=NULL)
						{
	//get relevance icon
							char *icon_alert=get_relevance_icon(relevance);
	// XSS mitigation
							char *title_clean=html_entities(title),*description_clean=html_entities(description),*relevance_clean=html_entities(relevance);
							char *reference_clean=html_entities(reference),*match_clean=html_entities(match),*path_clean=html_entities(path);

	// construct the report 
							size_t sizereport=strlen(title_clean)+strlen(result2[1])+strlen(description_clean)+strlen(relevance_clean);
							sizereport+=(strlen(reference_clean)*2)+strlen(match_clean)+strlen(result2[0])+(strlen(path_clean)*2)+strlen(config)+(strlen(language)*2);
							sizereport+=strlen(icon_alert);
							sizereport+=512;
							report=xmallocarray(sizereport,sizeof(char));
							memset(report,'\0',sizereport-1);
							snprintf(report,sizereport,"<img src=\"img/kunai.png\" width=\"80\" height=\"60\" align=\"center\" ><div class=\"path well\"><b>Title:</b> %s<br> <b>Description:</b> %s<br> <b>Relevance:</b> %s<br> <b>Reference:</b> <a class=\"fancybox fancybox.iframe\" href=\"%s\">%s</a><br><b>Match:</b> %s <br><b>Path:</b> <a class=\"fancybox fancybox.iframe\" href=\"viewcode.html?path=%s&lang=%s&lines=%s\">%s</a><br><b>Module:</b> %s <img src=\"img/%s\" width=\"80\" height=\"60\" align=\"right\" ></div><pre> <code class=\"language-%s\">%s</code></pre><br>",title_clean,description_clean,relevance_clean,reference_clean,reference_clean,match_clean,path_clean,language,result2[1],path_clean,config,icon_alert,language,result2[0]);


		
	// send result to web socket
							mg_ws_send(c, report, sizereport,WEBSOCKET_OP_TEXT);
							
							XFREE(result2[0]);
							XFREE(result2[1]);
							XFREE(icon_alert);
							XFREE(title_clean);
							XFREE(description_clean);
							XFREE(relevance_clean);
							XFREE(reference_clean);
							XFREE(match_clean);
							XFREE(path_clean);
							XFREE(report);

							report_status=true;
						}
					break;


				case END:
					result=1;	
					break;

	    	}
		
		
		}
		
		if(strlen(last)>16)
			XFREE( last);

	XFREE(language);
	XFREE(title);
	XFREE(description);
	XFREE(reference);
	XFREE(match);
	XFREE(relevance);	


	return report_status;
}


/*
 * Open dir on recursive mode, get file by extension...
 *
 * */
void warrior_start (const char * dir_name, char * extension, char * config,  struct mg_connection *c)
{
	DIR * d;
	char tmp_path[2048];	
	short counter=0;

 	d = opendir (dir_name);
	 
		if ( d == NULL) 
		{
			DEBUG ("Cannot open directory '%s': %s\n", dir_name, strerror (errno));
	 		exit (EXIT_FAILURE);
		}

		while (1) 
		{
			struct dirent * entry;
	 		const char * d_name;

			entry = readdir (d);

			if (! entry) 
			{
				break;
			}

			d_name = entry->d_name;
			size_t d_name_len=strlen(d_name);
		
	// TODO* i need improve that extension check
			if(d_name[0]!='.' && strncmp(d_name,"..",2)>0 && match_test(d_name,d_name_len,extension)==true)
			{
				snprintf(tmp_path,2048,"%s/%s",dir_name,d_name);
				bool result=fly_to_analyse(tmp_path, config, extension, c);

				if(result!=false )
				{
	// TODO: limit this
					counter++;
					memset(tmp_path,0,511);

					if(counter==20)
						sleep(1),counter=0;
				}

				

			}


		    if (entry->d_type & DT_DIR) 
			{

	            
		        if (strncmp (d_name, "..",2) > 0 && d_name[0]!='.') 
			    {
					char path[2048];
	 
					snprintf (path, 2048, "%s/%s", dir_name, d_name);
	                warrior_start (path,extension,config,c);
		        }
			}
		}
    

    	if(closedir(d)) 
    	{
       		perror("Could not close\n");
       		exit(0);
    	}

    
}

/*
 * Open and Show the source code with highlights
 *
 * */
void view_source(struct mg_connection *c, char *pathdirt)
{
	char pathdirt2[4094];
	memset(pathdirt2,0,4093);
	strlcat(pathdirt2,pathdirt,4094);
		
		if(pathdirt!=NULL && url_viewcode_valid(pathdirt2)==false)
		{
			DEBUG("error %s\n",pathdirt2);
			mg_ws_send(c, "invalid path", 12,WEBSOCKET_OP_TEXT);
			return;
				
		}

	size_t sz;
	int result=0;
	char *p = pathdirt;
	char *last = p;
	char path[3048],lang[32],lines[256];

	memset(path,0,3047);
	memset(lang,0,31);
	memset(lines,0,255);

	// parse the URL and get inputs
	    while(!result )
		   	 switch (parse_viewcode(&p, &last)) 
			 {
		       	  case PATH:
		         		sz = p - last - 1;
		           		snprintf(path,3048,"%.*s", (int)sz, last);
			    		memmove(path, path+5, strlen(path));
		            	break;

		          case LANG:
		          		sz = p - last - 1;
		            	snprintf(lang,32,"%.*s", (int)sz, last);
			    		memmove(lang, lang+5, strlen(lang));
		            	break;

		          case LINES: 
		            	sz = p - last - 1;
		            	snprintf(lines,256,"%.*s", (int)sz, last);
			    		short lenstr=strlen(lines);
			    		lines[lenstr-1]='\0';	    
			    		memmove(lines, lines+6, lenstr);    	
		            	break;

		          case END:
			    	result=1;	
		            	break;
		    }

	FILE * fh;
	static char buffer[MAX_file_len];

	memset(buffer,0,MAX_file_len-1);

// DEBUG("%s %s %s\n",path,lang,lines);

	char decode_path[3048];
	memset(decode_path,0,3047);
	URLdecode(path,decode_path);

	fh = fopen(decode_path, "rb");

		if( fh == NULL )
		{
			DEBUG("%s\n",path);

			perror("Error ");
			exit(-1); 	 
			
		}

		if(fseek(fh, 0L, SEEK_END)==0)
		{
	    		long s = ftell(fh);
	    		rewind(fh);

	    		if ( s < MAX_file_len )
	    		{
	      			if(!fread(buffer, s, 1, fh))
					perror("error \n");
	    		}
		}

	 
		if( fclose(fh) == EOF )
		{
			printf("Error in close() file %s",path);
			exit(1);
		}

	fh=NULL;

	char *code=html_entities(buffer);
	char *lines_clean=html_entities(lines);
	char *lang_clean=html_entities(lang);
	char *path_clean=html_entities(path);
	size_t len_output=(strlen(path_clean)+strlen(lines_clean)+strlen(lang_clean)+strlen(lang_clean)+strlen(code) )*sizeof(char);
	char *output=xmalloc(len_output);

	memset(output,0,len_output-1);

	snprintf(output,len_output,"<b>Path:</b> %s<br><pre class=\"line-numbers\" data-line=\"%s\"> <code class=\"language-%s\">%s</code></pre>",path_clean,lines_clean,lang_clean,code);

// send source code to viewcode.html
	mg_ws_send(c,  output, len_output, WEBSOCKET_OP_TEXT);

	XFREE(code);
	XFREE(lines_clean);
	XFREE(path_clean);
	XFREE(lang_clean);
	XFREE(output);
//	XFREE(pathdirt);
}


/*
 *  This function search in recursive per extension, and open each file and find one match
 *
 * */
void warrior_sink (const char * dir_name, char * extension, char *sink,  struct mg_connection *c)
{
	DIR * d;
	char tmp_path[2048];	
	char *language=get_extension(extension);
	
 	d = opendir (dir_name);
	 
		if ( d == NULL) 
		{
	//		closedir(d);
			DEBUG ("Cannot open directory '%s': %s\n", dir_name, strerror (errno));
	 		exit (EXIT_FAILURE);
		}

		while (1) 
		{
			struct dirent * entry;
	 		const char * d_name;

			entry = readdir (d);

			if (! entry) 
			{
				break;
			}

			d_name = entry->d_name;
			size_t d_name_len=strlen(d_name);
		
	// TODO* i need improve that extension check
			if(d_name[0]!='.' && strncmp(d_name,"..",2)>0 && match_test(d_name,d_name_len,extension)==true)
			{
				snprintf(tmp_path,2048,"%s/%s",dir_name,d_name);
				char **argv=(char **)Search_for(tmp_path,sink);
				char **result=(char **)argv;

				if(result[0]!=NULL)
				{
					size_t sizereport=strlen(result[0])+(strlen(result[1])*2);
					char *path_clean=html_entities(tmp_path);		
					char *sink_clean=html_entities(sink);
					sizereport+=strlen(sink_clean)+(strlen(path_clean)*2)+(strlen(language)*2)+512;	
					char *report=xmalloc(sizereport);
					memset(report,'\0',sizereport-1);		

		
					snprintf(report,sizereport,"<img src=\"img/kunai.png\" width=\"80\" height=\"60\" align=\"center\" ><div class=\"path well\"><b>Sink:</b> %s<br> <b>Lines:</b> %s<br><b>Path:</b> <a class=\"fancybox fancybox.iframe\" href=\"viewcode.html?path=%s&lang=%s&lines=%s\">%s</a><br></div><pre> <code class=\"language-%s\">%s</code></pre><br>",sink_clean,result[1],path_clean,language,result[1],path_clean,language,result[0]);
	// send result to web socket
					mg_ws_send(c, report, strlen(report), WEBSOCKET_OP_TEXT);
					memset(tmp_path,'\0',2047);

					XFREE(report);
					XFREE(result[0]);
					XFREE(result[1]);
					XFREE(path_clean);
					XFREE(sink_clean);
					XFREE(result);
				}

				
			}


		    if (entry->d_type & DT_DIR) 
			{

	            
		        if (strncmp (d_name, "..",2)>0 && d_name[0]!= '.') 
			    {
					char path[2048];
	 
					snprintf (path, 2048, "%s/%s", dir_name, d_name);
	                warrior_sink (path,extension,sink,c);
		        }
		         
			}
	    }

	    if(closedir(d)) 
	    {
	        perror("Could not close\n");
	        exit(0);
	    }


    XFREE( language);

}






/**
 *
 *  This function list tree of files...
 *
 */
void warrior_tree (const char * dir_name, char * extension,  struct mg_connection *c)
{
	DIR * d;
	char tmp_path[2048];	
	char *language=get_extension(extension);

//debug printf("%s\n",dir_name);
 	d = opendir (dir_name);
 
		if ( d == NULL) 
		{
	//		closedir(d);
			DEBUG ("Cannot open directory '%s': %s\n", dir_name, strerror (errno));
	 		exit (EXIT_FAILURE);
		}

		while (1) 
		{
			struct dirent * entry;
	 		const char * d_name;

			entry = readdir (d);

			if (! entry) 
			{
				break;
			}

			d_name = entry->d_name;
			size_t d_name_len=strlen(d_name);

		
	// TODO* i need improve that extension check
			if(d_name[0]!='.' && strncmp(d_name,"..",2)>0 && match_test(d_name,d_name_len,extension)==true)
			{
				snprintf(tmp_path,2048,"%s/%s",dir_name,d_name);
				char tree_element[4304];			
				memset(tree_element,0,4303);
				char *path_clean=html_entities(tmp_path);


				snprintf(tree_element,4303,"<div class=\"path well\"><b>Path:</b> <a class=\"fancybox fancybox.iframe\"  href=\"viewcode.html?path=%s&lang=%s&lines=1\">%s</a><br></div>",path_clean,language,path_clean);
				mg_ws_send(c,  tree_element, 4304, WEBSOCKET_OP_TEXT);
				XFREE( path_clean);
			}


		    if (entry->d_type & DT_DIR) 
			{

	            
		        if (strncmp (d_name, "..",2)>0 && d_name[0]!='.') 
			    {
					char path[2048];
	 
					snprintf (path, 2048, "%s/%s", dir_name, d_name);
	                warrior_tree (path,extension,c);
		         }
			}
	    }

	    if(closedir(d)) 
	    {
	        perror("Could not close\n");
	        exit(0);
	    }


    XFREE( language);
}

// write string in file
void Write_File(char *file,char *str)
{
	FILE *arq;
 
	arq=fopen(file,"wb"); 

		if ( arq == NULL ) 
		{
	//		fclose(arq);
			DEBUG("error in WriteFile() %s",file); 
			exit(1);
		}

	fprintf(arq,"%s",str); 

		if( fclose(arq) == EOF )
		{
			DEBUG("error in Write() file %s",file);
			exit(1);
		}
	arq=NULL;
}
