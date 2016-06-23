#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "md5.h"

#define CHUNK_SIZE 250

short md5File(char *filename,unsigned char *result){
      //result must be an array of size MD5_DIGEST_LENGTH
      short quick_return = 0;
      FILE *fd = fopen(filename,"rb");
      if(!fd)
            return 1;

      MD5_CTX mdContext;
      MD5_Init(&mdContext);

      char *buffer = malloc(CHUNK_SIZE);
      if(!buffer)
            return 2;
      quick_return = 0;
      size_t ret;

      while( (ret = fread(buffer, 1, CHUNK_SIZE, fd)) != 0){
            MD5_Update(&mdContext, buffer, ret);
      }
      MD5_Final(result, &mdContext);
      return quick_return;
}

void dumpMD5Hash(unsigned char *hash){
      for(size_t i = 0 ; i < MD5_DIGEST_LENGTH ; i++)
            printf("%02x",hash[i]);
      puts("");
      return;
}

char *convertMD5toString(unsigned char *hash){
      char *ret = malloc(MD5_DIGEST_LENGTH*2 +1);
      if(!ret)
            return NULL;

      size_t added = 0;
      for(size_t i = 0 ; i < MD5_DIGEST_LENGTH ; i++ , added += 2)
            snprintf(ret+added,MD5_DIGEST_LENGTH,"%02x",hash[i]);
      return ret;
}
