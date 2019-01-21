#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include "string_ops.h"
#include "file_ops.h"   // recursive directory walk
#include "validate.h"   // to validate inputs
#include "html_entities.h" // o make XSS mitigation
#include "mem_ops.h" // xmalloc() ,XFREE()...
#include "../lib/BSD/strsec.h" // OPENBSD's strlcpy(), strlcat() and strnstr() from FreeBSD
#include "../lib/frozen/frozen.h" // json parser
#include "../lib/libmongoose/mongoose.h" // HTTPd lib + krypton
#include "token_anti_csrf.h" // to generate simple token to CSRF mitigation
#include "whitelist.h" // list os whitelist to access this server, file  "conf/whitelist.conf"


struct mg_serve_http_opts s_http_server_opts;
int is_websocket(const struct mg_connection *nc); 

/*
 So this function get msgs from web socket and parse JSON, at end choice a function to execute...
*/
void broadcast(struct mg_connection *nc, const struct mg_str msg); 
void ev_handler(struct mg_connection *nc, int ev, void *ev_data); 

