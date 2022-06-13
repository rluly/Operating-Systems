//*********************************************************
//
// Ryan Luly
// Operating Systems
// Project #1: Writing Your Own Shell: hfsh
//
//*********************************************************


//*********************************************************
//
// Includes and Defines
//
//*********************************************************
#include <cstdlib>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> //sys includes from Stackoverflow user MOHAMED
#include <sys/wait.h>
#include <sys/stat.h> //for chmod call
#include <sys/time.h> //for gettimeofday call,since removed
#include <fcntl.h>
#include <vector>
#include <csignal>
#include <iostream>
#include <dirent.h> //include and all relevant code from pubs.opengroup.org
// #include <tabulate/table.hpp> //from github user p-ranav


#define STR_MYEXIT "myexit"

//Globals
extern char **environ; //From github user mightydeveloper
int quitcounter = 0;
int stopcounter = 0;
int hupcounter = 0;
int intcounter = 0;
std::vector< std::string > history;


//*********************************************************
//
// Type Declarations
//
//*********************************************************
using namespace std;
// using namespace tabulate;


//*********************************************************
//
// Extern Declarations
//
//*********************************************************
using namespace std;
extern "C"
{
  extern char **gettoks();
} 


//*********************************************************
//
// Function Prototypes
//
//*********************************************************
void myhist();
void forweb(char* path);
void nls();
void extcmd(char** toks);
void byte_count();
void print_prompt();
void nls_print_dir();
void print_signals();
void signal_handler(int signal_num);
int pipe_check(char** toks);
int bg_check(char** toks);
int in_check(char** toks);
int out_check(char** toks);

int pipe_check(char** toks){
  int result = 0;
  int ii;

  for(ii=0;toks[ii] != NULL;ii++){
    if(strcmp("|",toks[ii]) == 0){
      result = ii;
      // cout << "This is a pipe\n";
    }
  }
  
  return result;
}

int bg_check(char** toks){
  int result = 0;
  int ii;
  int jj;

  for(ii=0;toks[ii] != NULL;ii++){}

  ii = ii-1;
  // printf("%s",toks[ii]);
  if(strcmp("&",toks[ii]) == 0){
    // cout << "This is a bg\n";
    result = 1;
  }
  if(result == 1){
    toks[ii] = NULL;
  }
  // for(int jj=0;toks[jj] != NULL;jj++){
  //   printf("%s\n",toks[jj]);
  // }
  
  return result;
}

int in_check(char** toks){
  int result = 0;
  int ii;

  for(ii = 0;toks[ii] != NULL; ii ++){
    if(strcmp(toks[ii],"<") == 0){
      result = ii;
    }
  }
  
  return result;
}

int out_check(char** toks){
  int result;
  int ii;

  for(ii = 0;toks[ii] != NULL;ii ++){
    if(strcmp(toks[ii],">") == 0){
      result = ii;
    }
  }
  
  return result;
}

void signal_handler(int signal_num){
  cout << "\nThe interrupt signal is " << signal_num << "\n";

  if(signal_num == 2){
    intcounter = intcounter + 1;
  }
  else if(signal_num == 20){
    stopcounter = stopcounter + 1;
  }
  else if(signal_num == 3){
    quitcounter = quitcounter + 1;
  }
  else if(signal_num == 1){
    hupcounter = hupcounter + 1;
  }
  else{
    cout << "There was a signal error.\n";
  }
  
  return;
}

void print_signals(){
  printf("__________________\n");
  printf("|  %d  |  SIGINT  |\n",intcounter);
  printf("|  %d  |  SIGSTP  |\n",stopcounter);
  printf("|  %d  |  SIGHUP  |\n",hupcounter);
  printf("|  %d  |  SIGQUIT |\n",quitcounter);
  printf("__________________\n");
  
  return;
}

void myhist(){
  int ii;

  for(ii=0;ii<history.size();ii++){
    std::cout << ii << ":  " << history.at(ii) << '\n';
  }
  
  return;
}

