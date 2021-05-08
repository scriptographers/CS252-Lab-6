#include <stdio.h> // For IO operations
#include <stdlib.h> // Standard Library
#include <unistd.h> // Standard POSIX functions, like "close"
#include <string.h> // String manipulation for string packets etc
#include <sys/time.h> // For timeval struct
#include <sys/socket.h> // For socket programming
#include <arpa/inet.h> // Provides definitions for internet operations
#include <netinet/tcp.h> // Includes definitions for TCP

/* Constants */
int status; // Used for error handling
const int SIZE = 256; // in bytes
const char* LOCAL_HOST = "127.0.0.1"; // Standard address for IPv4 loopback traffic
// int const SIZE = 1024; // gives a weird error for some reason
int RECV_PORT = 5432;
char *PATH_SEND = "send.txt";
const char *TCP_CONG_TYPE = "cubic";

int main(){

    /* Create a TCP socket */

    // Active open
    int sockfd = socket(
        AF_INET, // IPv4
        SOCK_STREAM, // Specifies TCP socket
        IPPROTO_TCP // TCP protocol, setting this to 0 also works
    );
    if (sockfd < 0){
        perror("(Client) Error while creating the socket");
        exit(EXIT_FAILURE);
    }
    printf("(Client) Socket created successfully\n");

    // Set sock options to completely close the socket (addr + port), for immediate reuse:
    int reuse_flag = 1;

    status = setsockopt(
        // The socket:
        sockfd,
        // The socket layer, more info: https://stackoverflow.com/questions/21515946/what-is-sol-socket-used-for
        SOL_SOCKET,
        // This option allows your server to bind to an address which is in a TIME_WAIT state 
        SO_REUSEADDR,
        // Setting to true
        &reuse_flag,
        sizeof(int)
    );
    if (status != 0){
        perror("(Client) An error occured while setting the socket options");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    status = setsockopt(
        sockfd,
        SOL_SOCKET,
        SO_REUSEPORT,
        &reuse_flag,
        sizeof(int)
    );
    if (status != 0){
        perror("(Client) An error occured while setting the socket options");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set sock options to specify Reno or Cubic
    status = setsockopt(
        sockfd,
        IPPROTO_TCP,
        TCP_CONGESTION,
        TCP_CONG_TYPE,
        strlen(TCP_CONG_TYPE)
    );
    if (status != 0){
        perror("(Client) An error occured while setting the socket options");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    /* Create the TCP Connection */

    // Create the struct for the server's address:
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(RECV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    status = connect(
        sockfd, 
        (struct sockaddr*) &server_addr, 
        sizeof(server_addr)
    );
    if(status != 0){
        perror("(Client) Error in connecting to the server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("(Client) Connected to the server\n");


    /* File Transmisson */

    // Load in the file to send
    FILE *f;
    f = fopen(PATH_SEND, "r");
    if (f == NULL) {
        perror("(Client) Error while reading the file");
        exit(EXIT_FAILURE);
    }

    // Send file:
    char buffer[SIZE];
    bzero(buffer, sizeof(buffer));

    while(!feof(f)){

        fread(buffer, sizeof(buffer) - 1, 1, f);

        status = send(
            sockfd, 
            buffer,
            sizeof(buffer),
            0
        );

        if (status == -1){
            perror("(Client) Error while sending the file");
            exit(EXIT_FAILURE);
        }
        else{
            // printf("(Client) %d Bytes sent\n", status);
            // printf("(Client) %s\n", buffer);
        }

        bzero(buffer, sizeof(buffer)); // Erase the buffer

    }
    printf("(Client) Successfully sent the file\n");
    fclose(f);

    status = close(sockfd);
    if (status == 0)
        printf("(Client) Connection successfully closed\n");
    else
        perror("(Client) Error while closing the socket");

    return 0;

}