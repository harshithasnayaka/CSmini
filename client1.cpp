#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */ 
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>  


#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100
#define PLIK    "/home/aatami/Documents/programowanie/serwer/plik.txt"
#define LENGTH 512
int main(int argc, char *argv[]) {

  int sd, rc, i,fd;
  unsigned int cliLen;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;
  char  buf[LENGTH];
  /* check command line args */
  if(argc<3) {
    printf("usage : %s <server> <data1> ... <dataN> \n", argv[0]);
    exit(1);
  }

  /* get server IP address (no check if input is IP address or DNS name */
  h = gethostbyname(argv[1]);
  if(h==NULL) {
    printf("%s: unknown host '%s' \n", argv[0], argv[1]);
    exit(1);
  }

  printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
     inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
     h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

  /* socket creation */
  sd = socket(AF_INET,SOCK_DGRAM,0);  /* port 0 - system gets the first free one */ 
  if(sd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }

  /* bind any port */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);

  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: cannot bind port\n", argv[0]);
    exit(1);
  }


  /* send data */
  for(i=2;i<argc;i++) {
    rc = sendto(sd, argv[i], strlen(argv[i])+1, 0, 
        (struct sockaddr *) &remoteServAddr, 
        sizeof(remoteServAddr));

    if(rc<0) {
      printf("%s: cannot send data %d \n",argv[0],i-1);
      close(sd);
      exit(1);
    }
    /* send text file */


        char sdbuf[LENGTH]; 
        printf("[Client] Sending %s to the Server... ", PLIK);
        FILE *fs = fopen(PLIK, "r");
        if(fs == NULL)
        {
            printf("ERROR: File %s not found.\n", PLIK);
            exit(1);
        }

        bzero(sdbuf, LENGTH); 
        int fs_block_sz; 
        while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
        {
            if(send(sd, sdbuf, fs_block_sz, 0) < 0)
            {
                fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", PLIK,           errno);
                break;
            }
            bzero(sdbuf, LENGTH);
        }
        printf("Ok File %s from Client was Sent!\n", PLIK);


  }

  return 1;

}

