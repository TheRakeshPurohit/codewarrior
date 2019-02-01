#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include "routes.h"

const char *s_ssl_cert = "cert/certkey.pem";
const char *s_ssl_key = "cert/certkey.key";
const char *port = "1345"; //port to open
	
int main()
{
	struct mg_mgr mgr;
	struct mg_connection *nc;
  	struct mg_bind_opts bind_opts;
  	const char *err;

// global salt generator, for anti-csrf token
	memset(salt,0,15);
	rand_str(salt,15);

 	mg_mgr_init(&mgr, NULL);
	memset(&bind_opts, 0, sizeof(bind_opts));
  	bind_opts.ssl_cert = s_ssl_cert;
  	bind_opts.ssl_key = s_ssl_key;
  	bind_opts.error_string = &err;

  	printf("Starting SSL server on port %s, cert from %s, key from %s\n",
         port, bind_opts.ssl_cert, bind_opts.ssl_key);
  	nc = mg_bind_opt(&mgr, port, ev_handler, bind_opts);

  	if (nc == NULL) 
	{
    		DEBUG("Failed to create listener: %s\n", err);
    		return 1;
  	}

	mg_set_protocol_http_websocket(nc);

  	s_http_server_opts.document_root = "web/";
  	s_http_server_opts.dav_document_root = "web/";  // Allow access via WebDav
  	s_http_server_opts.enable_directory_listing = "no";

  	fprintf(stdout,"Code Warrior version 0.5\nserver started at port %s\nOpen your browser in https://127.0.0.1:%s\n", port,port);


  	for (;;) 
    		mg_mgr_poll(&mgr, 1000);
  	
  
  	mg_mgr_free(&mgr);

	exit(0);
}

