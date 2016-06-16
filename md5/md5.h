/*
Some md5 functions.
Compile with -lCrypto

Connor Armand du Plooy
*/
#ifndef MD5_CUSTOM_H
#define MD5_CUSTOM_H
short md5File(char *filename,unsigned char *result);
void dumpMD5Hash(unsigned char *hash);
char *convertMD5toString(unsigned char *hash);
#endif
