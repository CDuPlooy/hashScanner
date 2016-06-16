#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "md5.h"

short md5File(char *filename,unsigned char *result){
      //result must be an array of size MD5_DIGEST_LENGTH
      short quick_return = 0;
      int fd = open(filename,O_RDONLY);
      if(fd == -1)
            return 1;

      MD5_CTX mdContext;
      MD5_Init(&mdContext);

      struct stat st;
      stat(filename, &st);

      off_t bytes = 0;
      char *buffer = malloc(250);
      if(!buffer)
            return 2;
      memset(buffer,0,250);

      while(bytes < st.st_size){
            off_t ret = read(fd, buffer, 250);

            if(ret == -1){
                  quick_return = 2;
                  goto md5FileCleanup;
            }
            if(ret == 0)
                  break;
            MD5_Update(&mdContext, buffer, ret);
            bytes+=ret;
            }
      MD5_Final(result, &mdContext);

      md5FileCleanup:
            free(buffer);
            close(fd);
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
