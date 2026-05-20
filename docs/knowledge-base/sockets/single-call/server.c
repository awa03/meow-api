// standard libraries
// used for printing and error handling
#include <stdio.h>
#include <stdlib.h>

// for reading from file descriptor
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h>

/*
Using:
 - socket: function -> int
 - AF_INET: int (2)
 - SOCK_STREAM: int (1)
 - listen
 - bind
*/
#include <sys/socket.h>

// ----------------------------------------------- // 

/*
Using:
  - sockaddr_in: Struct
  - INADDR_ANY: uint(0x00000000 | 0)
*/
#include <netinet/in.h>

// ----------------------------------------------- // 

/*
Using:
  - EADDRINUSE
  - EBADF
  - ENOTSOCK
  - EOPNOTSUPP
*/
#include <asm-generic/errno.h>

void printerr(const char* errmsg){
  printf("ERROR | %s\n", errmsg);
  exit(1);
}

void printwarn(const char* errmsg){
  printf("WARNING | %s\n", errmsg);
}

// Not a full error function
// look at https://man7.org/linux/man-pages/man2/accept.2.html
void parse_sock_err(int listening){
  printf("parsing errors");
  if (listening == EADDRINUSE){
    printerr("Port already in use!");
  }
  else if(listening == EBADF){
    printerr("Bad file descriptor");
  }
  else if(listening == ENOTSOCK){
    printerr("Socket number is invalid");
  }
  else if(listening == EOPNOTSUPP){
    printerr("Socket number not supported");
  }
}

