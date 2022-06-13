# OS-thread-fun

## Spring 2022
## Due February 22, by 15:30

---


## Thread Arrays (20 points)
Type in the attached program that uses pthreads and verify that it compiles and executes correctly.  Modify the program to use a thread array rather than individual thread variables.  Pass into your program the number of threads to create via command line arguments.  The name of this program should be `threadarray.c`.

``` c
/* Includes */
#include <unistd.h>     
#include <sys/types.h>   
#include <errno.h>      
#include <stdio.h>      
#include <stdlib.h>     
#include <pthread.h>    
#include <string.h>     

/* prototype for thread routine */
void print_message_function ( void *ptr );

/* struct to hold data to be passed to a thread
this shows how multiple data items can be passed to a thread */
typedef struct str_thdata
{
   int thread_no;
   char message[100];
} thdata;

/* main function */
int main()
{
   /* local variables */
   pthread_t thread1, thread2;  /* thread variables */
   thdata data1, data2;         /* structs to be passed to threads */

   /* initialize data to pass to thread 1 */
   data1.thread_no = 1;
   strcpy(data1.message, "Hello!");

   /* initialize data to pass to thread 2 */
   data2.thread_no = 2;
   strcpy(data2.message, "Hi!");

   /* create threads 1 and 2 */    
   pthread_create (&thread1, NULL, (void *) &print_message_function, (void *) &data1);
   pthread_create (&thread2, NULL, (void *) &print_message_function, (void *) &data2);

   /* Main block now waits for both threads to terminate, before it exits
   If main block exits, both threads exit, even if the threads have not
   finished their work */ 
   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);

   /* return to calling environment */  
   exit(0);

} /* main() */



/**
* print_message_function is used as the start routine for the threads used
* it accepts a void pointer 
**/
void print_message_function (void *ptr)
{
   thdata *data;            
   data = (thdata *) ptr;  /* type cast to a pointer to thdata */

   /* do the work */
   printf("Thread %d says %s \n", data->thread_no, data->message);

   pthread_exit(0); /* exit */
} /

```


---


## Threaded Connect Four Win Validator (40 points)

Connect Four is a two-person game similar to tic-tac-toe.  Each player alternate turns placing a red or blue checker on a 6x7 board trying to make four-in-a-row vertically, horizontally, or diagonally.  Write a multithreaded C program that reads layout of a Connect Four game board from a file and determines if there is a winner (Red, Blue, No Winner).  your program will read the contents of a file, create threads to do the work, and then output the results to standard output.  Here are the requirements:

1. The name of your program must be `cfwin.c`.
2. The name of the file will be passed on the command line (only one file).  If the file name is not present on the command line, then your program should print the message `Usage: ./cfwin file`.  If the file does not exist, then print the message `./cfwin: File _name_ not found`.
3. The main thread should open the file, read the contents of the Connect Four board into memory, create the "worker" threads, and print the result.
4. The contents of the file are as follows.  Connect Four has a 6x7 2-D board containing a '.' if the square is open, 'r' if it occupied by a red piece, and 'b' if it is occupied by a blue piece.  No other characters will be used on the board.  
5. Each input file will either have 1 winner of any color or no winners.
6. Create a worker thread that will check for a winner in each of the 7 vertical columns.
7. Create a worker thread that will check for a winner in each of the 6 horizontal rows.
8. Create a worker thread that will check for a winner in "backslash" diagonals.
9. Create a worker thread that will check for a winner in "forward slash" diagonals.
10. The main thread will output one of three messages: `WINNER: Red`, `WINNER: Blue`, or `NO WINNER`

```
$ ./cfwin g1.txt
WINNER: Red
$
$ ./cfwin g2.txt
NO WINNER
$
$ ./cfwin
Usage: ./cfwin file
$
$ ./cfwin badfile.txt
./cfwin: File badfile.txt not found.
$
```

---

## Threaded Number Identifier (40 points)

Write a program that accepts an unspecified number integers from the command line.  The main thread will create six worker threads that will identify types of integers entered on the command line (negative, zero, one, even, odd, prime).  The threads should take turns printing their respective integers supplied on the command line. The name of this program must be `numbers.c`. 

1. The negative thread will identify and print all negative numbers.
2. The zero thread will identify and print all zeros.
3. The one thread will identify and print all ones.
4. The prime thread will identify and print all prime numbers.
5. The even thread will identify and print all even numbers > 0.
6. The odd thread will identify and print all odd numbers > 1 and not prime.


Note: that the main thread will output nothing – the output will be performed by the threads that the main thread creates.  The order of integers listed on the command line must be the order output to standard output.  Here is an example:

```
$ ./numbers 0 -1 1 5 9 8
ZERO:            0
NEG:            -1
ONE:             1
PRIME:           5
ODD:             9
EVEN:            8
$
$ ./numbers 121 2048 2147483647 0 -8192 1
ODD:           121
EVEN:         2048
PRIME:  2147483647
ZERO:            0
NEG:         -8192
ONE:             1
$
```

---

## Additional Notes

- Your programs must be written in C and use pthreads.
- Write well documented programs.  Use procedures and functions.
- Include a Makefile that will create *all* programs in one step (just by typing 'make' at the command line).
- Be sure to `git push` your project regularly.
- I will test your project on `lovelace.cs.tcu.edu`.
- This is an individual project.
- This project is worth 100 points. 

