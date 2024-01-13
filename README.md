# Operating Systems

Welcome to the Operating Systems Projects repository! This repository contains a collection of projects related to operating systems, implemented in the C programming language.

## Table of Contents

1. [Introduction](#introduction)
2. [Projects](#projects)
   - [Project 1 - Dynamic Process Tree Creation with Error Handling](#project1)
   - [Project 2 - Thread and Semaphore Synchronization](#project2)
   - [Project 3 - Consumer Manufacturer](#project3)
   - [Project 4 - Client Server](#project4)

## Introduction

In this repository you will find various projects focused on showing how my knowledge of the Linux operating system has evolved. Whether you are a beginner or an experienced programmer, you can find projects of varying complexity that will help you better understand the concepts of operating systems and C programming.

## Projects

<a name="project1"></a>
### 1. [Project 1 - Dynamic Process Tree Creation with Error Handling](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%201%20-%20Dynamic%20Process%20Tree%20Creation%20with%20Error%20Handling)
   Description: This project includes analyzing descendant processes, drawing a family tree, running new programs in descendant processes, handling errors, and waiting for all descendant processes to finish.

   - **program1_1** - Displays process IDs: PID (Process ID), PPID (Parent Process ID), UID (User ID) and GID (Group ID).
     
#### Result of the program
```
PID: 1728792
PPID: 1728727
UID: 2423
GID: 2425
```
     
   - **program1_2** - Creates three child processes using the fork() function, displays their PID, PPID, UID, GID, as well as the process tree before and after the creation of the child processes.
     
#### Result of the program
```
program1_2(58149)---sh(58150)---pstree(58151)
Action for the process parent.
PID: 58152
Action for the process parent.
PPID: 58149
UID: 2423
GID: 2425
Action for the process parent.
PID: 58153
Action for the process parent.
PPID: 58149
UID: 2423
GID: 2425
PID: 58155
PPID: 58152
UID: 2423
GID: 2425
Action for the process parent.
PID: 58154
PPID: 58149
Action for the process parent.
UID: 2423
GID: 2425
PID: 58158
PPID: 58153
UID: 2423
GID: 2425
PID: 58157
PPID: 58152
UID: 2423
Action for the process parent.
GID: 2425
PID: 58159
PPID: 58155
UID: 2423
GID: 2425
program1_2(58149)-+-program1_2(58152)-+-program1_2(58155)---program1_2(58159)
                  |                   `-program1_2(58157)
                  |-program1_2(58153)---program1_2(58158)
                  |-program1_2(58154)
                  `-sh(58156)---pstree(58160)
```
     
   - **program1_3** - Creates three child processes using the `fork()` function, which then execute another program `./program1_1` using the `execl()` function, and the parent process waits for each of the child                            processes to finish and displays their exit codes.
     
#### Result of the program
```
program1_3(1893374)---sh(1893375)---pstree(1893376)
Action for the process parent.
Action for the process parent.
Action for the process parent.
PID: 1893377
PPID: 1893374
UID: 2423
GID: 2425
PID: 1893378
PPID: 1893374
UID: 2423
GID: 2425
PID: 1893379
PPID: 1893374
UID: 2423
GID: 2425
program1_3(1893374)-+-program1_1(1893377)
                   |-program1_1(1893378)
                   |-program1_1(1893379)
                   `-sh(1893380)---pstree(1893381)
The process 1893377 ended with: 0
The process 1893378 ended with: 0
The process 1893379 ended with: 0
```

<a name="project2"></a>
### 2. [Project 2 - Thread and Semaphore Synchronization](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%202%20-%20Thread%20and%20Semaphore%20Synchronization)
   Description: The project uses threads to calculate the sum of elements in a two-dimensional array and synchronize three processes using semaphores, with each process displaying and storing specific information    and then terminating with the `sleep(1)` function, using the `fork()` and `exec()` functions.

   - **program2_1** - The program creates a two-dimensional array of random numbers and then uses two threads to calculate the sum of the elements in each row, after which it sums these partial totals and                               displays the results.

#### Result of the program
```
Sum of the elements of the first row: 45
Sum of the elements of the second row: 36
Total sum: 81
```

   - **semaphore** - This program creates a semaphore, sets it, generates child processes using the `fork()` and `exec()` functions, waits for those processes to finish, and then deletes the semaphore while       
                     saving the results to the "wynik.txt" file.
      - **program2_2** - It creates a semaphore, closes it, displays and saves the PID information for two sections, and then opens the semaphore and terminates the operation.
      - **program2_3** - It creates a semaphore and then executes three sections of code `(t21, t22, t23)`, each writing information to a file and manipulating the semaphore to control the order in which the                                 sections are executed.
      - **program2_4** - It creates a semaphore and then executes three sections of code `(t31, t32, t33)`, each writing information to a file and manipulating the semaphore to control the order in which the                                 sections are executed.

#### Result of the program
```
[MAIN] The semaphore has been created: 1441844
[MAIN] The semaphore has been set
[P1] The semaphore has been created: 1441844
[P2] The semaphore has been created: 1441844
Section t21 with PID = 1893445
[P2] Semaphore 0 has been opened
[P3] The semaphore has been created: 1441844
[P3] Semaphore 0 has been closed
Section t31 with PID = 1893450
Section t32 with PID = 1893450
[P3] Semaphore 1 has been opened
[P2] Semaphore 1 has been closed
Section t22 with PID = 1893445
[P2] Semaphore 2 has been opened
[P1] Semaphore 2 has been closed
Section t11 with PID = 1893444
Section t12 with PID = 1893444
[P1] Semaphore 3 has been opened
[P2] Semaphore 3 has been closed
Section t23 with PID = 1893445
The process 1893444 ended with: 1
[P2] Semaphore 4 has been opened
[P3] Semaphore 4 has been closed
Section t33 with PID = 1893450
The process 1893445 ended with: 1
The process 1893450 ended with: 1
[MAIN] The semaphore has been removed
```

#### wynik.txt
```
Section t11 with PID = 1893444
Section t12 with PID = 1893444
Section t21 with PID = 1893445
Section t22 with PID = 1893445
Section t23 with PID = 1893445
Section t31 with PID = 1893450
Section t32 with PID = 1893450
Section t33 with PID = 1893450
```
<a name="project3"></a>
### 3. [Project 3 - Consumer Manufacturer](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%203%20-%20Consumer%20Manufacturer))
   Description: The classic Producer-Consumer problem is an IT synchronization problem in which two types of processes, the producer and the consumer, share a common resource - a buffer - for the units produced (and consumed), and the producer's task is to produce a product, place it in the buffer and start again, and the consumer's task is to retrieve the product from the buffer.

   - **mainp** - It implements the producer-consumer problem by creating producer and consumer processes that communicate using shared memory, semaphores and message queues.
   - **kons** - The consumer program in the producer-consumer problem, using semaphores for synchronization and buffer handling.
   - **prod** - program acts as a producer in the producer-consumer problem, using semaphores for synchronization and buffer handling.

#### Result of the program
```
blank message sent 0
blank message sent 1
blank message sent 2
blank message sent 3
blank message sent 4
blank message sent 5
blank message sent 6
blank message sent 7
blank message sent 8
blank message sent 9
PID=2219869 has been written to the buffer [PRODUCER]
PID=2219871 has been written to the buffer [PRODUCER]
PID=2219870 has been written to the buffer [PRODUCER]
PID=2219873 has been written to the buffer [PRODUCER]
PID=2219872 has been written to the buffer [PRODUCER]
PID=2219874 has been written to the buffer [PRODUCER]
PID=2219875 has been written to the buffer [PRODUCER]
PID=2219876 has been written to the buffer [PRODUCER]
PID=2219877 has been written to the buffer [PRODUCER]
PID=2219878 has been written to the buffer [PRODUCER]
PID=2219880 has been written to the buffer [PRODUCER]
PID=2219879 has been written to the buffer [PRODUCER]
Process 2219881 has gained access to the semaphore set [CONSUMER]
Process 2219882 has gained access to the semaphore set [CONSUMER]
Process 2219883 has gained access to the semaphore set [CONSUMER]
Process 2219884 has gained access to the semaphore set [CONSUMER]
Process 2219885 has gained access to the semaphore set [CONSUMER]
Process 2219886 has gained access to the semaphore set [CONSUMER]
Process 2219887 has gained access to the semaphore set [CONSUMER]
Process 2219888 has gained access to the semaphore set [CONSUMER]
Process 2219889 has gained access to the semaphore set [CONSUMER]
Process 2219890 has gained access to the semaphore set [CONSUMER]
Process 2219891 has gained access to the semaphore set [CONSUMER]
Process 2219892 has gained access to the semaphore set [CONSUMER]
```

<a name="project4"></a>
### 4. [Project 4 - Client Server](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%204%20-%20Client%20Server)

Description: A client-server program is a computer system architecture that allows the division of tasks, where the server provides services to clients who submit service requests to it.

   - **client** - This code is responsible for communicating with the server using message queues in Unix, which sends messages to the server and receives responses.
   - **server** - This code is a simple server program communicating with a client using Unix message queues, which receives messages from the client, converts them to uppercase and sends them back.

#### Result of client
```
C[3582514]: Enter text to send:
test message
C[3582514]: Sending... "test message" -> SERVER
C[3582514]: Retrieved: "TEST MESSAGE" addressed to 3582514
C[3582514]: Enter text to send:
```

#### Result of server
```
^C terminates server
S: I look forward to the message...
S: Received from: 3582514~test message
S: Sending... TEST MESSAGE -> 3582514
S: I look forward to the message...
```

* * *
Feel free to explore each project's directory for more details.
