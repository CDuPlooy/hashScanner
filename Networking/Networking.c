#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "Networking.h"

short getAddrP(const char *Hostname,char *strIP,const int AI_FAM){
  char temp[30] = {0};
  struct addrinfo *res;   //contrary to what I thought , getaddrinfo returns a malloc addrinfo structure.
  struct addrinfo hints;

  memset(&hints,0,sizeof(struct addrinfo));//zero out hints


  //set hints
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_family = AI_FAM;
  hints.ai_flags = AI_PASSIVE;

  if( getaddrinfo(Hostname,NULL,&hints,&res) != 0)
      return 1;
  void *addr = NULL;

  struct sockaddr_in *ipv4;
  struct sockaddr_in6 *ipv6;

  if (AI_FAM == AF_INET){
        //ipv4
        ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
        }
  else if (AI_FAM == AF_INET6){
        //ipv6
        ipv6 = (struct sockaddr_in6 *)res->ai_addr; //typecast the addr into a sockaddr_in6 structure
        addr = &(ipv6->sin6_addr); //Save the void pointer to use as a one liner lately.
        }

  inet_ntop(AI_FAM,addr ,temp, 30);
  strcpy(strIP,(char *)(&temp));
  freeaddrinfo(res);
  return 0;
}


ssize_t send_allFixed(int fd , char *buffer , ssize_t size , int flags){
        ssize_t bytesSent = 0;
        while(bytesSent < size){
                ssize_t ret = send(fd,buffer,size-bytesSent,0);
                if(ret == -1)
                        return -1;
                bytesSent += ret;
        }
        return bytesSent;
}
ssize_t recv_allFixed(int fd , char *buffer , ssize_t size , int flags){
        ssize_t bytesReceived = 0;
        while(bytesReceived < size){
                ssize_t ret = recv(fd,buffer,size-bytesReceived,0);
                if(ret == -1)
                        return -1;
                bytesReceived += ret;
        }
        return bytesReceived;
}


size_t curl_outputToBuff( void *ptr, size_t size, size_t nmemb, struct String *out){
      //size*memb is data size
      const size_t newLength = out->len + size*nmemb;
      if(out->data == NULL){
            out->data=malloc(newLength);
            if(!out->data)
                  return -1;
      }
      else{
            out->data = realloc(out->data,newLength+1);
            if(!out->data)
                  return -2;
      }
      memcpy(out->data+out->len , ptr , size*nmemb);
      out->data[newLength-1] = (char)0;
      out->len = newLength;
      return size*nmemb; //standard
}
String *curl_get( char *resource , int *errNum){
      String *out = malloc(sizeof(String));
      if(!out)
            return NULL;
      out->data = NULL;
      out->len = 0;

      CURL *curl = curl_easy_init();
      curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, curl_outputToBuff);
      curl_easy_setopt(curl,CURLOPT_URL,resource);
      curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);

      curl_easy_setopt(curl,CURLOPT_WRITEDATA,out);
      CURLcode err = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      *errNum = err;
      return out;
}
