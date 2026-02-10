#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a function pointer type for task actions
typedef void (*TaskAction)(void*);

// Task Structure representing a unit of work
typedef struct {
    int id;
    char *description;
    void *arg;          // Generic argument (polymorphism)
    TaskAction action;  // Callback function
} Task;

// Manager Structure to hold the list of tasks
typedef struct {
    Task **tasks;
    size_t count;
    size_t capacity;
} TaskScheduler;

// --- User Definitions ---

void sayHello(void *arg) {
    char *name = (char*)arg;
    printf(" [Exec] Hello, %s!\n", name);
}

void printSquare(void *arg) {
    int *num = (int*)arg;
    printf(" [Exec] Square of %d is %d\n", *num, (*num) * (*num));
}

// --- System Functions ---

Task* task_create(int id, const char *desc, void *arg, TaskAction act) {
    Task *t = (Task*)malloc(sizeof(Task));
    if (!t) return NULL;

    t->id = id;
    t->arg = arg;
    t->action = act;
    
    // Allocate memory for string description
    t->description = strdup(desc); // strdup automatically mallocs and copies
    if (!t->description) {
        free(t);
        return NULL;
    }

    return t;
}

void scheduler_init(TaskScheduler *ts) {
    ts->count = 0;
    ts->capacity = 4; // Start with a reasonable size
    ts->tasks = (Task**)malloc(ts->capacity * sizeof(Task*));
    if (!ts->tasks) {
        fprintf(stderr, "Critical Error: Failed to init scheduler memory.\n");
        exit(1);
    }
}

void scheduler_add(TaskScheduler *ts, Task *t) {
    if (!t) return;

    // Resize array if full
    if (ts->count == ts->capacity) {
        size_t new_capacity = ts->capacity * 2;
        Task **temp = (Task**)realloc(ts->tasks, new_capacity * sizeof(Task*));
        
        if (!temp) {
            fprintf(stderr, "Error: Failed to expand scheduler memory.\n");
            return;
        }
        
        ts->tasks = temp;
        ts->capacity = new_capacity;
    }

    ts->tasks[ts->count] = t;
    ts->count++;
    printf("Task added: %s (ID: %d)\n", t->description, t->id);
}

void scheduler_run(TaskScheduler *ts) {
    printf("\n--- SCHEDULER STARTED ---\n");
    for (size_t i = 0; i < ts->count; i++) {
        Task *t = ts->tasks[i];
        printf("Running Task ID %d: %s...", t->id, t->description);
        
        // Execute the callback
        if (t->action) {
            t->action(t->arg);
        }
    }
    printf("--- SCHEDULER FINISHED ---\n");
}

void scheduler_cleanup(TaskScheduler *ts) {
    for (size_t i = 0; i < ts->count; i++) {
        free(ts->tasks[i]->description);
        free(ts->tasks[i]);
    }
    free(ts->tasks);
    printf("\n[Memory] All resources freed.\n");
}

int main() {
    TaskScheduler scheduler;
    scheduler_init(&scheduler);

    // Context data (simulating sensor data or user inputs)
    int val1 = 5;
    int val2 = 12;
    char user1[] = "Roman";
    char user2[] = "Admin";

    // Adding tasks with different data types but same structure
    scheduler_add(&scheduler, task_create(1, "User Greeting", user1, sayHello));
    scheduler_add(&scheduler, task_create(2, "Calc Square", &val1, printSquare));
    scheduler_add(&scheduler, task_create(3, "Calc Square Large", &val2, printSquare));
    scheduler_add(&scheduler, task_create(4, "System Check", user2, sayHello));

    // Execute all
    scheduler_run(&scheduler);

    // Clean up
    scheduler_cleanup(&scheduler);

    return 0;
}