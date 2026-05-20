#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>

void printerr(const char* errmsg){
  printf("ERROR | %s\n", errmsg);
  exit(1);
}

void printwarn(const char* errmsg){
  printf("WARNING | %s\n", errmsg);
}

int main(){
  const unsigned short PORT_NUMBR = 8080;
  // const unsigned int ATTEMPTS_UNTIL_REFUSE = 5;
  const unsigned int SOCK_MIN = 0;
  const unsigned int BUFF_SIZE = 256;

  /* Hostent members
   * char* h_name: is the official hostname of the connection server 
   * char** h_alias: is an array of pointers to hostname alias's end in nullptr
   * int h_addrtype: Address type
   * int h_length: The length in bytes 
   * */
  struct hostent* server;

  // AF_INET = ipv4
  // SOCK_STREAM = sequenced communication
  // 0 = No protocol used
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_fd < SOCK_MIN) {
    printerr("Invalid file descriptor while creating socket");
  } 

  // #include <netdb.h>
  // https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html
  // 
  server = gethostbyname("localhost");
  printf("Official Name: %s", server->h_name);

  for(char** tmp = server->h_aliases; *tmp != NULL; ++tmp ) {
    printf("%s", *tmp);
  }

  return 0;
}

/* References
 * - https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html
 *
 *
 * */
