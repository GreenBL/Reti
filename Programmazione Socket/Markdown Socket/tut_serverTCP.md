# Server TCP

## Inizializzazione

Per creare un Server TCP dobbiamo innanzitutto importare le librerie per programmare con le socket.

```c
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // per lavorare con le stringhe
#include <time.h> // time() e ctime()
#include <sys/types.h> // tipi di dati di sistema
#include <sys/socket.h> // definizioni per socket()
#include <netinet/in.h> // struct sockaddr_in
```
Dopo aver incluso nel preprocessore le librerie necessarie per la programmazione socket, creiamo una funzione per rappresentare un generico errore:

```c
void error(char *msg) {
    perror(msg);
    exit(1);
}
```
Questa funzione chiama *perror()* e *exit()*
+  perror(char  message) stampa una descrizione per l'ultimo errore;
+ exit(int return_code) esce dal programma con un valore di ritorno passato per argomento.

Adesso possiamo addentrarci nella funzione main del server TCP.

## Main
```c
int main(int argc, char *argv[]) {
    int sockfd;
    int newsockfd;
    int portno;
    int clilen;
    char buffer[256];
    int counter;
    int n;
    pid_t pid;
    int control_bool;
```
Iniziamo dichiarando:
+ sockfd = socket file descriptor
+ newsockfd = socket file descriptor del client
+ portno = numero di porta
+ clilen = dimensione della socket structure del client
+ buffer = buffer di memoria riservata

continuiamo dichiarando la socket del server e la socket del client:
```c
    struct sockaddr_in serv_addr;
    struct sockaddr_in serv_addr;
```
Dato che prendiamo il numero di porta dal terminale eseguiamo un controllo preventivo:
```c
    if(argc < 2) {
        fprintf(stderr, "ERROR: No port provided!\n");
        exit(1);
    }
```
Usiamo fprintf(FILE *output, char *message, ...) per stampare allo standard error (terminale) l'errore!

Adesso creiamo la socket:

```c
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error("ERROR: Could not open socket/ Invalid socket file descriptor");
    }
```
Le costanti:
+ **<text style=color:rgb(103,151,205)> AF_INET </text>** indica il protocollo IPv4;
+ **<text style=color:rgb(103,151,205)> SOCK_STREAM </text>** indica una socket TCP;
+ **<text style=color:rgb(103,151,205)> 0 </text>** con SOCK_STREAM è TCP.

A questo punto abbiamo il sockfd della socket, manca da fare qualche inizializzazione della server socket, porta e del buffer:

```c
    bzero((char *) &serv_addr, sizeof(serv_addr));
```
*bzero()* inizializza a zero il blocco di memoria della socket.

```c
    portno = atoi(argv[1]);
```
Salviamoci il numero di porta scelto dall'utente nella variabile **portno**.

Adesso modifichiamo i membri della struct della socket:
```c
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
```
La funzione htons(numerodiporta) converte l'intero del numero di porta in un formato comprendibile al sistema operativo.

Adesso associamo la socket all'indirizzo del server:

```c
    control_bool = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(control_bool < 0) {
        error("ERROR: Binding issue!");
    }
```
Adesso possiamo fare entrare il server TCP in stato di listen:

```c
    listen(sockfd, )
```






