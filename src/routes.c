#include "routes.h" // routes to make actions


/*
 So this function get msgs from web socket and parse JSON, at end choice a function to execute...
*/
void broadcast(struct mg_connection *c, struct mg_ws_message *msg) 
{
	if(msg->data.ptr==NULL)
		return;

	size_t sizeaction=0;
  	short form1=1,form2=1,form3=1,form4=1,form5=1,form6=1,counter_json=0;
  	char buf[10048],total_str[128];
  	char *action,*egg,*path,*extension,*sink,*text_module,*csrf_token;

	clean(buf,sizeof(buf)-1); //last for canary
	clean(total_str,sizeof(total_str)-1);

//limit buffer, if not limit causes stack overflow...
	if(msg->data.len>=10047)
		return;

	if( msg->data.ptr )
	{

	  	snprintf(buf, sizeof(buf), "%.*s", (int) msg->data.len, msg->data.ptr);


		counter_json=json_scanf(buf, strlen(buf), "{ action:%Q, path:%Q, module:%Q, extension:%Q, sink:%Q, text_module:%Q, csrf_token:%Q }", &action, &path, &egg, &extension, &sink, &text_module, &csrf_token);

	        if( action && counter_json>=2  )	
			sizeaction=strnlen(action,5);
		else
			sizeaction=0;



// this get information from form at index.html, if user click in "start"
				if(action && strnstr(action,"form1",sizeaction) && form1==1)
  				{
					if(path==NULL || !path_is_valid(path))
					{
						mg_ws_send(c, "Error in Path", 14, WEBSOCKET_OP_TEXT);
						return; 
					}

					if(egg==NULL || !module_is_valid(egg))	
					{
						mg_ws_send(c, "Error in module", 15, WEBSOCKET_OP_TEXT);
						return; 
					}


					if(extension==NULL || !extension_is_valid(extension))	
					{
						mg_ws_send(c, "Error in extension", 18, WEBSOCKET_OP_TEXT);
						return; 
					}

					form1=0;

					warrior_start(path, extension, egg, c);			

					XFREE(path);
					XFREE(extension);
					XFREE(egg);	
				}

// show source code, input at web/viewcode.html
				if(action && strnstr(action,"form2",sizeaction) && form2==1)
				{
					
					view_source(c,path);
					form2=0;

					XFREE(path);					
				}

// sink search
				if(action && strnstr(action,"form3",sizeaction) && form3==1)
  				{

// TODO valid sink var, if have ReDOS
					if(extension==NULL || !extension_is_valid(extension))	
					{
						mg_ws_send(c, "Error in extension", 18,WEBSOCKET_OP_TEXT);
						return; 
					}

					
					if(path==NULL || !path_is_valid(path))
					{	
						mg_ws_send(c, "Error in path", 13, WEBSOCKET_OP_TEXT);
						return; 
					}

					form3=0;

					warrior_sink(path, extension, sink, c);

					XFREE(path);	
					XFREE(extension);					
					XFREE(sink);
					
					mg_ws_send(c, "End of sink...", 14, WEBSOCKET_OP_TEXT);
					
				}

// Tree list
				if(action && strnstr(action,"form4",sizeaction) && form4==1)
  				{

					if(path==NULL || !path_is_valid(path))
					{	
						mg_ws_send(c, "Error in path", 13,WEBSOCKET_OP_TEXT);
						return; 
					}
						

					if(extension==NULL || !extension_is_valid(extension))
					{	
						mg_ws_send(c, "Error in extension", 18,WEBSOCKET_OP_TEXT);
						return; 
					}

					form4=0;

					warrior_tree(path, extension, c);

					mg_ws_send(c, "End results of tree...", 22, WEBSOCKET_OP_TEXT);

					XFREE(path);
					XFREE(extension);
				}

// view module egg to edit
				if(action && strnstr(action,"form5",sizeaction) && form5==1)
  				{

					if(egg==NULL || !module_is_valid(egg) )
					{	
						mg_ws_send(c, "Error in module", 15, WEBSOCKET_OP_TEXT);
						return; 
					}
// token anti-csrf					
					char *token=gen_anticsrf_token(0); 
					char *module_content=file_content(egg); 
					size_t module_len=strlen(module_content);	
// test if have xss
					if (libinjection_xss(module_content,module_len)!= 1)
					{
						//char *content=html_entities(module_content); // if need sanitize
						size_t size_form=module_len+150+strlen(token); 
						char *form_edit=xmallocarray(size_form,sizeof(char));
					        clean(form_edit,size_form-1);	

						snprintf(form_edit,size_form,"<input type=\"hidden\" id=\"csrf_token\" value=\"%s\"><br><textarea id=\"text_module\" rows=\"30\" cols=\"140\" >%s</textarea><br><button id=\"save\">save</button><br>", token, module_content);		

						mg_ws_send(c, form_edit, size_form, WEBSOCKET_OP_TEXT);

						form5=0;			
						
						XFREE(egg);
						XFREE(module_content);
						XFREE(token);
						XFREE(form_edit);
					} else {
						XFREE(egg);
						XFREE(module_content);
						XFREE(token);
						mg_ws_send(c, "XSS Detect error !", 18, WEBSOCKET_OP_TEXT);
						return; 
					}
					
				}

// write module in file
				if(action && strnstr(action,"form6",sizeaction) && form6==1)
  				{

					if(csrf_token==NULL || csrf_token_check(csrf_token)==false )
					{
						puts("error in token");
						mg_ws_send(c, "token invalid", 13, WEBSOCKET_OP_TEXT);
						return; 
					}

								
					if(egg==NULL || !module_is_valid(egg) )
					{	
						mg_ws_send(c, "Error in module", 15, WEBSOCKET_OP_TEXT);
						return; 
					}

					if(text_module==NULL)
					{	
						mg_ws_send(c,  "Error in text module", 20, WEBSOCKET_OP_TEXT);
						return; 
					}
	
					char *clean_text=filter_text_form(text_module);
					Write_File(egg,clean_text);					

					form6=0;
	
					XFREE(egg);				
					XFREE(clean_text);
					XFREE(text_module);
					XFREE(csrf_token);
				}
			
			XFREE(action);
    }
}






	