int main(){

  const unsigned short PORT_NUMBR = 8080;
  const unsigned int ATTEMPTS_UNTIL_REFUSE = 5;
  const unsigned int SOCK_MIN = 0;
  const unsigned int BUFF_SIZE = 256;

  char buff[BUFF_SIZE];
  bzero(buff, sizeof(buff));

  // return value of socket - this will be the file descriptor
  int endpoint_fd; 

  // communication domain
  // AF_INET = IPV4 domain ( 2 )
  int domain = AF_INET;

  // defines communication semantics
  // sockstream: sequenced, reliable, bytestream, 
  // see https://man7.org/linux/man-pages/man2/socket.2.html 
  // for more information on stream specifications - bitwise or modifications 
  // must be in a connected state before data can be transmitted
  // use SOCK_SEQPACKET to request a specific sized packet
  //  - Can you chunk like this? I would assume use datagram / packets for chunking 
  //    - ANS: << NO REMAINING DATA IS DISCARDED >> 
  int type = SOCK_STREAM;

  // using 0 for no specification
  // see https://man7.org/linux/man-pages/man2/socket.2.html
  int protocol = 0;

  // int socket(int domain, int type, int protocol);
  // get the file descriptor for the socket and store it in the endpoint variable (int)

  endpoint_fd = socket(AF_INET, SOCK_STREAM, 0);

  // INFO ---------------
  // printf("Domain: %d\n", domain);
  // printf("Type: %d\n", type);
  // printf("File descriptor for endpoint: %d\n", endpoint_fd);

  // sys dependent return value
  // -1 typically indicate failure- 
  //    negative will not be valid file descriptor (ever) so we use this for our validation
  // no file descriptors available ? 
  if(endpoint_fd < SOCK_MIN) {
    printerr("Error creating socket"); 
  }

  // used to store addresses
  struct sockaddr_in accpt_addr, in_addr;

  // casting to string so the function accepts it - not modifying structure
  // then zeroing values to prevent improper initialization
  bzero((char*) &accpt_addr, sizeof(accpt_addr));

  // size padding
  //  - used for casting?
  //  -------------------
  // addr.sin_zero

  // primarily used for interpreting the rest of the data
  // does not need to be the same as domain (even though it is in this case)
  accpt_addr.sin_family = AF_INET;

  // sin_addr = ip address
  // port & sin addr stored in network byte order (big edian) to ensure reliablity across various systems
  // IPv4 wildcard address -- symbolic const uint
  // Use say who is accepted
  // Can I use this to restrict IP connections?
  //  - Even if.. it maybe better to remove during a validation check 
  //  Something to consider during implementation, since programatic blocking will offer more flexibility
  accpt_addr.sin_addr.s_addr = INADDR_ANY;

  // port for identifying
  accpt_addr.sin_port = htons(PORT_NUMBR);

  // When a socket is created it has a namespace (ipv4, ipv6, etc) however, it does not have an address
  // when bind is called the address specified in addr will attempt to be bound to the socket
  // This is known as "assigning a name to a socket"
  //    sockfd: this is going to be our file descriptor number; endpoint
  //    addr: this is the address we are attempting to bind
  //    addrlen: this is the size of the address struct | should typically be sizeof(addr)
  //    https://man7.org/linux/man-pages/man2/bind.2.html
  int bound_sock = bind(endpoint_fd, (struct sockaddr *) &accpt_addr, sizeof(accpt_addr));  

  // if the socket does not bind to the specified address then the return value will be less than 0
  parse_sock_err(bound_sock);

  // listen on bound socket - goes until N requests then refuse
  // used to accept incomming connection until state reached
  // marks as passive socket
  // https://www.man7.org/linux/man-pages/man2/listen.2.html
  int listening = listen(endpoint_fd, ATTEMPTS_UNTIL_REFUSE);

  // accept will take the current socket - grab the latest connection request
  // it will then create a new socket binding - returning the file descriptor to that newly connected socket
  // so that is why we dont need a loop here. The accept will return a new instance not overwrite the original. 
  // Still read from the original to query new socket requests
  // on a queue, so the socket will go through the queue poping with each accept
  // if addr len is addr is null len should be null too! not 0
  // https://man7.org/linux/man-pages/man2/accept.2.html
  const unsigned int in_addr_len = sizeof(in_addr);
  int accepted_fd = accept(endpoint_fd, (struct sockaddr*) &in_addr, (socklen_t*)&in_addr_len);
   
  /*
   Error Handling: https://man7.org/linux/man-pages/man2/accept.2.html 
   
   Linux accept() (and accept4()) passes already-pending network
   errors on the new socket as an error code from accept().  This
   behavior differs from other BSD socket implementations.  For
   reliable operation the application should detect the network
   errors defined for the protocol after accept() and treat them like
   EAGAIN by retrying.  In the case of TCP/IP, these are ENETDOWN,
   EPROTO, ENOPROTOOPT, EHOSTDOWN, ENONET, EHOSTUNREACH, EOPNOTSUPP,
   and ENETUNREACH.
  */

  // read up to N bytes on a file descriptor - limited by buffer size,
  // I wonder how we can emulate unlimited read, what happens to the rest of the data
  // Returns the number of bytes read, can use to set expectations
  // int fd, void buf[count], size_t count
  // https://man7.org/linux/man-pages/man2/read.2.html
  int read_bytes = read(accepted_fd, buff, BUFF_SIZE - 1); 

  if(read_bytes < 0){
    printerr("Failed to read Buffer");
  }

  printf("MSG: %s\n", buff);
  bzero(buff, BUFF_SIZE);

  char resp[] = "This is your message slime\0";
  int write_bytes = write(accepted_fd, resp, strlen(resp)+1);

  if(write_bytes < 0){
    printerr("Failed to read Buffer");
  }

  printf("Read Bytes: %d\n", read_bytes);
  printf("Write Bytes: %d\n", write_bytes);

  return 0;
}

/*
 * References:
 * https://man7.org/linux/man-pages/man2/socket.2.html
 * https://man7.org/linux/man-pages/man7/address_families.7.html
 * https://www.wikiwand.com/en/Datagram
 * https://man7.org/linux/man-pages/man2/bind.2.html
 * https://www.man7.org/linux/man-pages/man2/listen.2.html
 * https://man7.org/linux/man-pages/man2/accept.2.html
 * https://man7.org/linux/man-pages/man2/read.2.html
 * */
