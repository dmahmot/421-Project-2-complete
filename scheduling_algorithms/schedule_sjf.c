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
    printf("\n* searching for task with shortest burst... *\n");
    while(ptr){//find task with shortest burst
        if(ptr->task->burst < nextNode->task->burst){
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
    
    printf("***\nSJF: Tasks will be picked based on which has the shortest burst (i.e. whichever has the shortest runtime\n***\n");

    for(int i=0; i<pCount;i++){
        struct node *n = pickNextTask();
        run(n->task, n->task->burst);
        delete(&head,n->task);
        free(n->task->name);
        free(n->task);
        free(n);
    }

    printf("\n* No more tasks! *\n");
}