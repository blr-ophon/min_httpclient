#include "recvmsg.h"

void recvmsg_toFile(FILE *f, char *msg, size_t n){
    fwrite(msg, 1, n, f); 
}
