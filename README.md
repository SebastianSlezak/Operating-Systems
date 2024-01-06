# Operating Systems

Welcome to the Operating Systems Projects repository! This repository contains a collection of projects related to operating systems, implemented in the C programming language.

## Table of Contents

1. [Introduction](#introduction)
2. [Projects](#projects)
   - [Project 1 - Dynamic Process Tree Creation with Error Handling](#project1)
   - [Project 2 - Thread and Semaphore Synchronization](#project2)

## Introduction

In this repository you will find various projects focused on showing how my knowledge of the Linux operating system has evolved. Whether you are a beginner or an experienced programmer, you can find projects of varying complexity that will help you better understand the concepts of operating systems and C programming.

## Projects

<a name="project1"></a>
### 1. [Project 1 - Dynamic Process Tree Creation with Error Handling](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%201%20-%20Dynamic%20Process%20Tree%20Creation%20with%20Error%20Handling)
   Description: This project includes analyzing descendant processes, drawing a family tree, running new programs in descendant processes, handling errors, and waiting for all descendant processes to finish.

   - **program1_1** - Displays process IDs: PID (Process ID), PPID (Parent Process ID), UID (User ID) and GID (Group ID).
     
     #### Result of the program
     ![program1_1](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/09a264c9-a282-4691-b5fb-c56a85f5bd5a)
     
   - **program1_2** - Creates three child processes using the fork() function, displays their PID, PPID, UID, GID, as well as the process tree before and after the creation of the child processes.
     
     #### Result of the program
     ![program1_2part1](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/a32117c1-b015-41a6-897e-cc244e37c86f)
     ![program1_2part2](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/efe88b95-6f38-40e5-8c7f-7d1506fd11e4)
     
   - **program1_3** - Creates three child processes using the fork() function, which then execute another program (./program1_1) using the execl() function, and the parent process waits for each of the child                            processes to finish and displays their exit codes.
     
     #### Result of the program
     ![program1_3](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/c51d87c6-bd04-468d-bbc9-9ce250471918)

<a name="project2"></a>
### 2. [Project 2 - Thread and Semaphore Synchronization](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%202%20-%20Thread%20and%20Semaphore%20Synchronization)
   Description: The project uses threads to calculate the sum of elements in a two-dimensional array and synchronize three processes using semaphores, with each process displaying and storing specific information    and then terminating with the sleep(1) function, using the fork() and exec() functions.

   - **program2_1** - The program creates a two-dimensional array of random numbers and then uses two threads to calculate the sum of the elements in each row, after which it sums these partial totals and                               displays the results.

     #### Result of the program
     ![program2_1](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/95b7a29b-a11e-4eae-8c52-2646c70092d7)

   - **semaphore** - This program creates a semaphore, sets it, generates child processes using the fork() and exec() functions, waits for those processes to finish, and then deletes the semaphore while saving                         the results to the "wynik.txt" file.
      - **program2_2** - It creates a semaphore, closes it, displays and saves the PID information for two sections, and then opens the semaphore and terminates the operation.
      - **program2_3** - It creates a semaphore and then executes three sections of code (t21, t22, t23), each writing information to a file and manipulating the semaphore to control the order in which the                                 sections are executed.
      - **program2_4** - It creates a semaphore and then executes three sections of code (t31, t32, t33), each writing information to a file and manipulating the semaphore to control the order in which the                                 sections are executed.

      #### Result of the program
      ![Semaphore Project](https://github.com/SebastianSlezak/Operating-systems/assets/41953849/c3ad148a-b98a-43a5-815f-b917afa17917)

* * *
Feel free to explore each project's directory for more details.
