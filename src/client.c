#include <stdio.h>  // For IO operations
#include <stdlib.h> // Standard Library
#include <string.h> // String manipulation for string packets etc
#include <unistd.h> // Standard POSIX functions, like "close"

#include <sys/sendfile.h>
#include <sys/socket.h> // For socket programming
#include <sys/stat.h>
#include <sys/time.h> // For timeval struct
#include <sys/types.h>

#include <arpa/inet.h>   // Provides definitions for internet operations
#include <netinet/tcp.h> // Includes definitions for TCP

/* Constants */
int status;            // Used for error handling
const int SIZE = 1024; // in bytes
const char *LOCAL_HOST =
    "127.0.0.1"; // Standard address for IPv4 loopback traffic
// int const SIZE = 1024; // gives a weird error for some reason
int RECV_PORT = 5432;
char *PATH_SEND = "send.txt";

int main(int argc, char *argv[]) {

  /* Get args */
  // Check no. of args specified:
  if (argc != 2) { // 1+4 args are needed
    printf("Usage: %s <congestion_protocol>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  const char *TCP_CONG_TYPE = argv[1];

  /* Create a TCP socket */

  // Active open
  int sockfd = socket(AF_INET,     // IPv4
                      SOCK_STREAM, // Specifies TCP socket
                      IPPROTO_TCP  // TCP protocol, setting this to 0 also works
  );
  if (sockfd < 0) {
    perror("(Client) Error while creating the socket");
    exit(EXIT_FAILURE);
  }
  printf("(Client) Socket created successfully\n");

  // Set sock options to completely close the socket (addr + port), for
  // immediate reuse:
  int reuse_flag = 1;

  status = setsockopt(
      // The socket:
      sockfd,
      // The socket layer, more info:
      // https://stackoverflow.com/questions/21515946/what-is-sol-socket-used-for
      SOL_SOCKET,
      // This option allows your server to bind to an address which is in a
      // TIME_WAIT state
      SO_REUSEADDR,
      // Setting to true
      &reuse_flag, sizeof(int));
  if (status != 0) {
    perror("(Client) An error occured while setting the socket options");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  status =
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse_flag, sizeof(int));
  if (status != 0) {
    perror("(Client) An error occured while setting the socket options");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Set sock options to specify Reno or Cubic
  status = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, TCP_CONG_TYPE,
                      strlen(TCP_CONG_TYPE));
  if (status != 0) {
    perror("(Client) An error occured while setting the socket options");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Set the SO_LINGER sock option
  struct linger so_linger;
  so_linger.l_onoff = 1;
  so_linger.l_linger = 10;
  status =
      setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
  if (status != 0) {
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

  status =
      connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (status != 0) {
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

  // Using sendfile
  struct stat file_stat;
  // Load file description
  int fd = fileno(f);
  status = fstat(fd, &file_stat);
  if (status < 0) {
    perror("(Client) Error while getting file description");
    exit(EXIT_FAILURE);
  }
  printf("(Client) File Size: %ld bytes\n", file_stat.st_size);

  int bytes_sent = sendfile(sockfd, fd, 0, file_stat.st_size);
  if (bytes_sent < 0) {
    perror("(Client) Error while sending the file");
    exit(EXIT_FAILURE);
  } else {
    printf("(Client) %d bytes sent\n", bytes_sent);
  }

  printf("(Client) Successfully sent the file\n");
  fclose(f);

  status = shutdown(
      sockfd,
      SHUT_WR); // close(sockfd); // Reference:
                // https://stackoverflow.com/questions/25476348/c-sockets-incomplete-file-transfer
  if (status == 0)
    printf("(Client) Connection successfully shutdown\n");
  else
    perror("(Client) Error while closing the socket");

  return 0;
}
