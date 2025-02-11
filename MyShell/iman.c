#include"headers.h"



void strip_html_tags(char *str) {
    char *p = str;
    char *q = str;
    while (*p) {
        if (*p == '<') {
            while (*p && *p != '>') {
                p++;
            }
            if (*p) {
                p++;
            }
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';
}

void iMan(char *command) {
    struct hostent *web = gethostbyname("man.he.net");
    if (web != NULL) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock != -1) {
            struct sockaddr_in web_address;
            struct in_addr **addr_list = (struct in_addr **)web->h_addr_list;
            web_address.sin_addr = *addr_list[0];
            web_address.sin_family = AF_INET;
            web_address.sin_port = htons(80);

            int ConnTn = connect(sock, (struct sockaddr *)&web_address, sizeof(web_address));
            if (ConnTn != -1) {
                char requested_content[20480];
                snprintf(requested_content, sizeof(requested_content), 
                         "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", 
                         command);
                int request = send(sock, requested_content, strlen(requested_content), 0);
                if (request != -1) {
                    char Req_Manual[40960];
                    int result = recv(sock, Req_Manual, sizeof(Req_Manual) - 1, 0);
                    if (result > 0) {
                        Req_Manual[result] = '\0';
                        
                        // Find the start of the body
                        char *body_start = strstr(Req_Manual, "\r\n\r\n");
                        if (body_start != NULL) {
                            body_start += 4; // Skip past the "\r\n\r\n"
                        } else {
                            // Handle cases where only "\n\n" is used as a delimiter
                            body_start = strstr(Req_Manual, "\n\n");
                            if (body_start != NULL) {
                                body_start += 2; // Skip past the "\n\n"
                            }
                        }

                        if (body_start != NULL) {
                            // Strip HTML tags
                            char text_body[40960];
                            strncpy(text_body, body_start, sizeof(text_body) - 1);
                            text_body[sizeof(text_body) - 1] = '\0';
                            strip_html_tags(text_body);
                            printf("%s\n", text_body);
                        } else {
                            printf("Can't find the body of the response.\n");
                            }
                        } 
                        else {
                            printf("Failed to receive manual page!!\n");
                        }
                        }                    
                        else {
                            printf("Failed to send request!!\n");
                        }
                        } 
                        else {
                            printf("Connection failed\n");
                        }
                        close(sock);
                        }
                        else {
                        printf("Failed to create socket\n");
                        }
                        } 
                        else {
                            printf("DNS lookup failed");
                        }
}


















