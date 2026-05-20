# Sockets

Most interprocess communication is done through the *client - server* model
  - Client typically makes a connection to request information from the server  
  - Both sides send and recieve information
  - Client has knowledge of the server
  - Server may not have knowledege of the client communication
  - Both can send and recieve information
  - Involves basic construct of a *socket*

A *socket* is a singular end of a communication channel.
  - Client Side Setup
    - Create through `socket()` syscall
    - Connect through `connect()` syscall
    - Send and recieve data 
      - Ex. `write()`
      - Ex. `read()`
  - Server Side Setup
    - Create through `socket()` syscall
    - Bind to an address using `bind()` syscall
      - On internet; an address consists of port number on host
    - Listen for connections with `listen()` syscall
    - Accept connection with syscall


When creating a socket the program specifies the *address domain* and *socket type*. Sockets are required to have the same
type and be in the same domain in order to communicate.
    - Unix Domain
        - two processes share a common filesystem to communicate
        - character string acting as entry in filesystem 
    - Internet Domain
        - two processes on any two hosts communicate
        - internet address of every hostmachine (ip address)
        - each socket needs a port number on that host
            - *port number*: 16 bit unsigned integers


****

### Socket Types

- Stream
    - Communication as continuos stream of characters
    - TCP (*Transmission Control Protocol*)
        - reliable 
        - stream oriented
    - `SOCK_STREAM`
- Datagram
    - Read entire message at once
    - UDP (*Unix Datagram Protocol*)
        - unreliable
        - message oriented
    - `SOCK_DGRAM`

****

### Sockets Good To Know / Helpful

- `INADDR_ANY`: wildcard address for accepting any ip
- `htons(aka unsigned short)`: The htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks (the AF_INET or AF_INET6 address family).
- port & sin addr stored in network byte order 
    - this means the deleration order **matters**
    - See example program for usage guidelines

****

### Resources

- [Sockets Tutorial - Robert Ingalls](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
    - [Example Server](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c)
    - [Example Client](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/client.c)
- [Bind Manpage](https://www.man7.org/linux/man-pages/man2/bind.2.html)
- [Socket Manpage](https://man7.org/linux/man-pages/man2/socket.2.html)
- [Error Manpage](https://man7.org/linux/man-pages/man2/accept.2.html)

