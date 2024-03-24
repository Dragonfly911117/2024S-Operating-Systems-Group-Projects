## Project 1: Linux Kernel Modules

- I. Kernel modules overview
- II. Loading and removing kernel modules
- III. The `/proc` file system
- IV. Assignment:
    1. Design a kernel module that creates a /proc file named `/proc/jiffies` that reports the current value of jiffies
       when the `/proc/jiffies` file is read, such as with the command: `cat /proc/jiffies`. Be sure to remove
       `/proc/jiffies` when the module is removed.
    2. Design a kernel module that creates a proc file named /proc/seconds that reports the number of elapsed seconds
       since the kernel module was loaded.
        - This will involve using the value of jiffies as well as the HZ rate.
        - When a user enters the command
          `cat /proc/seconds` your kernel module will report the number of seconds that have elapsed since the kernel
          module was first loaded.
        - Be sure to remove `/proc/seconds` when the module is
          removed.

## Project 2: Linux Kernel Module for Task

- Information  
  In this project, you will write a kernel module that uses `/proc` file system for displaying a task’s information in a
  Linux system.
- Be sure to review the programming project in Chap.2 before you begin this project
- It can be completed using the Linux virtual machine provided with the textbook 
- Part I – Writing to the `/proc` file system 
- Part II – Reading from the `/proc` file system