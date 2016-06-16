#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Strings.h"
short extractString(char *source,char *sIdentifier,char *eIdentifier,char **dest){
  *dest = NULL;
  size_t skipOver = strlen(sIdentifier);

  char *pnter = strstr(source,sIdentifier);
  if(!pnter)
      return 1;
  pnter += skipOver;

  char *pnter_two = strstr(pnter,eIdentifier);
  if(!pnter_two)
    return 2;


  size_t dstSize = pnter_two - pnter;
  *dest = malloc(dstSize);
  if(!(*dest))
    return 3;

  memcpy(*dest,pnter,dstSize);
  *(*(dest) + dstSize - 1) = (char)0; //adds the null terminator
  return 0;
}
