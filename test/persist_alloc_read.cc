
/*
 *  malloc-test
 *  cel - Thu Jan  7 15:49:16 EST 1999
 *
 *  Benchmark libc's malloc, and check how well it
 *  can handle malloc requests from multiple threads.
 *
 *  Syntax:
 *  malloc-test [ size [ iterations [ thread count ]]]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <nv_map.h>
#include <c_io.h>

#ifdef ENABLE_MPI_RANKS 
#include "mpi.h"
#endif

#define USECSPERSEC 1000000
#define pthread_attr_default NULL
#define MAX_THREADS 2
#define BASE_PROC_ID 20000

unsigned int procid;
void run_test(char a);
static unsigned size = 1048576; /* 1MB */
static int total_iterations = 10000;

int main(int argc, char *argv[])
{
  if(argc < 2) {
    fprintf(stdout, "enter read (r) or write(w) mode \n");
    exit(0);    
  }
     nvinit_(BASE_PROC_ID);

#ifdef ENABLE_MPI_RANKS  
  MPI_Init (&argc, &argv);  
#endif
  printf("Starting test...\n");
  if(!strcmp(argv[1], "w"))
    run_test('w');
  else
     run_test('r');

  exit(0);
}

void run_test(char r)
{
  register int i, isRead=0;
  register unsigned request_size = size;
  struct timeval start, end, null, elapsed, adjusted;
  register char *buf=NULL, *src=NULL;
  char varname[100];

  //struct timespec startpoll, endpoll;
  //startpoll.tv_sec = 0;
  //endpoll.tv_nsec = 1000L;


#ifdef ENABLE_MPI_RANKS
  int rank;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  fprintf(stderr,"rank %d \n",rank);
  procid = rank + 1;
#else
  procid = 1;
#endif

  src = (char *)malloc(size);
  assert(src);
  for(unsigned int j=0; j < size; j++)
	  src[j]= 'a' + (char)i;

  /*
   * Time a null loop.  We'll subtract this from the final
   * malloc loop results to get a more accurate value.
   */
  null.tv_sec = end.tv_sec - start.tv_sec;
  null.tv_usec = end.tv_usec - start.tv_usec;
  if (null.tv_usec < 0) {
    null.tv_sec--;
    null.tv_usec += USECSPERSEC;
  }
  /*
   * Run the real malloc test
   */
  gettimeofday(&start, NULL);

  for (i = 0; i < total_iterations; i++) {

    bzero(varname,0);
    sprintf(varname,"%d",i);
    strcat(varname,"_");
    strcat(varname,(char* )"buf");

    if(r == 'w') {
      buf = (char *)nvalloc_(size,varname,BASE_PROC_ID);
      assert(buf);
      memcpy(buf, src, size);
      //fprintf(stderr,"Created a new object %s count %d\n",
    	//	  (char *)varname, i);
    }else{
      isRead = 1;
      //sleep between each request
      //nanosleep(&startnano , &endnano);
      buf = (char *)nvread_(varname,BASE_PROC_ID);
      if(!buf)
    	  break;
      //fprintf(stdout,"varname %s %zu\n", varname, strlen(buf));
      memcpy(src, buf, strlen(buf));
    }
  }
  gettimeofday(&end, NULL);
  elapsed.tv_sec = end.tv_sec - start.tv_sec;
  elapsed.tv_usec = end.tv_usec - start.tv_usec;
  if (elapsed.tv_usec < 0) {
    elapsed.tv_sec--;
    elapsed.tv_usec += USECSPERSEC;
  }
  /*
   * Adjust elapsed time by null loop time
   */
  adjusted.tv_sec = elapsed.tv_sec - null.tv_sec;
  adjusted.tv_usec = elapsed.tv_usec - null.tv_usec;
  if (adjusted.tv_usec < 0) {
    adjusted.tv_sec--;
    adjusted.tv_usec += USECSPERSEC;
  }
  free(src);
  src = NULL;

  if(isRead) {
    printf("Read %d objects bytes %zu adjusted timing: %lu.%06lu "
       "seconds for %d \n", i, size*i,elapsed.tv_sec, elapsed.tv_usec);
  }else {
	printf("Wrote %d objects bytes %zu adjusted timing: %lu.%06lu "
	   "seconds for %d \n", i, size*i,elapsed.tv_sec, elapsed.tv_usec);
  }
  pthread_exit(NULL);
}

/**************************************************************************/
/*printf("Thread %lu adjusted timing: %lu.%06lu seconds for %d requests"
  " of %d bytes.\n", pthread_self(),
  elapsed.tv_sec, elapsed.tv_usec, total_iterations,
  request_size);*/

