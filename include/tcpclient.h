#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>

#include "parser.h"

int try_connection(struct addrinfo *const addresses);

void print_addr(struct addrinfo *addr);

#endif

