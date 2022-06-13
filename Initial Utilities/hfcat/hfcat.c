#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int main( int argc, char *argv[] ){
  /* printf("%s","hello world"); */
  FILE *fp;
  char buffer[255];
  /* char *buffer; */
  /* buffer = (char *)malloc(INT_MAX); */
  /* if(buffer == NULL){ */
  /*   fprintf(stderr,"Not enough memory available.\n"); */
  /*   exit(1); */
  /* } */
    
  int count;
  int size;

  if(argc < 2){
    exit(0);
  }

  count = 1;

  while(count < argc && (strlen(buffer)!= 0)){
    fp = fopen(argv[count],"r");
    if (fp == NULL){
      printf("hfcat.out: cannot open file\n");
      /* printf("exit with status code 1"); */
      /* fclose(fp); */
      exit(1);
    }

    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    if(size == 0){
      /* printf("do i get here?"); */
      fclose(fp);
      exit(0);
    }

    fclose(fp);
    fp = fopen(argv[count],"r");

    while(fgets(buffer,255,fp)){
      printf("%s",buffer);
    }
    
    count = count + 1;
    fclose(fp);
  }
  /* free(buffer); */
  /* printf("\n"); */
}
