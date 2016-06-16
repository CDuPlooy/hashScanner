#ifndef NETWORKING_H
#define NETWORKING_H
typedef struct String{
      char *data;
      size_t len;
}String;

short getAddrP(const char *Hostname,char *strIP,const int AI_FAM);
ssize_t recv_allFixed(int fd , char *buffer , ssize_t size , int flags);
ssize_t send_allFixed(int fd , char *buffer , ssize_t size , int flags);
size_t curl_outputToBuff( void *ptr, size_t size, size_t nmemb, struct String *out);
String *curl_get( char *resource , int *errNum);
#endif
