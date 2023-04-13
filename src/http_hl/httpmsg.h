#ifndef HTTPMSG_H
#define HTTPMSG_H

#include <string.h>
#include "parser.h"

//This struct is used to interpret headers in the response, to know how to
//interpret the body data and if the query should be resent in another way 
//i.e: missing header, change location.

//This is mostly used in response handling and in the transfer from buffer to file
struct httpmsg{
    char *content_type;
    char *new_loc;                  //new location. Unused when CHANGED_LOC flag is clear
    size_t body_length;             //unused when CHUNKED flag is set
    uint8_t flags;
};

typedef enum{
    RFLAGS_CHUNKED = (1 << 0),       //body is in chunked format
    RFLAGS_CHANGED_LOC = (1 << 1),   //Location is changed
}RESP_FLAGS;


void httpmsg_setHeader(struct parsed_url *url, char *method, char *httpmsg);

size_t httpmsg_extractHeader(char *response, char *header);

void httpmsg_handleResponse(char *response);

int httpmsg_getFieldValue(char *offset, char *field_name, char *value_buf);

void httpmsg_handleHeaders(char *header, struct httpmsg *httpmsg);

void httpmsg_getTypeSufix(struct httpmsg *msg, char *buf);

void httpmsg_free(struct httpmsg *msg);

size_t field_strlen(char *field);

#endif
