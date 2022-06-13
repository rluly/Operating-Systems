#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

/* Buffer vars */
int *buffer;
int *consumecount;
int BufferSize = 0;
/* int MaxItems = 0; */
int cycles = 0;
/* int front = -1; */
/* int rear = -1; */
int size = 0;

/* Stats Vars */
int fulltimes = 0;
int emptytimes = 0;
int nonprimes = 0;
int totalconsumed = 0;

/* Pthread and Semaphore vars */
/* sem_t empty; */
/* sem_t full; */
/* int in = 0; */
/* int out = 0; */
/* int buffer[BufferSize]; */
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

/* Linked List Structure */
struct node{
  int value;
  struct node *next;
};
struct node *head;
/* struct node *tail; */

void *producer(void *pno);
void *consumer(void *cno);
void *faulty_producer(void *fno);
void enQueue(int value);
void deQueue();
void display();
void printConsumeStats();
int primecheck(int value);

/* void printLinkedList(struct node *p); */

/* void printLinkedList(struct node *p){ */
/*   printf("["); */
/*   while(p != NULL){ */
/*     printf(" %d ",p->value); */
/*     p = p->next; */
/*   } */
/*   printf("]\n"); */
/* } */

int primecheck(int value){
  int current = value;
  int result = 0;
  int flag = 1;
  int flag2 = 1;
  if((current == 0) || (current == 1) || (current < 0)){
    flag = 0;
  }
  for(int j = 2;j <= current/2;j++){
    if(current % j ==0){
      flag2 = 0;
    }
  }
  if(flag && flag2){
    result = 1;
  }
  return result;
}

void enQueue(int value){
  if(size == BufferSize){
    /* printf("Queue is Full\n"); */
  }else{
    pthread_mutex_lock(&mutex2);
    size = size + 1;
    struct node *p = NULL;
    p = malloc(sizeof(struct node));
    p->value = value;
    p->next = NULL;
    if(size == 1){
      head = p;
    }else{
      struct node *temp = head;
      while(temp->next != NULL){
	temp = temp->next;
      }
      temp->next = p;
    }
    pthread_mutex_unlock(&mutex2);
  }
}

void deQueue(){
  if(size == 0){
    /* printf("Queue is Empty\n"); */
  }
  else{
    pthread_mutex_lock(&mutex2);
    size = size - 1;
    head = head->next;
    pthread_mutex_unlock(&mutex2);
  }
}

void display(){
  if(head == NULL){
    /* printf("Queue is Empty\n"); */
  }else{
    struct node *p = NULL;
    p = malloc(sizeof(struct node));
    p = head;
    printf("[");
    int sizecheck = size;
    while(sizecheck > 0){
      printf(" %d ",p->value);
      p = p->next;
      sizecheck --;
    }
    printf("]");
    free(p);
  }
}

void *producer(void *pno){
  int item;
  for(int i = 0; i < cycles; i++){
    int prime = 0;
    while(!prime){
      item = (rand() % (999999-2+1)+2);
      prime = primecheck(item);
    }
    /* sem_wait(&empty); */
    pthread_mutex_lock(&mutex);
    if(size<BufferSize){
      enQueue(item);
      printf("(PRODUCER %d writes %d/%d %d): (%d): ",*((int *)pno),(i+1),cycles,item,size);
      display();
      if(size==BufferSize){
	printf("*BUFFER NOW FULL*");
	fulltimes ++;
      }
      printf("\n");
    }else{
      printf("(PRODUCER %d %d/%d): Buffer is currently full. Nothing was produced.\n",*((int *)pno),(i+1),cycles);
    }
    pthread_mutex_unlock(&mutex);
    /* sem_post(&full); */
  }
}

void *consumer(void *cno){
  int consume = 1;
  int prime = 0;
  while(consume){
    /* sem_wait(&full); */
    pthread_mutex_lock(&mutex);
    int item;
    if(size>0){
      item = head->value;
      prime = primecheck(item);
      deQueue();
      printf("(CONSUMER %d reads    %d): (%d): ",*((int *)cno),item,size);
      consumecount[*((int *)cno)-1] ++;
      display();
      if(!prime){
	printf("*NOT PRIME* ");
	nonprimes ++;
      }
      if(size==0){
	printf("*BUFFER IS NOW EMPTY*");
	emptytimes ++;
      }
      printf("\n");
      totalconsumed ++;
      consume = 0;
    }else{
      /* printf("Buffer is empty, nothing was consumed\n"); */
    }
    pthread_mutex_unlock(&mutex);
  }
    /* sem_post(&empty); */
  /* } */
}

