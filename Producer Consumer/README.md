# OS-producer-consumer

---


## Description
The purpose of this project is to simulate the producer-consumer problem. There exists two types of producers: regular producers who will add prime numbers to the buffer and faulty producers who will add non prime numbers to the buffer. If adding an item makes the buffer full, a warning will be displayed. Consumers will wait until the buffer is not empty, and then will consume the first item in the buffer. If that number is non-prime or a consumer took the last remaining item in the buffer, a warning will be displayed.

- - -

## Assembly
The source code exists in the pcundefined.c file. To assemble an executable, run the `make` command. This will run the following command:

```
gcc -o pcexe pcundefined.c -lpthread

```

---

### Execution
Execution of the pcexe file will run via the command line. To run this program, enter a command of the following prototype:

```
./gcexe -n 10 -l 5 -p 2 -f 1 -c 2
```

The flags in this command stand for:

1. (-n) The number of items to produce per producer thread
2. (-l) The length of the buffer
3. (-p) The number of producer threads
4. (-f) The number of faulty producer threads
5. (-c) The number of consumer threads

---

### Summary
Once you run the program, the terminal will print a summary of the actions taken in the following format:

```
(PRODUCER 1 writes 1/10 37): (1): [ 37 ]
(PR*D*C*R 1 writes 1/10 18): (2): [ 37 18 ]
(CONSUMER 1 reads 1   18): (0): []*NOT PRIME*

=============
PRODUCER / CONSUMER SIMULATION COMPLETE
==================
Number of Items Per Producer Thread: (-n)
Size of Buffer: (-l)
Number of Producer Threads: (-p)
Number of Faulty Producer Threads: (-f)
Number of Consumer Threads: (-c)

Number of Times Buffer Became Full 6
Number of Times Buffer Became Empty 4

Number of Non-primes Detected 8
Total Number of Items Consumed: 30
Thread 1: 22
Thead 2: 8
...

Total Simulation Time: .500 seconds

```
