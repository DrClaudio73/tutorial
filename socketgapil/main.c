#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

int listen_sock;
int sock;

void gestore(int sig){
    printf("pressed Ctrl-C (signo %d). Closing and good bye;\r\n", sig);
    close(listen_sock);
    return;
}

int main(void){
    //signal (SIGINT, gestore); /* */
    const char * indirizzo_str = "192.168.1.101";
    in_addr_t indirizzo_inet;
    struct in_addr indirizzo_inet_strcut;
    int retu;


    retu = inet_aton(indirizzo_str, &indirizzo_inet_strcut);
    printf("%s: %d , %d -- (ret=%d)\r\n", indirizzo_str, indirizzo_inet_strcut.s_addr,htonl(indirizzo_inet_strcut.s_addr),retu);
    //unsigned long int ntohl(indirizzo_inet_strcut.s_addr);

    retu = inet_pton(AF_INET, indirizzo_str, &indirizzo_inet);
    //printf("%s: %d (ret=%d)\r\n", indirizzo_str, indirizzo_inet,retu);
    printf("%s: %d , %d -- (ret=%d)\r\n", indirizzo_str, indirizzo_inet,htonl(indirizzo_inet),retu);

    char indir_out[50];
    char dest[50];
    char *dest1;
    memset(dest,0,49);
    const int indirizzo_inet2 =indirizzo_inet;
    inet_ntop(AF_INET, &indirizzo_inet, dest, sizeof(dest));
    printf("%s: %d , %d -- (ret=)\r\n", dest, indirizzo_inet2,htonl(indirizzo_inet2));

    dest1=inet_ntoa(indirizzo_inet_strcut);
    printf("%s!\r\n", dest1);

    char rx_buffer[4198];
    char* addr_str;

    in_port_t PORT=32769;
    in_port_t PORT1;
    struct sockaddr_in local_addr;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    dest1=inet_ntoa(local_addr.sin_addr);
    printf("%s*\r\n", dest1);

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) {
        printf("Unable to create socket: errno %d\r\n", errno);
        return -1;
    }
    printf("Socket created %d\r\n",listen_sock);

    int err = bind(listen_sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (err != 0) {
        printf("Socket unable to bind: errno %d\r\n", errno);
        return -1;
    }

    printf("Socket %d bound, IP: %s, port %d\r\n", listen_sock, inet_ntoa(local_addr.sin_addr), ntohs(local_addr.sin_port));

    err = listen(listen_sock, 10);
    if (err != 0) {
        printf("Error occurred during listen: errno %d\r\n", errno);
        return -1;
    }
    printf("Socket listening\r\n");

    while(1) {
            struct sockaddr_in source_addr,test_addr; // Large enough for both IPv4 or IPv6
            uint addr_len = sizeof(source_addr);
            uint test_addr_len = sizeof(source_addr);

            sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);

            if (sock < 0) {
                printf("Unable to accept connection: errno %d\r\n", errno);
                break;
            }

            printf("Socket %d accepted (family: %d), remote IP: %s , remote_PORT: %d\r\n",sock,source_addr.sin_family, inet_ntoa(source_addr.sin_addr),ntohs(source_addr.sin_port));
            printf("====================================PRINTING SOCKET PAIRS DETAILS==================================\r\n");
            err=getsockname( sock, (struct sockaddr *)&test_addr, &test_addr_len);
            printf("socket %d (family: %d), err %d, local IP: %s, local PORT: %d, len: %d\r\n",sock,test_addr.sin_family, err,inet_ntoa(test_addr.sin_addr),ntohs(test_addr.sin_port),test_addr_len);
            err=getsockname( listen_sock, (struct sockaddr *)&test_addr, &test_addr_len);
            printf("listening socket %d (family: %d), err %d, local IP: %s, local PORT: %d, len: %d\r\n",listen_sock,test_addr.sin_family, err,inet_ntoa(test_addr.sin_addr),ntohs(test_addr.sin_port),test_addr_len);
            err=getpeername( sock, (struct sockaddr *)&test_addr, &test_addr_len);
            printf("socket %d (family: %d), err %d, remote IP: %s, remote PORT: %d, len: %d\r\n",sock,test_addr.sin_family, err,inet_ntoa(test_addr.sin_addr),ntohs(test_addr.sin_port),test_addr_len);
            err=getpeername( listen_sock, (struct sockaddr *)&test_addr, &test_addr_len);
            printf("listening socket %d (family: %d), err %d, remote IP: %s, remote PORT: %d, len: %d\r\n",listen_sock,test_addr.sin_family, err,inet_ntoa(test_addr.sin_addr),ntohs(test_addr.sin_port),test_addr_len);
            printf("====================================END SOCKET PAIRS DETAILS====================================\r\n");


            while(1){
                int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
                // Error occurred during receiving
                if (len < 0) {
                    printf("recv failed: errno %d\r\n", errno);
                    break;
                }
                // Connection closed
                else if (len == 0) {
                    printf("Connection closed\r\n");
                    break;
                }
                // Data received
                else {
                    // Get the sender's ip address as string
                    if (source_addr.sin_family == PF_INET) {
                        addr_str=inet_ntoa(source_addr.sin_addr);
                    } 

                    rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                    printf("\r\n\r\n************************************\r\nReceived %d bytes from %s: ***********************************\r\n\r\n", len, addr_str);
                    printf("%s\r\n", rx_buffer);

                    rx_buffer[len] = '%';
                    rx_buffer[len+1] = '&';
                    rx_buffer[len+2] = 0; // Null-terminate whatever we received and treat like a string

                    int err = send(sock, rx_buffer, len+2, 0);
                    if (err < 0) {
                        printf("Error occurred during sending: errno %d\r\n", errno);
                        break;
                    }
                }
            }  
            if (sock != -1) {
                printf("Shutting down socket and restarting...\r\n");
                shutdown(sock, SHUT_RDWR);
                //close(sock);
             }
        }

    return 0;
}