# OS-vmsim

---


## Description
The purpose of this project is to simulate the translation of virtual to physical address, storing these pages in a TLB and page table, and replacing them when need be. You, as a user, may choose the address file, the swap file, and which page replacement algorithim you would like the program to use. It will complete the simulatation based on those choices and output the results to stdout.

---

## Assembly
The source code exists in the vmsim.c file. To assemble an executable, run the `make` command. This will automatically call the following command:

```
gcc -o vmsim vmsim.c
```

---

## Execution
You will execute the vmsim file via the command line. To properly run the program, enter the following prototype into the command line:

```
./vmsim -a addressfile -s swapfile -m Mode
```

The flags in the command line stand for:

1. (-a) The text file you want the program to read addresses from
2. (-s) The binary file you want the program to treat as physical memory
3. (-m) The page replacement algorithm you want the program to run (FIFO,LRU,DEMAND)

---

## Summary
Once you run the program, you will be shown each entry in the page table with the following stats:

```
Page Number: #, Offset: #, Value: #
```

Once every entry in the page table has been printed, statistics for the simulation will appear as follows:

```
Number of Translated Addresses: #
Page Faults: #
Page Fault Rate: #
TLB Hits: #
TLB Hit Rate: #
```