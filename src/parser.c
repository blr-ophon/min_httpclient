#include "parser.h"

int main(void){
    parse_url("http://www.example.com:80/res/page1.php?user=bob#account");
}

void parse_url(char *url){
    struct parsed_url parsed_url;
    int current_index = 0;
    int url_len = strlen(url); 

    //PROTOCOL
    char *prtc_end = strstr(&url[current_index], "://");    //protocol end
    if(prtc_end){
        //protocol = everything before
        int prtc_size = (uint64_t)(prtc_end - &url[current_index]);
        parsed_url.protocol = (char*) calloc(prtc_size+1, 1);
        memcpy(parsed_url.protocol, &url[current_index], prtc_size);
        current_index += prtc_size + 3; //protocol length + ://
        printf("Protocol: %s\n", parsed_url.protocol);
    }


    //HOSTNAME
    //get hostname end
    char *hname_end = NULL;
    char *port_start = strstr(&url[current_index], ":");    
    char *path_start = strstr(&url[current_index], "/");   
    if(port_start){
        hname_end = port_start;
    }else if(path_start){
        hname_end = path_start;
    }else{
        hname_end = &url[url_len];
    }
    //NOTE: if a url can have a hash and no path, just add the case here

    int hname_size = (uint64_t)(hname_end - &url[current_index]);
    parsed_url.hostname= (char*) calloc(hname_size+1, 1);
    memcpy(parsed_url.hostname, &url[current_index], hname_size);
    current_index += hname_size + 1;    //hostname length + :
    printf("Hostname: %s\n", parsed_url.hostname);


    //PORT
    if(port_start){
        //get port after ':'
        char port_str[6] = {0}; //max port value is 65535
        int i = 0;
        for(; i < 6; i ++){
            if(url[current_index + i] == '/'){
                break;
            }
            port_str[i] = url[current_index + i];
        }
        if(i != 0){
            parsed_url.port = calloc(i+1, 1);
            memcpy(parsed_url.port, port_str, i+1);
        }
        current_index += i +1;
        printf("Port: %s\n", parsed_url.port);
    }


    //PATH
}

