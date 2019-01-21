#include "routes.h" // routes to make actions



int is_websocket(const struct mg_connection *nc) 
{
  return nc->flags & MG_F_IS_WEBSOCKET;
}

/*
 So this function get msgs from web socket and parse JSON, at end choice a function to execute...
*/
void broadcast(struct mg_connection *nc, const struct mg_str msg) 
{
	if(msg.p==NULL)
		return;

  size_t sizeaction=0;
  struct mg_connection *c;
  short form1=1,form2=1,form3=1,form4=1,form5=1,form6=1,counter_json=0;
  char buf[8048],total_str[128];
  char *action,*egg,*path,*extension,*sink,*text_module,*csrf_token;

	memset(buf,0,8047);
	memset(total_str,0,63);

//limit buffer, if not limit causes stack overflow...
	if(msg.len>=8048)
		return;

	if( msg.p )
	{

	  	snprintf(buf, sizeof(buf), "%.*s", (int) msg.len, msg.p);


		counter_json=json_scanf(buf, strlen(buf), "{ action:%Q, path:%Q, module:%Q, extension:%Q, sink:%Q, text_module:%Q, csrf_token:%Q }", &action, &path, &egg, &extension, &sink, &text_module, &csrf_token);

	        if( action && counter_json>=2  )	
			sizeaction=strnlen(action,5);
		else
			sizeaction=0;


  		for ( c = mg_next(nc->mgr, NULL); c != NULL && counter_json>=2; c = mg_next(nc->mgr, c) ) 
		{
  			
// this get information from form at index.html, if user click in "start"
				if(action && strnstr(action,"form1",sizeaction) && form1==1)
  				{
					if(path==NULL || !path_is_valid(path))
					{
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in Path", 14);
						break; 
					}

					if(egg==NULL || !module_is_valid(egg))	
					{
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in module", 15);
						break; 
					}


					if(extension==NULL || !extension_is_valid(extension))	
					{
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in extension", 18);
						break; 
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
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in extension", 18);
						break; 
					}

					
					if(path==NULL || !path_is_valid(path))
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in path", 13);
						break; 
					}

					form3=0;

					warrior_sink(path, extension, sink, c);

					XFREE(path);	
					XFREE(extension);					
					XFREE(sink);
					
					mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "End of sink...", 14);
					
				}

// Tree list
				if(action && strnstr(action,"form4",sizeaction) && form4==1)
  				{

					if(path==NULL || !path_is_valid(path))
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in path", 13);
						break; 
					}
						

					if(extension==NULL || !extension_is_valid(extension))
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in extension", 18);
						break; 
					}

					form4=0;

					warrior_tree(path, extension, c);

					mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "End results of tree...", 22);

					XFREE(path);
					XFREE(extension);
				}

// view module egg to edit
				if(action && strnstr(action,"form5",sizeaction) && form5==1)
  				{

					if(egg==NULL || !module_is_valid(egg) )
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in module", 15);
						break; 
					}
// token anti-csrf					
					char *token=gen_anticsrf_token(0); 
					char *module_content=file_content(egg); 
// anti-xss 
					char *content=html_entities2(module_content); // but this version allow reverse slash to use in regex
					size_t size_form=strlen(content)+150+strlen(token); 
					char *form_edit=xmallocarray(size_form,sizeof(char));
					

					snprintf(form_edit,size_form,"<input type=\"hidden\" id=\"csrf_token\" value=\"%s\"><br><textarea id=\"text_module\" rows=\"30\" cols=\"140\" >%s</textarea><br><button id=\"save\">save</button><br>", token, content);		

					mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, form_edit, size_form);

					form5=0;			
					
					XFREE(egg);
					XFREE(module_content);
					XFREE(content);
					XFREE(token);
					XFREE(form_edit);
					
				}

// write module in file
				if(action && strnstr(action,"form6",sizeaction) && form6==1)
  				{

					if(csrf_token==NULL || csrf_token_check(csrf_token)==false )
					{
						puts("error in token");
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "token invalid", 13);
						break; 
					}

								
					if(egg==NULL || !module_is_valid(egg) )
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in module", 15);
						break; 
					}

					if(text_module==NULL)
					{	
						mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, "Error in text module", 20);
						break; 
					}
	
					char *clean_text=filter_text_form(text_module);

					Write_File(egg,clean_text);					

					form6=0;
	
					XFREE(egg);	
					XFREE(text_module);
					XFREE(csrf_token);
					XFREE(clean_text);
				}
			
			XFREE(action);
  		}
    	}
}






void ev_handler(struct mg_connection *nc, int ev, void *ev_data) 
{
	switch (ev) 
	{	
		case MG_EV_HTTP_REQUEST: {

			char addr[64];
  			mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

// only 127.0.0.1 can access, you can edit whitelist at file config/whitelist.conf, you can use regex at list...
			if(whitelist_ip(addr)==true)
				mg_serve_http(nc, (struct http_message *)ev_data, s_http_server_opts);
      			break;
    		}
	
    		case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      			broadcast(nc, mg_mk_str("++ joined"));
      			break;
    		}

    		case MG_EV_WEBSOCKET_FRAME: {
			char addr[32];
  			mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

				if(whitelist_ip(addr)==true)
				{
      					struct websocket_message *wm = (struct websocket_message *) ev_data;
      					struct mg_str d = {(char *) wm->data, wm->size};
      					broadcast(nc, d);
				}
      			break;
    			}
	
    		case MG_EV_CLOSE: {
      			if (is_websocket(nc)) 
     				broadcast(nc, mg_mk_str("-- left"));
     			break;
	
    			}
  	}
}
	

