/*
most functions are using the same as bitcoin.. this can work for all similarly derivitive coins..
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <string.h>
#include "list.h"
#include "structs.h"
#include "utils.h"
#include "note_bitcoin.h"
#include "note_litecoin.h"

#define LITECOIN_MODULE_ID 21
char litecoin_magic[5] = "ABCD";

ModuleFuncs litecoin_funcs = { 
    &bitcoin_read,
    &bitcoin_write,
    &bitcoin_incoming,
    &bitcoin_outgoing,
    &litecoin_nodes,
    //&bitcoin_main_loop,
    NULL, // no connected
    NULL, // no disconnect
    NULL
};

BitcoinCustomFunctions _litecoin_custom = {
  &litecoin_build_version,
  &bitcoin_connect_nodes  
};

    
Modules CC_Litecoin = {
    // required ( NULL, NULL, 0 )
    NULL, NULL, 0, 0, 1,
    LITECOIN_MODULE_ID,
    // port, state
    9333, 0,
    // required 0, 0..  
    0, 0, 0,
    //timer = 300 seconds (5min) - get new nodes, etc
    300,
    // litecoin functions
    &litecoin_funcs,
    NULL,
    NULL, // node list
    NULL, // custom data
    (char *)&litecoin_magic,
    4

};



// add bitcoin to module list
int litecoin_init(Modules **_module_list) {
    Module_Add(_module_list, &CC_Litecoin);
    BitcoinSetCustom(&CC_Litecoin, &_litecoin_custom);
}


// obtain node list for connecting to network
// 2 ways this works.. 1 no clients = use DNS seeding
// and 2nd we can ask peers we are connected to for more
// this is pretty dumb for now.. itll just add everything and
// lookup every domain
int litecoin_nodes(Modules *note, Connection *conn, char *_buf, int _size) {
    char *dns_hosts[] = {
        "dnsseed.litecointools.com",
        NULL
    };
    struct hostent *he = NULL;
    Node *nptr = NULL;
    struct in_addr addr;
    int a = 0, i = 0;
        
    for (a = 0; dns_hosts[a] != NULL; a++) {
        he = gethostbyname2(dns_hosts[a], AF_INET);
        if (he == NULL) continue;
  
        // lets add every node we found..
        while (he->h_addr_list[i] != 0) {
            addr.s_addr = *(u_long *) he->h_addr_list[i++];
            node_add(note, addr.s_addr);
        }

    }
    
    // connect to nodes if we need em..
    //bitcoin_main_loop(note, conn, _buf, _size); 
}

char *litecoin_build_version(int *size) {
    char *buf = NULL;
    char *bptr = NULL;
    
    if ((buf = bptr = (char *)calloc(1024, 1)) == NULL) {
        return NULL;
    }
    
    // build version parameters for initial packet
    put_int32(&bptr, spoof_version);
    /*
    put_int64(&bptr, nLocalServices);
    put_int64(&bptr, nTime);
    put_uint64(&bptr, addrYou);
    put_uint64(&bptr, addrMe);
    put_uint64(&bptr, nonce);
    */
    put_str(&bptr, "killing bitcoin / litecoin", 16);

    *size = (int)(bptr - buf);
    
    return buf;    
}

