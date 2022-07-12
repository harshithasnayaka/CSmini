#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;
 
 void main()
 {

   ifstream myFile(filepath, ios::in|ios::binary|ios::ate);
   int size = (int)myFile.tellg();
   myFile.close();
   FILE * readFile =  fopen(filepath.data(),"rb");
   if (readFile == NULL)
   {
      cout<<"Unable to open File";
      return;
   }
   cout<<"\nNumber of Bytes :"<<size<<endl;
   // send the FileSize to the server
   char FileSize[50];
   memset(FileSize,0,sizeof(FileSize));
   itoa(size, FileSize, 10);
   int fileSizeLength = strlen(FileSize);
   FileSize[fileSizeLength] = '\0';
   send(sockfd,FileSize,fileSizeLength,0);
   char buffer[1050];
   int bytesReceived = 0;
   while(size > 0)
   {
      bytesReceived = 0;
      memset(buffer,0,sizeof(buffer));
        if(size>=1048)
        {
            fread(buffer, 1048, 1, readFile);
            bytesReceived = send( sockfd, buffer, 1048, 0 );
        }
        else
        {
            fread(buffer, size, 1, readFile);
            buffer[size]='\0';
            bytesReceived = send( sockfd, buffer, size, 0 );
        }
      size -= 1048;
   }
   char recev[10];
   memset(recev,0,sizeof(recev));
   recv(sockfd,recev,strlen("END"),0);
   if(strcmp(recev,"END")== 0)
   {
      cout<<"END received"<<endl;
   }
   shutdown(sockfd,2);
   fclose(readFile);