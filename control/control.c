#include "control.h"
void reportCritical(char *msg,int lvl){
        printf("[ err ] %s\n",msg);
        if(lvl != 0)
                exit(lvl);
}

void report(char *msg){
        printf("[ info ] %s\n",msg);
        return;
}
