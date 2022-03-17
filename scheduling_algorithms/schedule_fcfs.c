#include "schedulers.h"
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "cpu.h"


struct node *head = NULL; //need to use to free whole list
static int pCount = 0;

struct node* pickNextTask(){
    struct node *ptr = head;
    struct node *nextNode = ptr;

    while(ptr){

        if(ptr->task->tid < nextNode->task->tid){
            nextNode = ptr;
        }
        ptr = ptr->next;
        
    }
    return nextNode;
}

// add a task to the list 
void add(char *name, int priority, int burst){

    Task *task = malloc(sizeof(Task)); //need to free
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    task->tid = pCount++;

    insert(&head, task);
}

// invoke the scheduler
void schedule(){
    
    printf("***\nFCFS: all tasks will be run (for their full burst) in the order that they request\nthe CPU... or in this case the order that they were read in.\n***\n");

    for(int i=0; i<pCount;i++){

        printf("\n* picking next task in the order it was read in *\n");
        struct node *n = pickNextTask();
        run(n->task, n->task->burst);
        delete(&head,n->task);
        free(n->task->name);
        free(n->task);
        free(n);
    }

    printf("\n* No more tasks! *\n");
}