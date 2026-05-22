#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

void printerr(const char* errmsg){
  printf("ERROR | %s\n", errmsg);
  exit(EXIT_FAILURE);
}

void printwarn(const char* errmsg){
  printf("WARNING | %s\n", errmsg);
}

void clearsock(struct sockaddr_in* sock){
  bzero((char*) sock, sizeof(*sock));
}

int main(){
  const char* SERVER_ADDR = "localhost";
  const unsigned short PORT_NUM = 8080; 
  const unsigned int BUFF_SIZE = 256;
  const unsigned int ATTEMPTS_UNTIL_REFUSE = 10;

  char buff[BUFF_SIZE];
  bzero(buff, BUFF_SIZE);

  // init socket - returns file descriptor
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0){
    printerr("while creating socket, invalid file descriptor");
  }

  struct sockaddr_in server_sock, in_sock;
  clearsock(&server_sock);
  clearsock(&in_sock);

  // must be declared to seperate generalizations
  // https://stackoverflow.com/questions/57779761/why-does-the-sin-family-member-exist
  server_sock.sin_family = AF_INET;

  // wild card accepting
  server_sock.sin_addr.s_addr = INADDR_ANY;

  // see https://www.sciencedirect.com/topics/computer-science/network-byte-order
  // enforce common byte ordering standard across systems for network
  // https://medium.com/@tom_84912/unraveling-the-endianness-mess-86b7b0ef56a6
  server_sock.sin_port = htons(PORT_NUM);

  // boolean enabling 
  int sock_opt = 1;
  socklen_t sock_opt_sz = sizeof(sock_opt);

  // default is SOL_SOCK; apply to socket rather than a lower level function
  // opt val specifying if enabled or not
  // this pointer will allow us to change the setting value later
  setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &sock_opt, sock_opt_sz);

  // cast to generic type and bind
  int boundfd = bind(sockfd, (struct sockaddr*) &server_sock, sizeof(server_sock));

  if(boundfd < 0){
    printerr("while binding socket, invalid file descriptor");
  }

  int listening = listen(sockfd, ATTEMPTS_UNTIL_REFUSE);

  int flag = 0;
  
  // "game loop"
  while(1){
    const unsigned int in_sock_sz = sizeof(in_sock);
    int in_sockfd = accept(sockfd, (struct sockaddr *) &in_sock, (socklen_t*) &in_sock_sz);

    if(in_sockfd < 0){
      printerr("invalid file descriptor during accept function call");
    }


    char* resp_one = "Whats good slimerson\0";
    char* resp_two = "Woah how'd the result change\0";

    int write_bytes;
    if(flag){
      write_bytes = write(in_sockfd, resp_one, strlen(resp_one));
    }
    else {
      write_bytes = write(in_sockfd, resp_two, strlen(resp_two));
    }

    if(write_bytes < 0){
      printerr("while writing to outbound file descriptor");
    }

    flag++;
    flag = flag % 2;

    close(in_sockfd);
  }

  return EXIT_SUCCESS;
}
