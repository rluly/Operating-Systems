# Operating Systems

## Introduction
This directory is a collection of projects from the OSTEP textbook that I wrote during my senior year at university. It contains the initial utilities, a Linux shell, a
solution to the producer/consumer problem, various threading projects, and a simualation of virtual memory. I hope these files can help you to both learn and understand
these concepts in operating systems.

## Initial Utilities
This subdirectory contains 4 problems based on 4 Linux commands: cat, grep, zip, and unzip. They demonstrate an understanding of command line arguments and file I/O

## Linux Shell
This subdirectory contains a simulation of a Linux shell and supports the use of in-built Linux commands as well as a few custom features. This is the most comprehensive
project of the bunch. It demonstrates an understanding of C/C++ strings, file I/O, CLI interpretation and logic, piping, and file redirection. Unlike the other programs
in this repository, it was written in C++ and not C. If you are currently writing your own shell for uni, good luck, you'll need it :)

## Producer Consumer 
This subdirectory contains a solution to the producer consumer problem. There are producers (produce primes), faulty producers (produce non-primes), and consumers. This
project demonstrates the knowledge of threads, semaphores, mutex locks, and deadlock prevention.

## Threading
This subdirectory contains three seperate programs regarding threading. The first uses threads to analyze various Connect 4 win conditions, the second uses threading to
identify different types of numbers that occur in a given list, and the third converted a given static program into a dynamic program using thread arrays.

## Virtual Memory Simulator
This subdirectory contains a simulation of virtual memory. The program receives a list of logical addresses, checks if they are in the TLB or page table, and if they
aren't, fetches there data by translating the logical addresses into physical ones. This program demonstrates a knowledge of physical/logical/virtual memory addresses,
paging, address translation, and backing store.
