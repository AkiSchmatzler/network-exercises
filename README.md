# Network exercises  

This is a repository containing a couple of things we did in the "Algorithme des Reseaux" class, mainly manipulating sockets using the C language socket API. 

*tp == "traveaux pratiques" == practical exercises (on computers)*

## tp1  

Here we had to write a program that would send a simple udp message through a socket (client), another one that would receive it (server), the same for tcp, and then write a program that would be able to send and receive files through a udp connection (client and server as well). All connections use IPv4.  

### Syntax  
*use 127.0.0.1 as IP address if you're working on one computer (loopback address)*  
**compilation for all files:**
`make <filename without .c extension>` to compile (or `gcc <filename> -o <executable name>` if you prefer).  

**for udp and tcp message:**  

`./<executable name> <num_port>` for the server.  
`./<executable name> <adress IP dist> <num port_dist> <message>` for the client.  

**for udp file transfer:**  
`./<executable name> <port_number> <output_file>` for the server.  
`./<executable name> <remote address> <remote port> <file to send>` for the client.  
