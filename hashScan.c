#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Networking/Networking.h"
#include "Core/core.h"
#include "control/control.h"

#define VERSION "1.0.1"

int main(int argc , char **argv){
      if(argc == 1)
            printHelp();
      printf("hashScan Version %s\n",VERSION);
      if ( hashScan_init() != 0 )
            reportCritical("An Error Occured Starting The Scanner", 1);
      if( hashScan_md5_threatexpert(argv[1]) != 0 )
            reportCritical("An Error Occured While Hashing The File", 2);
      hashScan_cleanup();
      return 0;
}
