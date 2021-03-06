#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  FILE *fp;
  char buffer[255];

  int count;
  int size;

  if(argc < 2){
    printf("hfgrep.out: searchterm [file ...]\n");
    exit(1);
  }
  else if(argc == 2){
    while(fgets(buffer,255,stdin)){
      if(strstr(buffer,argv[1]) != NULL){
	    printf("%s",buffer);
	  }
    }
  }
  else{
    count = 2;
    while(count < argc){
      fp = fopen(argv[count],"r");
      if(fp == NULL){
	printf("hfgrep.out: cannot open file\n");
	exit(1);
      }

      fseek(fp,0,SEEK_END);
      size = ftell(fp);
      if(size == 0){
	fclose(fp);
	exit(0);
      }

      fclose(fp);
      fp = fopen(argv[count],"r");

      while(fgets(buffer,255,fp)){
	if(strstr(buffer,argv[1]) != NULL){
	    printf("%s",buffer);
	}
      }

      count = count + 1;
      fclose(fp);
    
    }
  }
}
