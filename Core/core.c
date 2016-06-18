#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <curl/curl.h>


#include "core.h"
#include "../Strings/Strings.h"
#include "../Networking/Networking.h"
#include "../md5/md5.h"


#define HTTP_HEAD_MAX_SIZE 250
#define HOST_THREAT_EXPERT "threatexpert.com"
#define HEAD_THREAT_EXPERT "Connection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.84 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: en-US,en;q=0.8\r\nCookie: __utmz=118387404.1465835483.2.2.utmccn=(referral)|utmcsr=google.co.za|utmcct=/|utmcmd=referral; ASP.NET_SessionId=hemm1y45wladf1yfz2akgh45; __utma=118387404.432821593.1465827972.1465841289.1465843522.4; __utmb=118387404; __utmc=118387404\r\n\r\n"
//Global variables go here
char *threatExpert;

//Functions go here
short hashScan_init(){
      threatExpert = malloc(100);
      if(!threatExpert)
            return 1;
      if( getAddrP(HOST_THREAT_EXPERT,threatExpert,AF_INET) != 0)
            return 1;
      return 0;
}

void hashScan_cleanup(){
      free(threatExpert);
}

void printHelp(){
      puts("hashScanner <file>");
      exit(0);
}

short hashScan_md5(char *filename){
      short quick_return = 0;
      struct stat st;
      if(stat(filename,&st) != 0)
            return 1;

      unsigned char *hash = malloc(250);
      quick_return = md5File(filename, hash);
      if(quick_return != 0){
            free(hash);
            return -1;
      }

      char *sHash = convertMD5toString(hash);
      if(sHash == NULL){
            free(hash);
            return -2;
      }

      printf("\tmd5 Hash: [ %s ]\n",sHash);

      int retVal = 0;
      char *resource = malloc(1000);
      if(!resource){
            free(sHash);
            free(hash);
            return -2;
      }
      strcpy(resource,"http://www.threatexpert.com/report.aspx?md5=");
      strcat(resource,sHash);
      String *page = curl_get(resource,&retVal);
      if(!page->data){
            free(sHash);
            free(hash);
            return -3;
            }
      char *name;

      extractString(page->data , "<title>","</title>", &name);
      if(name == NULL){
            puts("Hash is not in threatexpert db.");
      }
      else{
            if( strstr(name,"ThreatExpert Report:") == NULL )
                  puts("Hash is not in threatexpert db.");
            else
                  printf("\t[%s]\n",name);
      }

      free(name);
      free(resource);
      free(hash);
      free(sHash);
      free(page->data);
      free(page);
      return 0;
}
