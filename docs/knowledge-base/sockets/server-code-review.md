# Server Code

```c
int sockfd, newsockfd, portno, clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;
if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
}
```

- `sockfd`: 
- `newsockfd`: 
- `portno`: 
- `clilen`: 
- `serv_addr`: server address
- `cli_addr`: client address

`serv_addr` and `cli_addr` are of type `sock_addr_in`.
This type is used to describe an Ipv4 internet domain socket address.
The implementation is specified through the manpages as:

```c
struct sockaddr_in {
    sa_family_t     sin_family;     /* AF_INET */
    in_port_t       sin_port;       /* Port number */
    struct in_addr  sin_addr;       /* IPv4 address */
};
```

- `sa_family_t` is an unsigned integer type, describing a sockets protocol family.
- `in_port_t` is equivalent to `uint16_t` (`<inttypes.h>`)
- `in_addr` is equivalent to `uint32_t` (`<inttypes.h>`)

****

```c
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) 
    error("ERROR opening socket");
 bzero((char *) &serv_addr, sizeof(serv_addr));
 portno = atoi(argv[1]);
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = INADDR_ANY;
 serv_addr.sin_port = htons(portno);
```

> [!NOTE]
> A *file descriptor* is a file specific identifier for file / other I/O resources (pipes & sockets).

The `socket` function 
- From `socket.h`
- creates an endpoint for communication
- returns a file descriptor referring to that endpoint 
    - on successful call, will be the lowest numbered file descriptor not currently open for the process
- Arguments
    - `domain`: `int`
    - `type`: `int`
    - `protocol` `int`
    - (the macros used in assignment correspond to int values)

The conditional parameter (`if (sockfd < 0)`) is checking to see if the socket was successfully initialized. 
The return value is system dependent. Use `INVALID_SOCKET` for comparison rather than the method used in the above example.

`bzero` is used to erase values stored in a set range of byte memory by setting each byte to `\0`.
- From `strings.h`
- has the possibility to be skipped by compiler optimizations
    - `explicit_bzero` to enforce usage
- No return value 
- Arguments
    - `n`: `size_t`
    - `s[n]`: `void`
    - `n`: `size_t`



****

### Resources

- [Sockets Tutorial - Robert Ingalls](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
- [Example Server](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c)
- [Example Client](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/client.c)
- [Sock Addr Man Page](https://www.man7.org/linux/man-pages/man3/sockaddr.3type.html)
- [SA Family Man Page](https://man.archlinux.org/man/sa_family_t.3type.en)
- [Netinet Manpage](https://man7.org/linux/man-pages/man0/netinet_in.h.0p.html)
- [Socket Manpage](https://man7.org/linux/man-pages/man2/socket.2.html)
- [File Descriptor]
