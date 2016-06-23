#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Networking/Networking.h"
#include "Core/core.h"
#include "control/control.h"

#define VERSION "1.0.3"

int main(int argc , char **argv){
      if(argc == 1)
            printHelp();
      printf("hashScan Version %s\n",VERSION);
      if ( hashScan_init() != 0 )
            reportCritical("An Error Occured Starting The Scanner", 1);
      if( hashScan_md5_threatexpert(argv[1]) != 0 )
            reportCritical("An Error Occured While Hashing The File", 2);
      float entropy = hashScan_shannonEntropy(argv[1]);
      if(entropy != 0)
            printf("\tentropy : [ %.2f ]\n",entropy);
      else
            reportCritical("An Error Occured - Shannon!", 3);
      hashScan_cleanup();
      return 0;
}
