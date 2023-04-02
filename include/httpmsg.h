#ifndef HTTPMSG_H
#define HTTPMSG_H

#include <string.h>
#include "parser.h"

void httpmsg_setHeader(struct parsed_url *url, char *method, char *httpmsg);

#endif
