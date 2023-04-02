#include "httpmsg.h"

void httpmsg_setHeader(struct parsed_url *url, char *method, char *httpmsg){ 
    sprintf(httpmsg, "%s %s HTTP/1.1\r\n", method, url->path);
    sprintf(httpmsg +strlen(httpmsg), "Host: %s:%s\r\n", url->hostname, url->port);
    sprintf(httpmsg +strlen(httpmsg), "Connection: close\r\n");
    sprintf(httpmsg +strlen(httpmsg), "\r\n");

    printf("--HEADER:\n%s\n", httpmsg);
}
