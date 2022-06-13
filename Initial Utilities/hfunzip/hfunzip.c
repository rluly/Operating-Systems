#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int main( int argc, char *argv[] ){
  /* printf("%s","hello world"); */
  FILE *fp;
  /* char buffer[255]; */
  int number;
  char letter;
  /* char *buffer; */
  /* buffer = (char *)malloc(INT_MAX); */
  /* if(buffer == NULL){ */
  /*   fprintf(stderr,"Not enough memory available.\n"); */
  /*   exit(1); */
  /* } */
    
  int count;
  int size;

  if(argc < 2){
    printf("hfunzip.out: file1 [file2 ...]\n");
    exit(1);
  }

  count = 1;

  while(count < argc){
    fp = fopen(argv[count],"r");
    if (fp == NULL){
      printf("hfunzip.out: file1 [file2 ...]\n");
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

    while(!feof(fp)){
      fread(&number,sizeof(int),1,fp);
      fread(&letter,sizeof(char),1,fp);
      while(number > 0){
	printf("%c",letter);
	number = number - 1;
      }
    }
    
    count = count + 1;
    fclose(fp);
  }
  /* free(buffer); */
  /* printf("\n"); */
}
