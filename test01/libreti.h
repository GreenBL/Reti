// Structures for handling internet addresses
struct sockaddr_in {
    short sin_family;               // e.g. AF_INET, AF_INET6
    unsigned short sin_port;        // e.g. htons(3490)
    struct in_addr sin_addr;        // see struct in_addr
};

struct in_addr {
    unsigned long s_addr;           // e.g. INADDR_ANY
};

// Structure for handling host names
struct hostent {
    char *h_name;                   // The real canonical host name.
    int h_addrtype;                 // The result’s address type, e.g. AF_INET
    int h_length;                   // The length of the addresses in bytes, which is 4 for IP (version 4) addresses.
    char *h_addr;                   // An IP address for this host.
};

// Function prototypes
int socket(int domain, int type, int protocol);  // Allocates a socket descriptor
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);  // Accepts an incoming connection on a listening socket
int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);  // Associates a socket with an IP address and port number
int listen(int sockfd, int queuelength);  // Tells a socket to listen for incoming connections
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);  // Connects - initiate a connection on a socket
int close(int sockfd);  // Closes a socket descriptor
struct hostent *gethostbyname(const char *name);  // Gets an IP address for a hostname
uint32_t htonl(uint32_t hostlong);  // Converts host byte order to network byte order for 32-bit integers
uint16_t htons(uint16_t hostshort);  // Converts host byte order to network byte order for 16-bit integers
uint32_t ntohl(uint32_t netlong);  // Converts network byte order to host byte order for 32-bit integers
uint16_t ntohs(uint16_t netshort);  // Converts network byte order to host byte order for 16-bit integers
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);  // Converts IP addresses to human-readable form
int inet_pton(int af, const char *src, void *dst);  // Converts IP addresses from human-readable form to binary form
int inet_aton(const char *cp, struct in_addr *inp);  // Converts IP addresses from the IPv4 numbers-and-dots notation into binary form
char *inet_ntoa(struct in_addr in);  // Converts IP addresses from binary form to the IPv4 numbers-and-dots notation
ssize_t recv(int sockfd, void *buf, size_t len, int flags);  // Receives data on a stream socket
ssize_t send(int sockfd, const void *buf, size_t len, int flags);  // Sends data out over a stream socket
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *srcaddr, socklen_t *addrlen);  // Receives data on a datagram socket
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *destaddr, socklen_t addrlen);  // Sends data out over a datagram socket
ssize_t read(int fd, void *buf, size_t count);  // Reads data from a stream socket
ssize_t write(int fd, const void *buf, size_t count);  // Writes data on a stream socket

// Utility Functions
void bzero(void *s, size_t n);  // Sets the first n bytes of the area starting at s to zero
void bcopy(const void *src, void *dest, size_t n);  // Copies n bytes from src to dest
void *memset(void *s, int c, size_t n);  // Fills the first n bytes of the memory area pointed to by s with the constant byte c
void *memcpy(void *dest, const void *src, size_t n);  // Copies n bytes from memory area src to memory area dest. The memory areas must not overlap
int strcmp(const char *s1, const char *s2);  // Compares the two strings s1 and s2
int strncmp(const char *s1, const char *s2, size_t n);  // Compares the first n bytes of s1 and s2
char *strcat(char *dest, const char *src);  // Appends the src string to the dest string
char *strncat(char *dest, const char *src, size_t n);  // Appends the src string to the dest string, using at most n bytes from src
char *strcpy(char *dest, const char *src);  // Copies the string pointed to by src, including the terminating null byte, to the buffer pointed to by dest
char *strncpy(char *dest, const char *src, size_t n);  // Copies the string pointed to by src, including the terminating null byte, to the buffer pointed to by dest, using at most n bytes from src
size_t strlen(const char *str);  // Calculates the length of the string str, excluding the terminating null byte
int rand(void);  // Returns a pseudo-random number in the range of 0 to RAND_MAX