void forweb(char* path){
  DIR* dir;
  struct dirent *dp;
  string prefix(path);

  if((dir = opendir(path)) == NULL){
    printf("cannot be opened.\n");
    exit(1);
  }

  while((dp = readdir(dir)) != NULL){
    // printf("%s\n",dp->d_name);
    string suffix(dp->d_name);
    string tmp_path = prefix + suffix;
    int length = tmp_path.length();
    char fpath[length+1];
    strcpy(fpath,tmp_path.c_str());
    // printf("%s\n",fpath);

    if(dp->d_type == 4){
      chmod(fpath,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }
    else{
      chmod(fpath,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH);
    }
  }

  closedir(dir);
  return;
}

void extcmd(char** toks){
  // printf("I got here\n");
  int pipeloc = pipe_check(toks);
  char **tmppipe1;
  char **tmppipe2;
  if(pipeloc > 0){
    tmppipe1 = (char**)malloc(pipeloc * sizeof(char*));
    for(int ii=0;ii<pipeloc;ii++){
      tmppipe1[ii] = (char*)malloc(20*sizeof(char));
    }
    int endcheck;
    for(endcheck = 0;toks[endcheck] != NULL; endcheck ++){}
    tmppipe2 = (char**)malloc((endcheck-pipeloc -1) * sizeof(char*));
    for(int jj=0;jj<(endcheck-pipeloc-1);jj++){
      tmppipe2[jj] = (char*)malloc(20*sizeof(char));
    }
    for(int rr=0;rr<pipeloc;rr++){
      strcpy(tmppipe1[rr],toks[rr]);
      // printf("%s\n",pipe1[rr]);
    }
    // printf("This is endcheck %d\n",endcheck);
    for(int ss = 0;(pipeloc + 1)<endcheck;ss++){
      strcpy(tmppipe2[ss],toks[pipeloc + 1]);
      // printf("0: %s\n",tmppipe2[ss]);
      pipeloc = pipeloc + 1;
      // printf("2 -  %s",pipe2[ss]);
    }
  }
  
  int bgcheck = bg_check(toks);
  
  int incheck = in_check(toks);
  int outcheck = out_check(toks);
  FILE *fp;
  FILE *fp2;
  // printf("%d",outcheck);
  if(incheck > 0){
    fp = fopen(toks[incheck+1],"r");
    if(fp == NULL){
      printf("input file error\n");
      exit(1);
    }
  }
  if(outcheck > 0){
    fp2 = fopen(toks[outcheck+1],"w");
    if(fp2 == NULL){
      printf("output file error\n");
      exit(1);
    }
  }

  // toks[incheck] = NULL;
  // if(incheck > 0){
  //   for(int cc = incheck;toks[cc + 1] != NULL;cc++){
  //   }
  // }

  if(outcheck > 0){
    for(outcheck = outcheck;toks[outcheck] != NULL;outcheck++){
      toks[outcheck] = NULL;
    }
  }
  
  string prefix = "/bin/";
  string suffix(toks[0]);
  string tmp_path = prefix + suffix;
  int length = tmp_path.length();
  char path[length+1];
  strcpy(path,tmp_path.c_str());
  int length2 = 0;
  string tmp_path2;
  if(pipeloc > 0){
    string suffix2(tmppipe2[0]);
    tmp_path2 = prefix + suffix;
    int length2 = tmp_path2.length();
  }
  char path2[length2 + 1];
  if(pipeloc > 0){
    strcpy(path2,tmp_path2.c_str());
  }
  // for(int aa = 0;tmppipe1[aa] != NULL; aa++){
  //   printf("1 : %s\n", tmppipe1[aa]);
  // }
  // for(int bb = 0;tmppipe2[bb] != NULL; bb++){
  //   printf("2 : %s\n", tmppipe2[bb]);
  // }
  int lnk[2]; //learned piping from StackOverflow user Aif
  char foo[4096];
  int standardout = dup(1);
  int standardin = dup(0);
  pipe(lnk);
  pid_t pid = fork();
  int status;
  // printf("start %d %d %d\n",pipeloc,outcheck,bgcheck);
  if(pid<0){
    exit(1);
  }
  else if(pid == 0){
    // printf("I am the child\n");
    if((pipeloc == 0) & (outcheck == 0) & (incheck == 0)){   
      if(execve(path,toks,environ) <  0){
	exit(1);
      }
    }
    else if(outcheck > 0 | pipeloc > 0){
      // printf("do i get here?\n");
      dup2(lnk[1],1);
      close(lnk[1]);
      close(lnk[0]);
      if(outcheck > 0){
	if(execve(path,toks,environ) < 0){
	  exit(1);
	}
      }else if(pipeloc > 0){
	if(execve(path,tmppipe1,environ) < 0){
	  exit(1);
	}
    }
    // wait(NULL);
    // while(1);
  }
  }
    else{
    // cout << "We get here\n";
      if((pipeloc == 0) & (outcheck ==0) & (incheck == 0)){
      // printf("do i get here %d %d %d",pipeloc,outcheck,bgcheck);
      waitpid(pid,&status,0);
      dup2(standardin,0);
      dup2(standardout,1);
    }
    else if(bgcheck == 1){
      wait(NULL);
    }
    else if(outcheck > 1){
      dup2(standardout,1);
      // printf("i am the parent");
      read(lnk[0],foo,sizeof(foo));
      fwrite(foo,1,sizeof(foo),fp2);
      dup2(standardin,0);
      // printf("Output: %s",foo);
      wait(NULL);
      }
    else if(pipeloc > 0){
      waitpid(pid,&status,0);
      pid_t pid2 = fork();
      int status2;
      if(pid2<0){
      	printf("fork error\n");
      	exit(1);
      }
      else if(pid2 == 0){
	dup2(lnk[0],0);
	close(lnk[1]);
	close(lnk[0]);
      	if(execve(path2,tmppipe2,environ) < 0){
      	  exit(1);
      	}
      }
      else if(pid2 > 0){
      	waitpid(pid2,&status2,0);
      }
    }
  }
  if(incheck > 0){
    fclose(fp);
  }
  if(outcheck > 0){
    fclose(fp2);
  }
  return;
}
void nls(){
  nls_print_dir();
  DIR* dir;
  struct dirent *dp;

  if((dir = opendir(".")) == NULL){
    printf("cannot be opened.\n");
    exit(1);
  }

  while((dp = readdir(dir)) != NULL){
    if(dp->d_type == 8){
      printf("\033[0;37m"); //this is white because I can't find grey hope thats ok
      printf("%s\n",dp->d_name);
    }else if(dp->d_type == 10){
      printf("\033[1;31m");
      printf("%s\n",dp->d_name);
    }else if(dp->d_type == 6){
      printf("\033[0;32m");
      printf("%s\n",dp->d_name);
    }
    // printf("%d\n",dp->d_type);
    printf("\033[0;37m");
  }
  closedir(dir);
  return;
}

void nls_print_dir(){
  DIR* dir;
  struct dirent *dp;

  if((dir = opendir(".")) == NULL){
    printf("cannot be opened.\n");
    exit(1);
  }

  printf("\033[0;34m");
  
  while((dp = readdir(dir)) != NULL){
    if(dp->d_type == 4){
      printf("%s\n",dp->d_name);
    }
  }
  printf("\033[0;37m");
  printf("------------------\n");
  closedir(dir);
  return;
}

// This is my personal function. It takes all the standard files(dp_type == 8),
// counts the bytes of each file, adds them up and returns the result.
void byte_count(){
  DIR* dir;
  struct dirent *dp;
  struct stat buffer;
  int status;
  int result;
  result = 0;
  string prefix = "./";

  if((dir = opendir(".")) == NULL){
    printf("cannot be opened.\n");
    exit(1);
  }

  while((dp = readdir(dir)) != NULL){
    if(dp->d_type == 8){
      string suffix(dp->d_name);
      string tmp_path = prefix + suffix;
      int length = tmp_path.length();
      char path[length+1];
      strcpy(path,tmp_path.c_str());
      if(stat(path,&buffer) == 0){
	result = result + buffer.st_size;
      }
    }
  }

  printf("The number of total bytes in the current subdirectory is %d\n",result);
  
  return;
}

void print_prompt(){
  printf("\033[0;36m");
  char cwd[255];
  getcwd(cwd,sizeof(cwd));
  // struct timeval current_time;
  // time_t t;
  // struct tm *info;
  // gettimeofday(&current_time,NULL);
  // t = current_time.tv_sec;
  // info = localtime(&t);
  printf("hfsh : %s > ",cwd);
  printf("\033[0;37m");
}
//*********************************************************
//
// Main Function
//
//*********************************************************
int main( int argc, char *argv[] )
{
  // local variables
  int ii;
  char **toks;
  int retval;
  // int callcount;
  string cmd;

  // initialize local variables
  ii = 0;
  toks = NULL;
  retval = 0;
  // callcount = 0;

  //Signal handler setup
  signal(SIGINT,signal_handler);
  signal(SIGTSTP,signal_handler);
  signal(SIGHUP,signal_handler);
  signal(SIGQUIT,signal_handler);

  // main (infinite) loop
  while(true){
    print_prompt();
    // get arguments
    toks = gettoks();

    if(toks[0] != NULL){
      // simple loop to echo all arguments
      for(ii=0; toks[ii] != NULL; ii++){
        // cout << "Argument " << ii << ": " << toks[ii] << endl;
	string arg(toks[ii]);
	cmd = cmd + ' ' + arg;
      }
      history.push_back(cmd);
      cmd.clear();
      // std::cout << history.at(0) << '\n';

      if(!strcmp(toks[0], STR_MYEXIT)){
	print_signals();
        exit(0);
      }
      else if(!strcmp(toks[0],"nls"))
	nls();
      else if(!strcmp(toks[0],"myhist"))
	myhist();
      else if(!strcmp(toks[0],"forweb"))
	forweb(toks[1]);
      else if(!strcmp(toks[0],"bytecount")) //personal function cmd
	byte_count();
      else
	extcmd(toks);
      
    }
  }

// return to calling environment
return( retval );
}
