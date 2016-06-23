#ifndef CORE_H
#define CORE_H
short hashScan_init();
void hashScan_cleanup();
void printHelp();
short hashScan_md5_threatexpert(char *filename);
short hashScan_md5_virustotal(char *filename);
#endif
