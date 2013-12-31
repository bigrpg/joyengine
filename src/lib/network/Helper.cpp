#include "Helper.h"


//get millisecond
time_t getTime()
{
  struct timeval t;
  gettimeofday(&t,NULL);
  return (t.tv_sec*1000+t.tv_usec/1000);
}