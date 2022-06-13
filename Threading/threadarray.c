#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void print_message_function(void *ptr);

typedef struct str_thdata
{
  int thread_no;
  char message[100];
} thdata;

int main(int argc, char *argv[]){
  /* pthread_t thread1, thread2; */
  /* thdata data1, data2; */

  int count = atoi(argv[1]);
  pthread_t thread[count];
  thdata data[count];
  /* printf("We need this many threads %d\n",count); */

  for(int ii = 0;ii<count;ii++){
    data[ii].thread_no = ii;
    strcpy(data[ii].message,"Hi!");
    pthread_create(&thread[ii],NULL,(void *) &print_message_function,(void *) &data[ii]);
    pthread_join(thread[ii],NULL);
  }

  /* data1.thread_no = 1; */
  /* strcpy(data1.message,"Hello!"); */

  /* data2.thread_no =2; */
  /* strcpy(data2.message,"Hi!"); */

  /* pthread_create(&thread1,NULL,(void *) &print_message_function,(void *) &data1); */
  /* pthread_create(&thread2,NULL,(void *) &print_message_function,(void *) &data2); */

  /* pthread_join(thread1,NULL); */
  /* pthread_join(thread2,NULL); */

  exit(0);
}

void print_message_function (void *ptr){
  thdata *data;
  data = (thdata *) ptr;
  printf("Thead %d says %s \n",data->thread_no,data->message);
  pthread_exit(0);
}
