#include "httpmsg.h"
//TODO: (mandatory) separate header from body in response, so only the header is analyzed

void httpmsg_setHeader(struct parsed_url *url, char *method, char *httpmsg){ 
    sprintf(httpmsg, "%s %s HTTP/1.1\r\n", method, url->path);
    sprintf(httpmsg +strlen(httpmsg), "Host: %s:%s\r\n", url->hostname, url->port);
    sprintf(httpmsg +strlen(httpmsg), "Connection: close\r\n");
    sprintf(httpmsg +strlen(httpmsg), "\r\n");

    printf("\n---Sending request:\n\n%s", httpmsg);
}


//extracts header from response and returns header size including the ending \r\n\r\n
size_t httpmsg_extractHeader(char *response, char *header){
    char *header_start = strstr(response, "HTTP/");

    //HTTP/ must exist and be the start of the message
    if(!header_start || (header_start != response)){
        return 0;
    }

    //Find header end and size
    char *header_end = strstr(response, "\r\n\r\n");
    if(!header_end){
        //Header has no proper end
        return 0;
    }
    size_t header_size = (uint64_t) (header_end - response) + 4;

    //copy header to buffer
    memcpy(header, response, header_size);
    header[header_size] = '\0';

    return header_size;
}

void httpmsg_handleResponse(char *response, struct parsed_url *url){

    //extract header
    char header[2048]; 
    int header_size = httpmsg_extractHeader(response, header);
    if(header_size <= 0){    //if header doesn't exist
        //Do nothing, just receive the message
        return;
    }

    struct httpmsg resp_msg;
    memset(&resp_msg, 0, sizeof(struct httpmsg));
    //printf("%s\n", header);

    //handle header
    httpmsg_handleHeaders(header, &resp_msg);
    if(resp_msg.flags & RFLAGS_CHANGED_LOC){
        //TODO:
        //change parsed url
        //inform callee to connect to new url
    }

    /*
    printf("%s\n%s\n%d\n%x\n", resp_msg.content_type, resp_msg.new_loc,
            resp_msg.body_length, resp_msg.flags);
    */

    FILE *f = fopen("response.dat", "w+");
    if(!(resp_msg.flags & RFLAGS_CHUNKED)){ //if not in chunked format
        char *offset = &response[header_size];
        fwrite(offset, sizeof(char), resp_msg.body_length, f);
    }
    httpmsg_free(&resp_msg);
}

//Gets the adress of a field start pointer and a field name. Stores the value in a buffer and
//returns the length of the value
int httpmsg_getFieldValue(char *offset, char *field_name, char *value_buf){
    char *field_value = &offset[strlen(field_name)];  //points to first character after field
    size_t field_value_len = field_strlen(field_value);   //chars between field_value and \r

    if(field_value_len > 0){
        memcpy(value_buf, field_value, field_value_len); 
    }
    value_buf[field_value_len] = '\0';
    return field_value_len;
}

void httpmsg_handleHeaders(char *header, struct httpmsg *httpmsg){
    //RESPONSE CODES
    char *respcodePtr = &header[9];     //expected after "HTTP/x.x "
    if(strstr(header, "301") == respcodePtr){
        httpmsg->flags |= RFLAGS_CHANGED_LOC;
    }
    if(strstr(header, "307") == respcodePtr){
        httpmsg->flags |= RFLAGS_CHANGED_LOC;
    }

    //FIELDS
    
    //TODO: add content type
    char *offset = NULL;

    if((offset = strstr(header, "Location: ")) != NULL){
        char value_buf[100];
        int field_value_len = httpmsg_getFieldValue(offset, "Location :", value_buf);
        if(field_value_len > 0){
            httpmsg->new_loc = malloc(field_value_len);
            memcpy(httpmsg->new_loc, value_buf, field_value_len); 
        }
    }

    if((offset = strstr(header, "Content-Length: ")) != NULL){
        char value_buf[100];
        int field_value_len = httpmsg_getFieldValue(offset, "Content-Length:", value_buf);
        if(field_value_len > 0){
            httpmsg->body_length = strtol(value_buf, NULL, 10);
        }
    }


    if(strstr(header, "Transfer-Encoding: chunked") != NULL){
        httpmsg->flags |= RFLAGS_CHUNKED;
    }
}

void httpmsg_free(struct httpmsg *msg){
    free(msg->content_type);
    free(msg->new_loc);
}

//returns the size of the string ending with \r\n
size_t field_strlen(char *field){
    int i = 0;
    for(; field[i] != '\r'; i++){}
    return i;
}

/*
//interpretation of body based on header
if(strstr(header, "Transfer-Encoding: chunked")){
    char *offset = &response[header_size];
    size_t len = strtol(offset, NULL, 16);  //reads string in base 16
    len = 1000;
    *body = (char*) malloc(len+1);
    memcpy(*body, offset, len);
    (*body)[len+1] = 0;

}
*/
