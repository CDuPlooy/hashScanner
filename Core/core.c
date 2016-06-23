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


//Functions go here
short hashScan_init(){
      return 0;
}

void hashScan_cleanup(){
}

void printHelp(){
      puts("hashScanner <file>");
      exit(0);
}
short hashScan_md5_threatexpert(char *filename){
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
