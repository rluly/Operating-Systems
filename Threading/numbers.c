#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int loc = 1;
char **toks;
int args;
pthread_mutex_t lock;

void negative_check(void *ptr);
void zero_check(void *ptr);
void one_check(void *ptr);
void prime_check(void *ptr);
void even_check(void *ptr);
void odd_check(void *ptr);

int main(int argc, char *argv[]){
  pthread_t thread1,thread2,thread3,thread4,thread5,thread6;

  toks = (char**)malloc(argc * sizeof(char*));
  for(int ii = 0;ii<argc;ii++){
    toks[ii] = (char*)malloc(20*sizeof(char));
  }
  
  for(args = 0; args<argc; args++){
    strcpy(toks[args],argv[args]);
  }

  if(pthread_mutex_init(&lock,NULL) != 0){  
    printf("mutex failure\n");
    exit(1);
  }
  
  pthread_create(&thread1,NULL,(void *)negative_check,NULL);
  pthread_create(&thread2,NULL,(void *)zero_check,NULL);
  pthread_create(&thread3,NULL,(void *)one_check,NULL);
  pthread_create(&thread4,NULL,(void *)prime_check,NULL);
  pthread_create(&thread5,NULL,(void *)even_check,NULL);
  pthread_create(&thread6,NULL,(void *)odd_check,NULL);

  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);
  pthread_join(thread3,NULL);
  pthread_join(thread4,NULL);
  pthread_join(thread5,NULL);
  pthread_join(thread6,NULL);

  pthread_mutex_destroy(&lock);
  exit(0);
}

void negative_check(void *ptr){
  int current = 0;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if(current < 0){
      while(loc != ii){}
      printf("NEGATIVE: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
  }
}

void zero_check(void *ptr){
  int current = 0;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if(current == 0){
      while(loc != ii){}
      printf("ZERO: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
  }
}

void one_check(void *ptr){
  int current = 0;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if(current == 1){
      while(loc != ii){}
      printf("ONE: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
  }
}

void prime_check(void *ptr){
  int current = 0;
  int flag = 1;
  int flag2 = 1;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if((current == 0) || (current == 1) || (current < 0)){
      flag = 0;
    }
    for(int jj = 2;jj <= current/2;jj++){
      if(current % jj == 0){
	flag2 = 0;
	break;
      }
    }
    if(flag && flag2){
      while(loc != ii){}
      printf("PRIME: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
    flag = 1;
    flag2 = 1;
  }
}

void even_check(void *ptr){
  int current = 0;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if((current % 2 == 0) && (current != 2) && (current != 0) && (current > 0)){
      while(loc != ii){}
      printf("EVEN: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
  }
}

void odd_check(void *ptr){
  int current = 0;
  int flag = 1;
  int flag2 = 0;
  for(int ii = 1;ii<args;ii++){
    current = atoi(toks[ii]);
    if(current == 1 || current < 0){
      flag = 0;
    }
    for(int jj =2;jj < current/2;jj++){
      if(current % jj == 0){
	flag2 = 1;
      }
    }
    if((current %2 != 0) && flag && flag2){
      while(loc != ii){}
      printf("ODD: %d\n",current);
      pthread_mutex_lock(&lock);
      loc = ii + 1;
      pthread_mutex_unlock(&lock);
    }
    flag = 1;
  }
}
