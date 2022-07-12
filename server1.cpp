#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <errno.h>
#define LENGTH 512
#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100
#define PLIKSERV    "/home/aatami/Documents/programowanie/serwer/plikserv.txt"
int main(int argc, char *argv[]) { /* licznik argumentow, tablica argumentow */

  int sd, rc,nsockfd;
  unsigned int n,cliLen;
  struct sockaddr_in cliAddr, servAddr;
  char msg[MAX_MSG];
   char  buf[512],sbuf[LENGTH];
  /* socket creation */
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd<0) {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }

  /* bind local server port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc<0) {
    printf("%s: cannot bind port number %d \n", 
       argv[0], LOCAL_SERVER_PORT);
    exit(1);
  }

  printf("%s: waiting for data on port UDP %u\n", 
       argv[0],LOCAL_SERVER_PORT);



  /* server infinite loop */
  while(1) {

    /* init buffer */
    memset(msg,0x0,MAX_MSG);


    /* receive message */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, msg, MAX_MSG, 0, 
         (struct sockaddr *) &cliAddr, &cliLen);

    if(n<0) {
      printf("%s: cannot receive data \n",argv[0]);
      continue;
    }

    /* print received message */
    printf("%s: from %s:UDP%u : %s \n", 
       argv[0],inet_ntoa(cliAddr.sin_addr),
       ntohs(cliAddr.sin_port),msg);

         unsigned int sin_size = sizeof(struct sockaddr_in);




        FILE *fr = fopen(PLIKSERV, "a");
        if(fr == NULL)
            printf("File %s Cannot be opened file on server.\n", PLIKSERV);
        else
        {
            bzero(sbuf, LENGTH); 
            int fr_block_sz = 0;
            while((fr_block_sz = recvfrom(sd, buf, LENGTH, 0,(struct sockaddr *) &cliAddr, &cliLen)) > 0) 
            {
                int write_sz = fwrite(buf, sizeof(char), fr_block_sz, fr);
                if(write_sz < fr_block_sz)
                {
                    perror("File write failed on server.\n");
                }
                bzero(buf, LENGTH);
                if (fr_block_sz == 0 || fr_block_sz != 512) 
                {
                    break;
                }
            }
            if(fr_block_sz < 0)
            {
                if (errno == EAGAIN)
                {
                    printf("recv() timed out.\n");
                }
                else
                {
                    fprintf(stderr, "recv() failed due to errno = %d\n", errno);
                    exit(1);
                    }
                }
            printf("Ok received from client!\n");
            fclose(fr); 
        }

  }/* end of server infinite loop */

return 0;

}

