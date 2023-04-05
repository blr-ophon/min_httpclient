#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

//http://www.example.com:80/res/page1.php?user=bob#account
//
//protocol:     http
//hostname:     www.example.com
//port:         80
//path:         /res/page1.php?user=bob
//hash:         #acccount

struct parsed_url{
    char *protocol;
    char *hostname;
    char *port;
    char *path;
    char *hash;
};

void parse_url(struct parsed_url *url, char *url_str);

void free_url(struct parsed_url *url);

#endif