void *faulty_producer(void *fno){
  for(int i =0;i < cycles;i ++){
    int item;
    int prime = 1;
    while(prime){
      item = (rand() % (999999-4+1)+4);
      prime = primecheck(item);
    }
    /* sem_wait(&empty); */
    pthread_mutex_lock(&mutex);
    if(size < BufferSize){
      enQueue(item);
      printf("(PR*D*C*R %d writes %d/%d %d): (%d): ",*((int *)fno),(i+1),cycles,item,size);
      display();
      if(size==BufferSize){
	printf("*BUFFER IS NOW FULL*");
	fulltimes ++;
      }
      printf("\n");
    }else{
      printf("(PR*D*C*R %d %d/%d): Buffer is currently full. Nothing was produced.\n",*((int *)fno),(i+1),cycles);
    }
    pthread_mutex_unlock(&mutex);
    /* sem_post(&full); */
  }
}

void printConsumerStats(){
  printf("Total Number of Items Consumer: %d",totalconsumed);
}

int main(int argc, char *argv[]){
  /* printf("test\n"); */
  int timer = 0;
  clock_t before = clock();
  
  int n = 0;
  int l = 0;
  int p = 0;
  int f = 0;
  int c = 0;
  
  for(int i = 1;i < argc; i++){
    if(!strcmp(argv[i],"-n")){
      n = atoi(argv[i+1]);
      i++;
    }else if(!strcmp(argv[i],"-l")){
      l = atoi(argv[i+1]);
      i++;
    }else if(!strcmp(argv[i],"-p")){
      p = atoi(argv[i+1]);
      i++;
    }else if(!strcmp(argv[i],"-f")){
      f = atoi(argv[i+1]);
      i++;
    }else if(!strcmp(argv[i],"-c")){
      c = atoi(argv[i+1]);
      i++;
    }
  }

  /* printf("n = %d\n",n); */
  /* printf("l = %d\n",l); */
  /* printf("p = %d\n",p); */
  /* printf("f = %d\n",f); */
  /* printf("c = %d\n",c); */

  /* buffer = (int *)malloc(l * sizeof(int)); */
  consumecount = (int *)malloc(c * sizeof(int));
  BufferSize = l;
  /* MaxItems = l; */
  cycles = n;
  
  pthread_t pro[p],con[c],faulty[f];
  pthread_mutex_init(&mutex,NULL);
  pthread_mutex_init(&mutex2,NULL);
  
  /* sem_init(&empty,0,); */
  /* sem_init(&full,0,0); */

  int* thread_no_p;
  thread_no_p = (int *)malloc(p*sizeof(int));
  for(int i = 0; i <p;i++){
    thread_no_p[i] = i + 1;
  }
  /* printf("p: %d\n",thread_no_p[p-1]); */
  /* printf(" test: %d\n",thread_no_p[0]); */
  /* printf("How about here\n"); */
  
  int* thread_no_c;
  thread_no_c = (int *)malloc(c*sizeof(int));
  for(int i = 0; i < c;i++){
    /* printf("Do I get here?\n"); */
    thread_no_c[i] = i + 1;
  }

  /* printf("c: %d\n",thread_no_c[c-1]); */

  int* thread_no_f;
  thread_no_f = (int *)malloc(f*sizeof(int));
  for(int i = 0; i < f;i++){
    /* printf("Do I get here\n"); */
    thread_no_f[i] = i + 1;
  }

  /* printf("f: %d\n",thread_no_f[f-1]); */

  /* int num1,num2,num3 = 0; */

  for(int i=0; i < p; i++){
    pthread_create(&pro[i],NULL,producer,(void *)&thread_no_p[i]);
  }

  for(int i = 0;i < c; i++){
    pthread_create(&con[i],NULL,consumer,(void *)&thread_no_c[i]);
  }

  for(int i = 0;i < f;i++){
    pthread_create(&faulty[i],NULL,faulty_producer,(void *)&thread_no_f[i]);
  }

  for(int i = 0; i < p; i++){
    pthread_join(pro[i],NULL);
  }

  for(int i = 0;i < f;i ++){
    pthread_join(faulty[i],NULL);
  }

  for(int i = 0; i < c; i++){
    pthread_join(con[i],NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);
  /* sem_destroy(&empty); */
  /* sem_destroy(&full); */
  clock_t diff = clock() - before;
  timer = diff/1000;
  
  /* Print Section */
  printf("\n");
  printf("=======================================\n");
  printf("PRODUCER / CONSUMER SIMULATION COMPLETE\n");
  printf("=======================================\n");
  printf("Number of Items Per Producer Thread: %d\n",n);
  printf("Size of Buffer: %d\n",l);
  printf("Number of Producer Threads: %d\n",p);
  printf("Number of Faulty Producer Threads: %d\n",f);
  printf("Number of Consumer Threads: %d\n",c);
  printf("\n");
  printf("Number of Times Buffer Became Full %d\n",fulltimes);
  printf("Number of Times Buffer Became Empty %d\n",emptytimes);
  printf("\n");
  printf("Number of Non-Primes Detected %d\n",nonprimes);
  printf("Total Number of Items Consumed: %d\n",totalconsumed);
  for(int i = 0;i<c;i++){
    printf("Thread %d: %d\n",i,consumecount[i]);
  }
  printf("\n");
  printf("Total Simulation Time: .%d seconds\n",timer);
  
  return(0);
}
