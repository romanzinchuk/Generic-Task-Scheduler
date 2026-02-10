# Generic Task Scheduler in C

## Overview
This project implements a **polymorphic task scheduler** using C function pointers and `void*` arguments. 
It demonstrates how embedded systems manage asynchronous events and callbacks without the overhead of an Operating System.

## Key Concepts Demonstrated
- **Function Pointers:** implementing the "Callback" design pattern.
- **Polymorphism in C:** Using `void*` to pass different data types (int, string, structs) to a generic execution handler.
- **Dynamic Memory Management:** Safe usage of `malloc`, `realloc`, and `free` to manage a growing list of tasks.
- **Structs:** Encapsulating data and behavior (methods) in a single unit.

## How it Works
1. The `Task` struct holds a pointer to a function (`ActionFunc`) and a pointer to data (`void* arg`).
2. The `Scheduler` stores an array of these tasks.
3. When `scheduler_run()` is called, it iterates through the list.
4. It invokes the specific function for each task, casting the `void*` argument back to its original type inside the callback.

## Technical Details
- **Language:** C (Standard C99)
- **Memory Safety:** Includes checks for allocation failures and cleanup routines to prevent memory leaks.
- **Scalability:** The scheduler dynamically resizes its capacity using `realloc` when the task limit is reached.

## Build & Run
```bash
gcc main.c -o scheduler
./scheduler
