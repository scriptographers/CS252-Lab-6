#include <time.h> // For setting seeds for random numbers
#include <stdio.h> // For IO operations
#include <stdlib.h> // Standard Library
#include <unistd.h> // Standard POSIX functions, like "close"
#include <string.h> // String manipulation for string packets etc
#include <sys/time.h> // For timeval struct
#include <sys/socket.h> // For socket programming
#include <arpa/inet.h> // Provides definitions for internet operations

/* Constants */
int status; // Used for error handling
const int SIZE = 1024; // In bytes
const char* LOCAL_HOST = "127.0.0.1"; // Standard address for IPv4 loopback traffic
// int const SIZE = 1024; // gives a weird error for some reason
int RECV_PORT = 5432;
char *PATH_RECV = "recv.txt";
const int MAX_PENDING = 5;

int main(){
    
    /* Create a TCP socket */

    // Passive open
    int sockfd = socket(
        AF_INET, // IPv4
        SOCK_STREAM, // Specifies TCP socket
        IPPROTO_TCP // TCP protocol, setting this to 0 also works
    );
    if (sockfd < 0){
        perror("(Server) Error while creating the socket");
        exit(EXIT_FAILURE);
    }
    printf("(Server) Socket created successfully\n");

    // Set sock options to completely close the socket, for immediate reuse:
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
        perror("(Server) An error occured while setting the socket options");
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
        perror("(Server) An error occured while setting the socket options");
        exit(EXIT_FAILURE);
    }

    /* Create the TCP Connection */

    // Create the struct for the server's address:
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(RECV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);

    // Passive open: Bind server to server's port:
    status = bind(
        sockfd, 
        (struct sockaddr*) &server_addr, 
        sizeof(server_addr)
    );
    if(status < 0) {
        perror("(Server) Error while binding");
        exit(EXIT_FAILURE);
    }
    printf("(Server) Bind successful\n");

    status = listen(sockfd, MAX_PENDING);
    if(status == 0)
        printf("(Server) Listening...\n");
    else{
        perror("(Server) Unable to listen");
        exit(EXIT_FAILURE);
    }

    /* File Transmisson */

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t len_caddr = sizeof(client_addr);
    int new_sockfd = accept(
        sockfd, 
        (struct sockaddr*) &client_addr, 
        &len_caddr
    );
    if (new_sockfd < 0){
        perror("(Server) Error while creating the new socket");
        exit(EXIT_FAILURE);
    }
    
    // Create the file where the contents will be saved
    FILE *f;
    f = fopen(PATH_RECV, "w+");

    char buffer[SIZE];
    bzero(buffer, sizeof(buffer));

    while (1){
        status = recv(new_sockfd, buffer, sizeof(buffer), 0);
        if (status == 0){
            printf("(Server) File contents saved successfully\n");
            break;
        }
        else if (status < 0){
            perror("(Server) Error while recieving file contents");
            exit(EXIT_FAILURE);
        }
        else
            fputs(buffer, f);
        bzero(buffer, sizeof(buffer));
    }

    fclose(f);

    int status1 = close(new_sockfd);
    int status2 = close(sockfd);

    if (status1 == 0 && status2 == 0)
        printf("(Server) Connection successfully closed\n");
    else
        perror("(Server) Error while closing the socket");

    printf("(Common) File transfer complete\n");

    return 0;

}