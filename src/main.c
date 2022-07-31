#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include "routes.h"
#include "token_anti_csrf.h"
#include "file_ops.h"

char salt[16];
static const char *s_https_addr = "https://127.0.0.1:1345";  // HTTPS port


static void init_server_warrior(struct mg_connection *c, int ev, void *ev_data, void *fn_data) 
{

	if (ev == MG_EV_ACCEPT && fn_data != NULL) 
	{
		// if exist ca.pem uses Two way TLS
		if(file_exists("cert/ca.perm"))
		{
    			struct mg_tls_opts opts = {
				.ca = "cert/ca.pem",         // CA file
    	    			.cert = "cert/cert.pem",     // Certificate PEM file
    	    			.certkey = "cert/key.pem",  // This pem contains both cert and key
    			};
    			mg_tls_init(c, &opts);
		} else {
    			struct mg_tls_opts opts = {
    	    			.cert = "cert/cert.pem",     // Certificate PEM file
    	    			.certkey = "cert/key.pem",  // This pem contains both cert and key
    			};
	    		mg_tls_init(c, &opts);
		}


  	} else if (ev == MG_EV_HTTP_MSG) {


    		struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    		if (mg_http_match_uri(hm, "/health")) {
     			mg_http_reply(c, 200, "", "1");  

    		} else if (mg_http_match_uri(hm, "/websocket")) {
      			mg_ws_upgrade(c, hm, NULL);   
    		} else {
      			struct mg_http_serve_opts opts = {.root_dir = "web"};
      			mg_http_serve_dir(c, ev_data, &opts);
    		}

  	} else if (ev == MG_EV_WS_MSG) {
    	// Got websocket frame. Received data is wm->data. Echo it back!
    		struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    		broadcast(c, wm);
    		mg_iobuf_del(&c->recv, 0, c->recv.len);
    }

    char addr[1024];
    mg_straddr(c, addr, sizeof(addr));

    // only 127.0.0.1 can access, you can edit allowlist at file config/allowlist.conf, you can use regex at list...
    	if(allowlist_ip(addr)==false)
    	{
    		mg_http_reply(c, 404, "%s\n", addr);
    		return;
    	}

  	(void) fn_data;
}


int main()
{
// global salt generator, for anti-csrf token
	memset(salt,0,15);
	rand_str(salt,15);
	// start lib mongoose
	struct mg_mgr mgr;                           
	mg_log_set("0");                             
	printf("Code Warrior\n Listening : %s\n",s_https_addr);
	mg_mgr_init(&mgr);                            
	mg_http_listen(&mgr, s_https_addr, init_server_warrior, (void *) 1);	
	for (;;) mg_mgr_poll(&mgr, 1000);                  
	mg_mgr_free(&mgr);
	exit(0);
}

