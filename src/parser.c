#include "parser.h"

void free_url(struct parsed_url *url){
    free(url->protocol);
    free(url->hostname);
    free(url->port);
    free(url->path);
    free(url->hash);
}

void parse_url(struct parsed_url *url, char *url_str){
    memset(url, 0, sizeof(struct parsed_url));
    int current_index = 0;
    int url_len = strlen(url_str); 

    //PROTOCOL
    char *prtc_end = strstr(&url_str[current_index], "://");    //protocol end
    if(prtc_end){
        //protocol = everything before
        int prtc_size = (uint64_t)(prtc_end - &url_str[current_index]);
        url->protocol = (char*) calloc(prtc_size+1, 1);
        memcpy(url->protocol, &url_str[current_index], prtc_size);
        current_index += prtc_size + 3; //protocol length + ://
        printf("Protocol: %s\n", url->protocol);
    }else{
        url->protocol = calloc(5, 1);
        strncpy(url->protocol, "http", 5);
    }


    //HOSTNAME
    //get hostname end
    char *hname_end = NULL;
    char *port_start = strstr(&url_str[current_index], ":");    
    char *path_start = strstr(&url_str[current_index], "/");   
    if(port_start){
        hname_end = port_start;
    }else if(path_start){
        hname_end = path_start;
    }else{
        hname_end = &url_str[url_len];
    }
    //NOTE: if a url can have a hash and no path, just add the case here

    int hname_size = (uint64_t)(hname_end - &url_str[current_index]);
    url->hostname= (char*) calloc(hname_size+1, 1);
    memcpy(url->hostname, &url_str[current_index], hname_size);
    current_index += hname_size + 1;    //hostname length + :
    printf("Hostname: %s\n", url->hostname);


    //PORT
    if(port_start){
        //get port after ':'
        char port_str[6] = {0}; //max port value is 65535
        int i = 0;
        for(; i < 6; i ++){
            if(url_str[current_index + i] == '/'){
                break;
            }
            port_str[i] = url_str[current_index + i];
        }
        if(i != 0){
            url->port = calloc(i+1, 1);
            memcpy(url->port, port_str, i+1);
        }
        current_index += i +1;
        printf("Port: %s\n", url->port);
    }else{
        url->port = calloc(3, 1);
        strncpy(url->port, "80", 3);
    }


    //PATH
    char *hash_start = strstr(&url_str[current_index], "#");
    char *path_end = hash_start;
    if(path_end == NULL){
        path_end = &url_str[url_len];
    }
    //NOTE: if implementing query string, check for '?' and handle it the same way
    if(path_start){
        current_index --;   //to get the '/'
        int path_size = (uint64_t)(path_end - &url_str[current_index]);
        url->path= (char*) calloc(path_size+1, 1);
        memcpy(url->path, &url_str[current_index], path_size);
        current_index += path_size + 1;    //hostname length + :
        printf("Path: %s\n", url->path);
    }else{
        url->port = calloc(2, 1);
        strncpy(url->port, "/", 2);
    }


    //HASH
    if(hash_start){
        int hash_size = (uint64_t)(&url_str[url_len] - &url_str[current_index]);
        url->hash= (char*) calloc(hash_size+1, 1);
        memcpy(url->hash, &url_str[current_index], hash_size);
        printf("hash: %s\n", url->hash);
    }
}

