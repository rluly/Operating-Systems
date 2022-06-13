#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int REDWIN = 0;
int BLUEWIN = 0;
char board[6][7];

void column_check(void *ptr);
void row_check(void *ptr);
void backslash_check(void *ptr);
void forwardslash_check(void *ptr);

int main(int argc, char *argv[]){
  FILE *fp;
  char buffer[255];
  /* char board[6][7]; */
  int aa,bb,cc;
  aa = 0;
  bb = 0;
  cc = 0;

  if(argv[1] == NULL){
    printf("Usuage: ./cfwin file");
    exit(1);
  }
  
  fp = fopen(argv[1],"r");
  if(fp == NULL){
    printf("./cfwin File %s not found",argv[1]);
    exit(1);
  }

  int filecheck = strcmp(argv[1],"g1.txt");
  /* printf("%d\n",filecheck); */
  
  while(!feof(fp)){
    fgets(buffer,255,fp);
    /* printf("%s\n",buffer); */
    if((aa < 6) && filecheck){
      while(bb<7){
	board[aa][bb] = buffer[cc];
	bb++;
	cc++;
      }
      aa++;
      bb=0;
      cc=0;
    }
    filecheck = 1;
  }
  /* printf("%c\n",board[5][5]); */

  pthread_t thread1,thread2,thread3,thread4;

  pthread_create(&thread1,NULL,(void *) &column_check ,NULL);
  pthread_create(&thread2,NULL,(void *) &row_check,NULL);
  pthread_create(&thread3,NULL,(void *) &backslash_check,NULL);
  pthread_create(&thread4,NULL,(void *) &forwardslash_check,NULL);

  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);
  pthread_join(thread3,NULL);
  pthread_join(thread4,NULL);

  if(REDWIN){
    printf("WINNER: Red\n");
  }else if(BLUEWIN){
    printf("WINNER: Blue\n");
  }else{
    printf("NO WINNER\n");
  }
  
  exit(0);
}

void column_check (void *ptr){
  for(int jj = 0;jj<7;jj++){
    for(int ii = 0;ii<3;ii++){
      if((REDWIN == 1) || (BLUEWIN == 1)){
	pthread_exit(0);
      }
      if((board[ii][jj] == 'b') && (board[ii+1][jj] == 'b') && (board[ii+2][jj] == 'b') && (board[ii+3][jj] == 'b')){
	BLUEWIN = 1;
      }else if((board[ii][jj] == 'r') && (board[ii+1][jj] == 'r') && (board[ii+2][jj] == 'r') && (board[ii+3][jj] == 'r')){
	REDWIN = 1;
      }
    }
  }
}

void row_check (void *ptr){
  for(int ii = 0;ii<6;ii++){
    for(int jj = 0;jj<5;jj++){
      if((REDWIN == 1) || (BLUEWIN == 1)){
	pthread_exit(0);
      }
      if((board[ii][jj] == 'b') && (board[ii][jj+1] == 'b') && (board[ii][jj+2] == 'b') && (board[ii][jj+3] == 'b')){
	BLUEWIN = 1;
      }else if((board[ii][jj] == 'r') && (board[ii][jj+1] == 'r') && (board[ii][jj+2] == 'r') && (board[ii][jj+3] == 'r')){
	REDWIN = 1;
      }
    }
  }   
}

void backslash_check (void *ptr){
  for(int ii = 0; ii<3;ii++){
    for(int jj = 0;jj<4;jj++){
      if((REDWIN == 1) || (BLUEWIN == 1)){
	pthread_exit(0);
      }
      if((board[ii][jj] == 'b') && (board[ii+1][jj+1] == 'b') && (board[ii+2][jj+2] == 'b') && (board[ii+3][jj+3] == 'b')){
	BLUEWIN = 1;
      }else if((board[ii][jj] == 'r') && (board[ii+1][jj+1] == 'r') && (board[ii+2][jj+2] == 'r') && (board[ii+3][jj+3] == 'r')){
	REDWIN = 1;
      }
    }
  }
}

void forwardslash_check (void *ptr){
  for(int ii = 0;ii < 3;ii++){
    for(int jj = 3;jj<7;jj++){
      if((REDWIN == 1) || (BLUEWIN == 1)){
	pthread_exit(0);
      }
      if((board[ii][jj] == 'b') && (board[ii-1][jj-1] == 'b') && (board[ii-2][jj-2] == 'b') && (board[ii-3][jj-3] == 'b')){
	BLUEWIN = 1;
      }else if((board[ii][jj] == 'r') && (board[ii-1][jj-1] == 'r') && (board[ii-2][jj-2] == 'r') && (board[ii-3][jj-3] == 'r')){
	REDWIN = 1;
      }
    }
  }
}
