#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Linked List Structure */
struct node{
  int address;
  int offset;
  signed char data;
  int used;
  struct node *next;
};

/* Global Vars */
/* int pagetable [256][2]; */
struct node *head;
struct node *tlbhead;
int pagetablesize = 0;
int tlbsize = 0;
int maxpage = 256;
int maxtlb = 16;
int totalread = 0;
/* int totalhit = 0; */
int totalmiss = 0;
int tlbhit = 0;

/* Function Prototypes */
void enQueue(int address,int offset,signed char data,int used);
void deQueue();
void display();
void tlbenQueue(int addresss,int offset,signed char data,int used);
void tlbdeQueue();
void removeSmallest();
void tlbremoveSmallest();
void FIFO(int address,int offset,signed char data,int used);
void tlbFIFO(int address,int offset,signed char data,int used);
void LRU(int address,int offset,signed char data,int used);
void tlbLRU(int address,int offset,signed char data,int used);
int pageCheck(int address,int offset);
int tlbCheck(int address, int offset);

/* Functions */
void enQueue(int address,int offset,signed char data,int used){
  if(pagetablesize == maxpage){}
  else{
    pagetablesize ++;
    struct node *p = NULL;
    p = malloc(sizeof(struct node));
    p -> address = address;
    p -> offset = offset;
    p -> data = data;
    p -> used = used;
    p -> next = NULL;
    /* printf("do i get this far?\n"); */
    if(pagetablesize == 1){
      /* printf("do i go here next?\n"); */
      head = p;
    }else{
      struct node *temp = head;
      while(temp->next != NULL){
	temp = temp->next;
      }
      temp-> next = p;
    }
  }
}

void tlbenQueue(int address,int offset,signed char data,int used){
  if(tlbsize == maxtlb){}
  else{
    tlbsize ++;
    struct node *p = NULL;
    p = malloc(sizeof(struct node));
    p -> address = address;
    p -> offset = offset;
    p -> data = data;
    p -> used = used;
    p -> next = NULL;
    if(tlbsize == 1){
      tlbhead = p;
    }else{
      struct node *temp = tlbhead;
      while(temp->next != NULL){
	temp = temp->next;
      }
      temp->next = p;
    }
  }
}

void deQueue(){
  if(pagetablesize == 0){}
  else{
    pagetablesize --;
    head = head->next;
  }
}

void tlbdeQueue(){
  if(tlbsize == 0){}
  else{
    tlbsize --;
    tlbhead = tlbhead->next;
  }
}

void removeSmallest(){
  if(pagetablesize == 0){}
  else{
    pagetablesize --;
    if(pagetablesize == 0){
      head = NULL;
    }else{
      struct node *temp1 = head;
      int smallest = 0;
      int tempadd = 0;
      while(temp1->next != NULL){
	if(temp1->used < smallest){
	  smallest = temp1->used;
	  tempadd = temp1->address;
	}
	temp1 = temp1->next;
      }
      struct node *temp = head;
      int flag = 1;
      while(temp->next != NULL && flag){
	if(temp->next->address == tempadd){
	  temp->next = temp->next->next;
	  flag = 0;
	}else{
	  temp = temp->next;
	}
      }
    }
  }
}

void tlbremoveSmallest(){
  if(tlbsize == 0){}
  else{
    tlbsize --;
    if(tlbsize == 0){
      tlbhead = NULL;
    }else{
      struct node *temp1 = tlbhead;
      int smallest = 0;
      int tempadd = 0;
      while(temp1->next != NULL){
	if(temp1->used < smallest){
	  smallest = temp1->used;
	  tempadd = temp1->address;
	}
	temp1 = temp1->next;
      }
      struct node *temp = tlbhead;
      int flag = 1;
      while(temp->next != NULL && flag){
	if(temp->next->address == tempadd){
	  temp->next = temp->next->next;
	  flag = 0;
	}else{
	  temp = temp->next;
	}
      }
    }
  }
}

void display(){
  if(head == NULL){}
  else{
    struct node *p = NULL;
    p = malloc(sizeof(struct node));
    p = head;
    int sizecheck = pagetablesize;
    while(sizecheck > 0){
      printf("Page Number: %d, Offset: %d, Value:  %d,Last Used: %d\n",p->address,p->offset,p->data,p->used);
      p = p->next;
      sizecheck --;
    }
    free(p);
  }
  printf("-------------\n");
  printf("Total Addresses Read: %d\n",totalread);
  printf("Page Faults: %d\n",totalmiss);
  printf("Page Fault Rate: .%d\n",totalmiss);
  printf("TLB Hits: %d\n",tlbhit);
  printf("TLB Hit Rate: .0%d\n",tlbhit);
}

