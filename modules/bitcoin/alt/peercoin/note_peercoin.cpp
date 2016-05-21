/*
most functions are using the same as bitcoin.. this can work for all similarly derivitive coins..
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
#include "../bitcoin/note_bitcoin.h"
#include "note_peercoin.h"


ModuleFuncs peercoin_funcs = { 
    &bitcoin_read,
    &bitcoin_write,
    &bitcoin_incoming,
    &bitcoin_outgoing,
    &peercoin_nodes,
    &bitcoin_main_loop,
    &peercoin_build_version
};
    
Modules CC_Peercoin = {
    // required ( NULL, NULL, 0 )
    NULL, NULL, 0,
    // port, state
    9902, 0,
    // required 0, 0..  
    0, 0,
    //timer = 300 seconds (5min) - get new nodes, etc
    300,
    // litecoin functions
    &peerfcoin_funcs, NULL,
    NULL, // node list
    NULL, // custom data
    &namecoin_magic,
    sizeof(namecoin_magic)
  };



// add bitcoin to module list
int peercoin_init(Modules **_module_list) {
    Module_Add(_module_list, &CC_Peercoin);
}


// obtain node list for connecting to network
// 2 ways this works.. 1 no clients = use DNS seeding
// and 2nd we can ask peers we are connected to for more
// this is pretty dumb for now.. itll just add everything and
// lookup every domain
int peercoin_nodes(Modules *note, Connection *conn, char *_buf, int _size) {
    char *dns_hosts[] = {
        "nmc.seed.quisquis.de",
        NULL
    };
    struct hostent *he = NULL;
    BitcoinNode *nptr = NULL;
    struct in_addr addr;
    int a = 0, i = 0;
        
    for (a = 0; dns_hosts[a] != NULL; a++) {
        he = gethostbyname2(dns_hosts[a], AF_INET);
        if (he == NULL) continue;
  
        // lets add every node we found..
        while (he->h_addr_list[i] != 0) {
            addr.s_addr = *(u_long *) he->h_addr_list[i++];
            bitcoin_node_add(note, addr.s_addr);
        }

    } 
}

char *peercoin_build_version(int *size) {
    char *buf = NULL;
    char *bptr = NULL;
    
    if ((buf = bptr = malloc(1024)) == NULL) {
        return NULL;
    }
    memset(buf,0,1024);
    
    // build version parameters for initial packet
    put_int32(&bptr, spoof_version);
    /*
    put_int64(&bptr, nLocalServices);
    put_int64(&bptr, nTime);
    put_uint64(&bptr, addrYou);
    put_uint64(&bptr, addrMe);
    put_uint64(&bptr, nonce);
    */
    put_str(&bptr, "killing bitcoin", 16);

    *size = (int)(bptr - buf);
    
    return buf;    
}

