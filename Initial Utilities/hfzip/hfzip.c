#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int main(int argc, char *argv[]){
  FILE *fp;
  /* FILE *outfp; */
  char buffer[255];
  int count;
  /* int limit; */
  int size;
  /* int argcheck; */
  int length;
  int a;
  char temp;
  /* char buffer2[255]; */
  int occ;

  if(argc < 2){
    printf("hfzip.out: file1 [file2 ...]\n");
    exit(1);
  }

  /* argcheck = strcmp(argv[argc-2],">") */;
  count = 1;
  occ = 0;

  /* if(argcheck == 0){ */
  /*   outfp = fopen(argv[argc-1],"w"); */
  /*   limit = argc - 2; */
  /* }else{ */
  /*   outfp = fopen("output.z","w"); */
  /*   limit = argc; */
  /* } */
  
  while(count < argc){
    fp = fopen(argv[count],"r");
    if(fp == NULL){
      printf("hfzip.out: cannot open file\n");
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
       /* printf("%s",buffer); */
       length = strlen(buffer);
       for(a = 0; a < length; a++){
	 if(temp != buffer[a]){
	   occ = 1;
	   temp = buffer[a];
	 }else{
	   occ = occ + 1;
	 }
	 /* temp = buffer[a]; */
	 /* printf("%c",buffer[a]); */
	 /* occ = 1; */
	 while(a + 1 < length && buffer[a] == buffer[a+1]){
	   occ = occ + 1;
	   a = a + 1;
	 }
	 /* sprintf(buffer2,"%d",occ); */
	 if(a + 1 < length || (count + 1) >= argc){
	   fwrite(&occ,sizeof(int),1,stdout);
	   printf("%c",temp);
	 }
	 /* memset(buffer2,0,strlen(buffer2)); */
       }
    count = count + 1;
    fclose(fp);
  }
  }
}
  /* fclose(outfp); */