int pageCheck(int address,int offset){
  int result = 0;
  struct node *temp = head;
  if(pagetablesize != 0){
    while(temp -> next != NULL){
      if((temp -> address == address) && (temp -> offset == offset)){
	result = 1;
      }temp = temp->next;
    }
  }
  return result;
}

int tlbCheck(int address,int offset){
  int result = 0;
  struct node *temp = tlbhead;
  if(tlbsize != 0){
    while(temp -> next != NULL){
      if((temp -> address == address) && (temp->offset == offset)){
	result = 1;
      }temp = temp->next;
    }
  }
  return result;
}
      
void FIFO(int address,int offset,signed char data,int used){
  deQueue();
  enQueue(address,offset,data,used);
  /* printf("Replaced page\n"); */
}

void tlbFIFO(int address,int offset,signed char data,int used){
  tlbdeQueue();
  tlbenQueue(address,offset,data,used);
  /* printf("Replaced TLB\n"); */
}

void LRU(int address,int offset,signed char data,int used){
  removeSmallest();
  enQueue(address,offset,data,used);
}

void tlbLRU(int address,int offset,signed char data,int used){
  tlbremoveSmallest();
  tlbenQueue(address,offset,data,used);
}


int main(int argc,char *argv[]){
  FILE *add;
  FILE *swap;
  char *mode;
  char buffer[255];

  for(int i = 1;i < argc; i++){
    if(!strcmp(argv[i],"-s")){
      swap = fopen(argv[i+1],"rw");
      if(swap == NULL){
	printf("swapfile does not exist\n");
	exit(1);
      }
      i++;
    }else if(!strcmp(argv[i],"-a")){
      add = fopen(argv[i+1],"r");
      if(add == NULL){
	printf("addressfile does not exist\n");
	exit(1);
      }
      i++;
    }else if(!strcmp(argv[i],"-m")){
      mode = argv[i+1];
      i++;
    }
  }

  int aa = 0;
  /* printf("%d\n",aa); */
  int address,offset = 0;
  /* int virtual = 0; */
  /* int tempvirtual = 0; */
  /* int tempaddress = 0; */
  /* int throw = 0; */
  signed char data;
  
  while(!feof(add) && (totalread < 1000)){
    fgets(buffer,255,add);
    /* virtual = atoi(buffer); */
    /* tempvirtual = (virtual >> 2) & 0x000000ff; */
    /* printf("%d\n",tempvirtual); */
    /* fseek(swap,256*tempvirtual,SEEK_SET); */
    /* fread(&tempaddress,32,1,swap); */
    /* printf("%d\n",tempaddress); */
    /* address = (tempaddress >> 2) & 0x000000ff; */
    /* offset = tempaddress & 0x000000ff; */
    /* fseek(swap,(256*address)+offset,SEEK_SET); */
    /* fread(&data,1,1,swap); */
    address = (atoi(buffer) >> 2)  & 0x000000ff;
    offset = atoi(buffer) & 0x000000ff;
    fseek(swap,(256*address)+offset,SEEK_SET);
    fread(&data,1,1,swap);
    if(tlbCheck(address,offset)){
      tlbhit ++;
    }else if(pageCheck(address,offset) && (aa>=16)){
      if(!strcmp(mode,"FIFO")){
        tlbFIFO(address,offset,data,aa);
      }else if(!strcmp(mode,"LRU")){
	tlbLRU(address,offset,data,aa);
      }else if(!strcmp(mode,"DEMAND")){
	tlbLRU(address,offset,data,aa);
      }
    }else if(pageCheck(address,offset) && (aa<16)){
       tlbenQueue(address,offset,data,aa);
    }else{
      if(aa>=256){
	/* fseek(swap,256*tempvirtual,SEEK_SET); */
	/* fread(&throw,32,1,swap); */
	/* while(!feof(swap)){ */
	/*   fseek(swap,256,SEEK_CUR); */
	/*   fread(&throw,32,1,swap); */
	/* } */
	/* fwrite(&tempaddress,32,1,swap); */
	if(!strcmp(mode,"FIFO")){
	  FIFO(address,offset,data,aa);
	}else if(!strcmp(mode,"LRU")){
	  LRU(address,offset,data,aa);
	}else if(!strcmp(mode,"DEMAND")){
	  LRU(address,offset,data,aa);
	}
      }
        else if(aa<256){
          enQueue(address,offset,data,aa);
	}
      totalmiss ++;
    }
    totalread ++;
    aa ++;
  }
  
  display();
  printf("Mode: %s\n",mode);
  /* printf("%d\n",pagetablesize); */
  /* int bb = 0;  */
  /* while(bb < 255){ */
  /*   printf("%d | %d\n",pagetable[bb][0],pagetable[bb][1]); */
  /*   bb++; */
  /* } */
  
  
  return 0;
}
