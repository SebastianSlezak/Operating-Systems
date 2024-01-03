# Operating Systems

Welcome to the Operating Systems Projects repository! This repository contains a collection of projects related to operating systems, implemented in the C programming language.

## Table of Contents

1. [Introduction](#introduction)
2. [Projects](#projects)

## Introduction

In this repository you will find various projects focused on showing how my knowledge of the Linux operating system has evolved. Whether you are a beginner or an experienced programmer, you can find projects of varying complexity that will help you better understand the concepts of operating systems and C programming.

## Projects

### 1. [Project 1 - Dynamic Process Tree Creation with Error Handling](https://github.com/SebastianSlezak/Operating-systems/tree/main/Project%201%20-%20Dynamic%20Process%20Tree%20Creation%20with%20Error%20Handling)
   Description: This project includes analyzing descendant processes, drawing a family tree, running new programs in descendant processes, handling errors, and waiting for all descendant processes to finish.

   - program1_1 - Displays process IDs: PID (Process ID), PPID (Parent Process ID), UID (User ID) and GID (Group ID).
   - program1_2 - Creates three child processes using the fork() function, displays their PID, PPID, UID, GID, as well as the process tree before and after the creation of the child processes.
   - program1_3 - Creates three child processes using the fork() function, which then execute another program (./program1_1) using the execl() function, and the parent process waits for each of the child processes to finish and displays their exit codes.

Feel free to explore each project's directory for more details.
