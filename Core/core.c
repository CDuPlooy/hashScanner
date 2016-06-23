#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <curl/curl.h>


#include "core.h"
#include "../Strings/Strings.h"
#include "../Networking/Networking.h"
#include "../md5/md5.h"

#define CHUNK_SIZE 250
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
            puts("\tmd5-info : [ Hash is not in threatexpert db. ]");
      }
      else{
            if( strstr(name,"ThreatExpert Report:") == NULL )
                  puts("\tmd5-info : [ Hash is not in threatexpert db. ]");
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

float hashScan_shannonEntropy(char *filename){
      float quick_return;

      size_t byteCount[256]; //representing bytes 0->254
       memset(byteCount,0,256 * sizeof(size_t));
      FILE *src = fopen(filename,"rb");
      if(!src)
            return 1;

      char *buffer = malloc(CHUNK_SIZE);
      if(!buffer)
            return 2;

      size_t ret;
      unsigned long long totalBytes = 0;
      while( (ret = fread(buffer, 1, CHUNK_SIZE, src)) != 0){
            size_t i;
            for(i =0 ; i < ret ; i++){
                  byteCount[(unsigned char)buffer[i]]++;
                  totalBytes++;
                  } //smarts were involved here.
      }
      if(ferror(src)){
            quick_return = 0;
            goto hashScan_shannonEntropy_bufferCleanup;
      }

      size_t i;
      float Entropy = 0;
      float prob_x = 0;
      for(i = 0 ; i < 256 ; i++){
            if(byteCount[i] == 0)
                  continue;
                  prob_x = (float)(byteCount[i])/totalBytes;
                  Entropy +=  prob_x * log2(prob_x);//prob(x) * log2(prob(x))
            }
      Entropy *= -1;
      quick_return = Entropy;

      hashScan_shannonEntropy_bufferCleanup:
            free(buffer);
            return quick_return;
}
